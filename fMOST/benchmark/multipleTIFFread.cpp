// g++ -std=c++11 -fopenmp -I/home/yangy/work/local/include -L/home/yang/work/local/lib -ltiff -ltiffxx -lpthread -o multipleTIFFread multipleTIFFread.cpp
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
            //copy(bufp, bufp+size, ostream_iterator<uint16>(fout, ""));

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


int tiffread(stringstream *dataStreamInMemory, unsigned char *&img, long &sz0, long &sz1, long &sz2, long &sz3, int &datatype)
{
    cout<<"call tiffread read stream "<<&dataStreamInMemory<<endl;

//    dataStreamInMemory->seekg (0, dataStreamInMemory->end);
//    int len = dataStreamInMemory->tellg();

//    cout<<"length of istream "<<len<<endl;

//    if(len<0)
//        return -1;

//    dataStreamInMemory->seekg (0, dataStreamInMemory->beg);

    //
    uint16 spp, bpp, orientation, photo, comp, planar_config;
    int check, StripsPerImage, LastStripSize;
    uint32 XSIZE;
    uint32 YSIZE;
    uint16 Cpage;
    uint16 Npages;
    uint32 rps;

    //
    TIFF* mem_TIFF = TIFFStreamOpen("MemTIFF", (istream *)dataStreamInMemory);

    cout<<"sucess open"<<endl;

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

    sz0 = XSIZE;
    sz1 = YSIZE;
    sz2 = Npages;
    sz3 = spp;
    datatype = bpp/8;

    long imgsize = sz0*sz1*sz2*sz3;

    if(img)
    {
        delete []img;
    }
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

    //
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

            if (LastStripSize==0)
            {
                //
                if (comp==1)
                {
                    tstrip_t s;
                    for (s = 0; s < ns; s++)
                    {
                        if (TIFFReadRawStrip(mem_TIFF, s, img + s*stripsize, stripsize)<0)
                        {
                            TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) s);
                        }
                    }
                }
                else
                {
                    tstrip_t s;
                    for (s = 0; s < ns; s++)
                    {
                        if (TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize)<0)
                        {
                            TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) s);
                        }
                    }
                }
            }
            else
            {
                for (tstrip_t s = 0; s < ns-1; s++)
                {
                    if (TIFFReadEncodedStrip(mem_TIFF, s, img + s*stripsize, stripsize) < 0)
                    {
                        TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) s);
                    }

                    unsigned char *bufp = img + sz1 - 1 - LastStripSize;
                    if (TIFFReadEncodedStrip(mem_TIFF, ns-1, bufp, LastStripSize) < 0)
                    {
                        TIFFError(TIFFFileName(mem_TIFF), "Error, can't read strip %lu", (unsigned long) (ns-1));
                    }
                }
            }
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

    //
    dataStreamInMemory->clear();

    //
    return 0;
}

//
int main(int argc, char *argv[])
{
    //
    int nFiles = 0;
    vector<char*> fileList;
    if(argc>1)
    {
        for(int i=1; i<argc; i++)
        {
            char *filename = argv[i];
            nFiles++;
            fileList.push_back(filename);
        }
    }
    else
    {
        cout<<"No inputs\n";
        return -1;
    }

    //
    omp_set_num_threads(omp_get_max_threads());

    //
    Timer timer;

    // load TIFFs into memory
    int i;
    vector<stringstream*> dataInMemory;
    vector<unsigned char*> imgList;

    for(i=0; i<nFiles; i++)
    {
        dataInMemory.push_back(new stringstream);
    }

    for(i=0; i<nFiles; i++)
    {
        cout<<"load "<<fileList[i]<<endl;

        ifstream inFile;
        inFile.open(fileList[i]);
        if (!inFile) {
            cerr << "Unable to open file "<<fileList[i];
            return -1;
        }

        dataInMemory.push_back(new stringstream);
        *dataInMemory[i] << inFile.rdbuf();

        //
        inFile.close();

        //
        unsigned char *img = NULL;
        imgList.push_back(img);
    }

    //
    cout << "sequence loading "<<nFiles<<" TIFF files into the memory cost "<< timer.elapsedMs() << "ms" <<endl;
    timer.update();

    // read TIFFs from the memory
    #pragma omp parallel
    {
        #pragma omp for
        for(i=0; i<nFiles; i++)
        {
            long sz0, sz1, sz2, sz3;
            int datatype;

            tiffread(dataInMemory[i],imgList[i],sz0,sz1,sz2,sz3,datatype);
        }
    }
    cout << "read TIFF image from the memory cost "<< timer.elapsedMs() << "ms" <<endl;

    //
    for(i=0; i<nFiles; i++)
    {
        if(imgList[i])
        {
            delete [](imgList[i]);
        }
    }

    //
    return 0;
}
