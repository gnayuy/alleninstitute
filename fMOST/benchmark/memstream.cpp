// g++ -std=c++11 -fopenmp -I/home/yangy/work/local/include -L/home/yang/work/local/lib -ltiff -ltiffxx -lpthread -o memstream memstream.cpp
// export OMP_NUM_THREADS=8

#include <tiffio.h>
#include <tiffio.hxx>

#include <omp.h>

#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <iterator>

#include <vector>
#include <thread>
#include <random>
#include <future>

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

int rawwrite(char *fileNameToWrite, unsigned char *buf, long size, int datatype)
{
    //
    ofstream fout(fileNameToWrite);

    if(fout.is_open())
    {
        if(datatype==1)
        {
            copy(buf, buf+size, ostream_iterator<unsigned char>(fout, ""));
        }
        else if(datatype==2)
        {
            uint16 *bufp = (uint16*)buf;
            //copy(bufp, bufp+size, ostream_iterator<unsigned short>(fout, ""));

            for(long i=0; i<size; i++)
            {
                fout << bufp[i];
                if(bufp[i]>4096)
                {
                    cout<<"ODD"<<endl;
                    return -1;
                }
            }
        }
        else
        {

        }

        fout.close();
    }
    else
    {
        cout << "Fail to open the file "<< fileNameToWrite << endl;
        return -1;
    }

    //
    return 0;
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
    int N = 100000;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> ini(0.0, 10.0);
    std::vector<int> vec(N);
    for(int i = 0; i < N; ++i)
    {
        vec[i] = ini(mt);
    }
    int Q = 1;
    for(int Th : {1, 2, 4, 8, 16})
    {
        auto start = std::chrono::system_clock::now();

        std::vector<std::future<void>> fut(Th);

        for(int t = 0; t < Th; ++t)
        {
            fut[t] = std::async(std::launch::async, [&vec, &N, Q]{
                int end = N / Q;
                for(int i = 0; i < end; ++i)
                {
                    vec[i] = 2 * vec[i] + 3 * vec[i] - vec[i];
                }
            });
        }
        for(int t = 0; t < Th; ++t)
        {
            fut[t].get();
        }

        auto end = std::chrono::system_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Th = " << Th << " : " << dur << " ms." << std::endl;
    }

    //
    char *filename = NULL;
    if(argc>1)
    {
        filename = argv[1];
    }
    else
    {
        cout<<"No inputs\n";
        return 0;
    }

    //
    cout << "  Number of processors available = " << omp_get_num_procs ( ) << "\n";
    cout << "  Number of threads = " << omp_get_max_threads ( ) << "\n";
    omp_set_num_threads(8);

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

    cout<<"image size ... "<<YSIZE<<" "<<XSIZE<<" "<<spp<<endl;

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

    if(planar_config==PLANARCONFIG_CONTIG)
    {
        cout<<"PLANARCONFIG_CONTIG"<<endl;

        //
        if(TIFFIsTiled(mem_TIFF))
        {
            cout<<"tiled TIFF"<<endl;
        }
        else if(TIFFGetField(mem_TIFF, TIFFTAG_ROWSPERSTRIP, &rps))
        {
            cout<<"strip TIFF"<<endl;

            //
            tsize_t stripsize  = TIFFStripSize(mem_TIFF);
            tstrip_t ns = TIFFNumberOfStrips(mem_TIFF);

            StripsPerImage =  (sz1 + rps - 1) / rps;
            LastStripSize = sz1 % rps;

            cout<<"stripsize "<<stripsize<<" "<<rps<<" "<<LastStripSize<<endl;
            cout<<"there are "<<ns<<" strips"<<" "<<StripsPerImage<<endl;

            if (LastStripSize==0)
            {
                cout<<"LastStripSize is zero"<<endl;

                //#pragma omp parallel
                {
                    //cout<<"There are "<<omp_get_num_threads()<<" threads"<<endl;
                    //#pragma omp for
                    //for (tstrip_t s = 0; s < ns; s++)
                    {
                        //tsize_t cc = (row + rps > sz1) ? TIFFVStripSize(mem_TIFF, sz1 - row) : stripsize;
//                        if (TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize) < 0)
//                        {
//                            TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) s);
//                        }
                        //row += rps;

                        //_TIFFmemset(bufp, 0, stripsize);
                        //TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize);
                        //bufp += stripsize;





                    }
                }

                if (comp==1)
                {
                    for (tstrip_t s = 0; s < ns; s++)
                    {
                        TIFFReadRawStrip(mem_TIFF, s, img + s*stripsize, stripsize);
                    }
                }
                else
                {
                    for (tstrip_t s = 0; s < ns; s++)
                    {
                        TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize);
                    }
                }



//                std::vector<std::future<void>> f(ns);

//                for(uint32 s = 0; s < ns; ++s)
//                {
//                    f[s] = std::async(std::launch::async, [&mem_TIFF, &img, &ns, stripsize, s]{
//                        TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize);
//                    });
//                }
//                for(uint32 s = 0; s < ns; ++s)
//                {
//                    f[s].get();
//                }




            }
            else
            {
                unsigned char * bufp = img;

                tstrip_t ns = TIFFNumberOfStrips(mem_TIFF);
                //uint32 row = 0;

                cout<<"there are "<<ns<<" strips"<<" "<<StripsPerImage<<endl;

                //#pragma omp parallel
                {
                    //cout<<"There are "<<omp_get_num_threads()<<" threads"<<endl;
                    //#pragma omp for
                    for (tstrip_t s = 0; s < ns-1; s++)
                    {
                        //tsize_t cc = (row + rps > sz1) ? TIFFVStripSize(mem_TIFF, sz1 - row) : stripsize;
    //                    if (TIFFReadEncodedStrip(mem_TIFF, s, bufp, stripsize) < 0)
    //                    {
    //                        TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) s);
    //                    }
                        //row += rps;

                        //_TIFFmemset(bufp, 0, stripsize);
                        TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize);
                        //bufp += stripsize;
                    }
                }

                cout<<"end parallel lzwdecode\n";

                bufp = img + sz1 - 1 - LastStripSize;
                if (TIFFReadEncodedStrip(mem_TIFF, ns-1, bufp, LastStripSize) < 0)
                {
                    TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) (ns-1));
                }
            }

            //uint32 rawstripsize;
            //TIFFRawStripSize(mem_TIFF, rawstripsize);
            //cout<<"stripsize "<<stripsize<<" "<<rawstripsize<<endl;
        }
        else
        {
            cout<<"Unsupported TIFF"<<endl;
            return -1;
        }

    }
    else if(planar_config==PLANARCONFIG_SEPARATE)
    {
        cout<<"PLANARCONFIG_SEPARATE"<<endl;
    }
    else if(planar_config==PLANARCONFIG_SEPARATE)
    {
        cout<<"PLANARCONFIG_SEPARATE"<<endl;
    }
    else
    {
        cout<<"Unkown configuration!"<<endl;
        return -3;
    }


//    if(TIFFIsTiled(mem_TIFF))
//    {
//        cout<<"tiled TIFF"<<endl;

//        uint32 tiledepth, tilelength, tilewidth;

//        if(TIFFGetField(mem_TIFF, TIFFTAG_TILEDEPTH, &tiledepth))
//        {

//        }
//        if ( spp > 1 )
//        {
//            if ( TIFFGetField(mem_TIFF, TIFFTAG_PLANARCONFIG, &planar_config) )
//            {
//                if ( planar_config > 1 )
//                {

//                }
//            }
//        }

//        if(TIFFGetField(mem_TIFF, TIFFTAG_TILEWIDTH, &tilewidth))
//        {

//        }
//        if(TIFFGetField(mem_TIFF, TIFFTAG_TILELENGTH, &tilelength))
//        {

//        }
//        long tilesize = TIFFTileSize(mem_TIFF);
//        long tilenum = TIFFNumberOfTiles(mem_TIFF);

//        cout<<"test ... ... "<<tilenum<<" "<<comp<<" "<<tiledepth<<" "<<tilelength<<" "<<tilewidth<<" "<<tilesize<<endl;

//        //
//        long tilenum_width, tilenum_length;
//        ttile_t tile;
//        unsigned char *psrc;
//        unsigned char *pdst;
//        uint32 stride_src;
//        uint32 stride_dst;
//        int i; // row index in the slice of the top left pixel of the current block to be filled
//        int j; // column index in the slice of the top left pixel of the current block to be filled
//        uint32 width; // width of the current block to be filled (in pixels)
//        uint32 len; // length of the current block to be filled (in pixels)
//        int page;

//        // tiling is in x,y only
//        tilenum_width  = (sz0 % tilewidth) ? (sz0 / tilewidth) + 1 : sz0 / tilewidth;
//        tilenum_length = (sz1 % tilelength) ? (sz1 / tilelength) + 1 : sz1 / tilelength;

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

//        stride_src = tilewidth * spp; // width of tile (in bytes)
//        stride_dst = sz0 * spp; // width of subregion (in bytes)

//        page = 0;
//        do
//        {
//            psrc = ((unsigned char *)data);
//            pdst = img; // the buffer has the size of the subregion
//            len = tilelength;
//            tile = TIFFComputeTile(mem_TIFF,0,0,0,0); // index of the first tile to be copied in the current row of tiles
//            for ( i=0; i<sz1; )
//            {
//                width = tilewidth;
//                for ( j=0; j<sz0; )
//                {
//                    TIFFReadEncodedTile(mem_TIFF,tile,pdst,(tsize_t) -1); // read tile into tile buffer
//                    copydata (psrc,stride_src,pdst,stride_dst,(width * spp),len); // copy the block
//                    j += width;
//                    tile++; // index of the next tile in the same row of tiles
//                    psrc = ((unsigned char *)data) + ((i % tilelength)*tilewidth) * spp; // the block in the next tile begins just after (i % tilelength) rows
//                    pdst += width * spp; // the block in the image buffer move forward of width pixels
//                    width = (((tile%tilenum_width) + 1) * tilewidth <= sz0) ? tilewidth : (sz0%tilewidth); // if the next tile in the row is all within the subregion, width is tilewidth otherwise it is shorter
//                }
//                i += len;
//                tile = TIFFComputeTile(mem_TIFF,0,i,0,0); // index of the first tile to be copied in the current row of tiles
//                psrc = ((unsigned char *)data) + ((i % tilelength)*tilewidth)*spp; // in the first tile of the next row of tiles skip (i % tilelength) rows plus (startj % tilewidth) pixels
//                pdst = img + (i * stride_dst); // the block in the image buffer begin after (i-starti) rows
//                len = (((tile/tilenum_width) + 1) * tilelength <= sz1) ? tilelength : (sz1%tilelength); // if the next row of tiles is all within the subregion, len is tilelength otherwise it is shorter
//            }

//            page++;

//            cout<<"page "<<page<<endl;

//        }
//        while ( page < 1 && TIFFReadDirectory(mem_TIFF));

//        //
//        if(data)
//        {
//            delete []data;
//        }

//    }
//    else if(TIFFGetField(mem_TIFF, TIFFTAG_ROWSPERSTRIP, &rps))
//    {
//        cout<<"strip TIFF"<<endl;
//    }
//    else
//    {
//        cout<<"Unsupported TIFF"<<endl;
//        return -1;
//    }


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


    // save as raw file
    if(argc>2)
    {
        rawwrite(argv[2], img, imgsize, datatype);
    }

    //
    if(img)
    {
        delete []img;
    }

    return 0;
}
