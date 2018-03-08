// g++ -std=c++11 -I/home/yangy/work/local/include -L/home/yang/work/local/lib -ltiff -ltiffxx -o memstream memstream.cpp


#include <tiffio.h>
#include <tiffio.hxx>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <iostream>
#include <cstdint>
using namespace std;

//
void swap2bytes(void *targetp)
{
    unsigned char * tp = (unsigned char *)targetp;
    unsigned char a = *tp;
    *tp = *(tp+1);
    *(tp+1) = a;
}

void swap4bytes(void *targetp)
{
    unsigned char * tp = (unsigned char *)targetp;
    unsigned char a = *tp;
    *tp = *(tp+3);
    *(tp+3) = a;
    a = *(tp+1);
    *(tp+1) = *(tp+2);
    *(tp+2) = a;
}

void copydata ( unsigned char *psrc, uint32 stride_src, unsigned char *pdst, uint32 stride_dst, uint32 width, uint32 len ) {
    uint32 i;
    for ( uint32 i=0; i<len; i++, psrc+=stride_src, pdst+=stride_dst )
        memcpy(pdst, psrc, width*sizeof(unsigned char));
}

//
class Timer
{
public:
    Timer()
        : mStart(),
          mStop()
    {
        update();
    }

    void update()
    {
        mStart = chrono::high_resolution_clock::now();
        mStop  = mStart;
    }

    double elapsedMs()
    {
        mStop = chrono::high_resolution_clock::now();
        chrono::milliseconds elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(mStop - mStart);
        return elapsed_ms.count();
    }

private:
    chrono::high_resolution_clock::time_point mStart;
    chrono::high_resolution_clock::time_point mStop;
};

//
int main(int argc, char *argv[])
{
    //
    char *filename = NULL;
    if(argc>1)
        filename = argv[1];

    //
    Timer timer;

    //
    ifstream inFile;
    inFile.open(filename);
    if (!inFile) {
        cerr << "Unable to open file "<<filename;
        return -1;
    }

    stringstream input_TIFF_stream;
    input_TIFF_stream << inFile.rdbuf();

    istream& input_TIFF = input_TIFF_stream;

    //string input_TIFF = static_cast<stringstream const&>(stringstream() << inFile.rdbuf()).str();
    //std::ostringstream buf; std::ifstream input (path.c_str()); buf << input.rdbuf(); string str = buf.str();

    //
    inFile.close();

    //
    cout << "loading TIFF file into the memory cost "<< timer.elapsedMs() << "ms" <<endl;
    timer.update();

    //
    uint16 spp, bpp, orientation, photo, comp, planar_config;
    int check, StripsPerImage, LastStripSize;
    uint32 XSIZE;
    uint32 YSIZE;
    uint16 Cpage;
    uint16 Npages;
    uint32 rps;

    //
    TIFF* mem_TIFF = TIFFStreamOpen("MemTIFF", &input_TIFF);

    //
    if(TIFFGetField(mem_TIFF, TIFFTAG_IMAGEWIDTH, &XSIZE))
    {

    }
    if(TIFFGetField(mem_TIFF, TIFFTAG_IMAGELENGTH, &YSIZE))
    {

    }
    if(TIFFGetField(mem_TIFF, TIFFTAG_BITSPERSAMPLE, &bpp))
    {

    }
    if(TIFFGetField(mem_TIFF, TIFFTAG_SAMPLESPERPIXEL, &spp))
    {

    }
    check=TIFFGetField(mem_TIFF, TIFFTAG_PAGENUMBER, &Cpage, &Npages);
    if (!check || Npages==0)
    {
        Npages = 0;
        do {
            Npages++;
        } while ( TIFFReadDirectory(mem_TIFF) );
    }

    long sz0 = XSIZE;
    long sz1 = YSIZE;
    long sz2 = Npages;
    long sz3 = spp;
    int datatype = bpp/8;

    long imgsize = sz0*sz1*sz2*sz3;

    uint8 *img=NULL;
    try
    {
        img = new uint8 [imgsize*datatype];
    }
    catch(...)
    {
        cout<<"fail to alloc memory"<<endl;
        return -1;
    }

    //
    int b_swap=TIFFIsByteSwapped(mem_TIFF);

    if(TIFFGetField(mem_TIFF, TIFFTAG_PHOTOMETRIC, &photo))
    {

    }
    if(TIFFGetField(mem_TIFF, TIFFTAG_COMPRESSION, &comp))
    {

    }
    if(TIFFGetField(mem_TIFF, TIFFTAG_PLANARCONFIG, &planar_config))
    {

    }

    if(TIFFIsTiled(mem_TIFF))
    {
        cout<<"tiled TIFF"<<endl;

        uint32 tiledepth, tilelength, tilewidth;

        if(TIFFGetField(mem_TIFF, TIFFTAG_TILEDEPTH, &tiledepth))
        {

        }
        if ( spp > 1 )
        {
            if ( TIFFGetField(mem_TIFF, TIFFTAG_PLANARCONFIG, &planar_config) )
            {
                if ( planar_config > 1 )
                {

                }
            }
        }

        if(TIFFGetField(mem_TIFF, TIFFTAG_TILEWIDTH, &tilewidth))
        {

        }
        if(TIFFGetField(mem_TIFF, TIFFTAG_TILELENGTH, &tilelength))
        {

        }
        long tilesize = TIFFTileSize(mem_TIFF);
        long tilenum = TIFFNumberOfTiles(mem_TIFF);

        cout<<"test ... ... "<<tilenum<<" "<<comp<<" "<<tiledepth<<" "<<tilelength<<" "<<tilewidth<<" "<<tilesize<<endl;

        //
        long tilenum_width, tilenum_length;
        ttile_t tile;
        unsigned char *psrc;
        unsigned char *pdst;
        uint32 stride_src;
        uint32 stride_dst;
        int i; // row index in the slice of the top left pixel of the current block to be filled
        int j; // column index in the slice of the top left pixel of the current block to be filled
        uint32 width; // width of the current block to be filled (in pixels)
        uint32 len; // length of the current block to be filled (in pixels)
        int page;

        // tiling is in x,y only
        tilenum_width  = (sz0 % tilewidth) ? (sz0 / tilewidth) + 1 : sz0 / tilewidth;
        tilenum_length = (sz1 % tilelength) ? (sz1 / tilelength) + 1 : sz1 / tilelength;

//        unsigned char *data = NULL;
//        try
//        {
//            data = new unsigned char[tilesize];
//        }
//        catch(...)
//        {
//            cout<<"fail to alloc memory"<<endl;
//            return -1;
//        }

        stride_src = tilewidth * spp; // width of tile (in bytes)
        stride_dst = sz0 * spp; // width of subregion (in bytes)

        page = 0;
        do
        {
            //psrc = ((unsigned char *)data);
            pdst = img; // the buffer has the size of the subregion
            len = tilelength;
            tile = TIFFComputeTile(mem_TIFF,0,0,0,0); // index of the first tile to be copied in the current row of tiles
            for ( i=0; i<sz1; )
            {
                width = tilewidth;
                for ( j=0; j<sz0; )
                {
                    TIFFReadEncodedTile(mem_TIFF,tile,pdst,(tsize_t) -1); // read tile into tile buffer
                    //copydata (psrc,stride_src,pdst,stride_dst,(width * spp),len); // copy the block
                    j += width;
                    tile++; // index of the next tile in the same row of tiles
                    //psrc = ((unsigned char *)data) + ((i % tilelength)*tilewidth) * spp; // the block in the next tile begins just after (i % tilelength) rows
                    pdst += width * spp; // the block in the image buffer move forward of width pixels
                    width = (((tile%tilenum_width) + 1) * tilewidth <= sz0) ? tilewidth : (sz0%tilewidth); // if the next tile in the row is all within the subregion, width is tilewidth otherwise it is shorter
                }
                i += len;
                tile = TIFFComputeTile(mem_TIFF,0,i,0,0); // index of the first tile to be copied in the current row of tiles
                //psrc = ((unsigned char *)data) + ((i % tilelength)*tilewidth)*spp; // in the first tile of the next row of tiles skip (i % tilelength) rows plus (startj % tilewidth) pixels
                pdst = img + (i * stride_dst); // the block in the image buffer begin after (i-starti) rows
                len = (((tile/tilenum_width) + 1) * tilelength <= sz1) ? tilelength : (sz1%tilelength); // if the next row of tiles is all within the subregion, len is tilelength otherwise it is shorter
            }

            page++;

            cout<<"page "<<page<<endl;

        }
        while ( page < 1 && TIFFReadDirectory(mem_TIFF));

        //
//        if(data)
//        {
//            delete []data;
//        }

    }
    else if(TIFFGetField(mem_TIFF, TIFFTAG_ROWSPERSTRIP, &rps))
    {

    }
    else
    {
        cout<<"Unsupported TIFF"<<endl;
        return -1;
    }
    cout<<"size ... "<<YSIZE<<" "<<XSIZE<<endl;

    // swap the data bytes if necessary
    if (b_swap)
    {
        int i;
        size_t total = sz0 * sz1 * spp;
        if (bpp/8 == 2)
        {
            for (i=0;i<total; i++)
            {
                swap2bytes((void *)(img+2*i));
            }
        }
        else if (bpp/8 == 4)
        {
            for (i=0;i<total; i++)
            {
                swap4bytes((void *)(img+4*i));
            }
        }
    }

    //
    TIFFClose(mem_TIFF);

    cout << "read TIFF image from the memory cost "<< timer.elapsedMs() << "ms" <<endl;

    //
    cout << timer.elapsedMs() << "ms" <<endl;


    //
    long n=0;
    for(long i=0; i<imgsize; i++)
    {
        if(datatype==2)
        {
            uint16 *p = (uint16 *)img;

            if(p[i])
                n++;
        }
    }
    cout<<"non-zero voxels "<<n<<endl;



    //
    if(img)
    {
        delete []img;
    }

    return 0;
}
