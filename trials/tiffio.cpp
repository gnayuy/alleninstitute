// pgc++ -I/home/yangy/work/tiffio/include -acc -ta=nvidia -Minfo=accel -o tiffio tiffio.cpp -lm -L/home/yangy/work/tiffio/lib -ltiff -ltiffxx
// export LD_LIBRARY_PATH=/home/yangy/work/tiffio/lib:$LD_LIBRARY_PATH

/*
PGCC-S-0155-Procedures called in a compute region must have acc routine information: TIFFReadEncodedStrip (tiffio.cpp: 139)
PGCC-S-0155-Accelerator region ignored; see -Minfo messages  (tiffio.cpp: 131)
main:
    131, Generating copy(img[:][:])
         Accelerator region ignored
    137, Accelerator restriction: loop contains induction variable live after the loop
    139, Accelerator restriction: call to 'TIFFReadEncodedStrip' with no acc routine information
         Accelerator restriction: induction variable live-out from loop: i
    140, Accelerator restriction: induction variable live-out from loop: i

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <set>
#include <time.h>
#include <errno.h>
#include <queue>
#include <dirent.h>
#include <cstring>
#include <math.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <omp.h>
#include <openacc.h>

#include "tiffio.h"
#include "tiffio.hxx"

#ifdef WIN32
double PCFreq = 0.0;
__int64 timerStart = 0;
#else
struct timeval timerStart;
#endif

using namespace std;

//
void StartTimer()
{
#ifdef WIN32
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency failed!\n");

    PCFreq = (double)li.QuadPart/1000.0;

    QueryPerformanceCounter(&li);
    timerStart = li.QuadPart;
#else
    gettimeofday(&timerStart, NULL);
#endif
}

// time elapsed in ms
double GetTimer()
{
#ifdef WIN32
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart-timerStart)/PCFreq;
#else
    struct timeval timerStop, timerElapsed;
    gettimeofday(&timerStop, NULL);
    timersub(&timerStop, &timerStart, &timerElapsed);
    return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
#endif
}

//
int main(int argc, char* argv[])
{
	unsigned int w, h, rps, nStrips, lastStripSize;
	unsigned short bpp;

	ifstream inFile;
        inFile.open(argv[1]);

	stringstream* tifbin = new stringstream;
	*tifbin << inFile.rdbuf();

	inFile.close();



	//
	TIFF* input = TIFFStreamOpen("MemTIFF", (istream *)tifbin);

	
	TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(input, TIFFTAG_IMAGELENGTH, &h);

	TIFFGetField(input, TIFFTAG_BITSPERSAMPLE, &bpp);

	TIFFGetField(input, TIFFTAG_ROWSPERSTRIP, &rps);

	cout<<"w "<<w<<" h "<<h<<" bpp "<<bpp<<" rps "<<rps<<endl;

	//unsigned char * img = NULL;
	long imgSize = long(w)*long(h)*long(bpp/8); 

	unsigned short img[54600][34412];

	try
	{
		//img = new unsigned char [imgSize];
		memset(img, 0, imgSize);
	}
	catch(...)
	{
		cout<<"fail in allocation"<<endl;
		return -1;
	}

#if _OPENACC
    acc_init(acc_device_nvidia);
#endif


	nStrips = (h+rps-1) / rps;
	lastStripSize = h % rps;
	if(lastStripSize==0) lastStripSize=rps;

	cout<<"start reading ... "<<endl;
	StartTimer();

	unsigned int offset = rps * w * (bpp/8);
	//unsigned char* buf = (unsigned char *)img;

#pragma acc data copy(img)
#pragma omp parallel for shared(nStrips, offset, input, img)
#pragma acc kernels
	for(long i=0; i<nStrips; i++)
	{
		TIFFReadEncodedStrip(input, i, (unsigned char*)(img[i][0]), offset);
	}

	double runtime = GetTimer();
    	printf(" total: %f s\n", runtime / 1000.f);
	
	//TIFFReadEncodedStrip(input, nStrips-1, buf, lastStripSize * w * (bpp/8));
	
	// write for test
	ofstream binFile("testImage.raw", ios::out | ios::binary);
	binFile.write((char*)img, imgSize);
	binFile.close();

	//
	TIFFClose(input);

	//
	if(img)
	{
		delete []img;
	}

	tifbin->clear();
	tifbin->str(string());
	delete tifbin;

	//
	return 0;
}
