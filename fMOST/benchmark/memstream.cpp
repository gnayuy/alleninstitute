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

    //inFile.open(filename, ios::in | ios::out | ios::binary);

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

    cout << timer.elapsedMs() << "ms" <<endl;
    timer.update();

    //
    uint16 spp, bpp, orientation, photo, comp, planar_config;
    int StripsPerImage,LastStripSize;
    uint32 XSIZE;
    uint32 YSIZE;
    uint32 tiledepth, tilelength, tilewidth;

    //
    TIFF* mem_TIFF = TIFFStreamOpen("MemTIFF", &input_TIFF);

    //
    TIFFGetField(mem_TIFF, TIFFTAG_BITSPERSAMPLE, &bpp);
    TIFFGetField(mem_TIFF, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(mem_TIFF, TIFFTAG_PHOTOMETRIC, &photo);
    TIFFGetField(mem_TIFF, TIFFTAG_COMPRESSION, &comp);
    TIFFGetField(mem_TIFF, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(mem_TIFF, TIFFTAG_TILEDEPTH, &tiledepth);
    TIFFGetField(mem_TIFF, TIFFTAG_TILEWIDTH, &tilewidth);
    TIFFGetField(mem_TIFF, TIFFTAG_TILELENGTH, &tilelength);
    long tilesize = TIFFTileSize(mem_TIFF);

    cout<<"test ... ... "<<comp<<" "<<tiledepth<<" "<<tilelength<<" "<<tilewidth<<" "<<tilesize<<endl;


    TIFFGetField(mem_TIFF, TIFFTAG_IMAGEWIDTH, &XSIZE);
    TIFFGetField(mem_TIFF, TIFFTAG_IMAGELENGTH, &YSIZE);

    cout<<"size ... "<<YSIZE<<" "<<XSIZE<<endl;


    //
    TIFFClose(mem_TIFF);
    inFile.close();

    return 0;
}
