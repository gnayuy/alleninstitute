// g++ -o copyservice copyservice.cpp

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

//
class ImageFile
{
public:
    ImageFile()
    {
        visited = false;
    }

    ~ImageFile()
    {

    }

public:
    string filename;
    bool visited;
};


int main(int argc, char * argv[])
{
    if(argc<3)
    {
        cout<<"copyservice images.txt processing.txt";
        return -1;
    }

    //
    string srcdir = "/local3/images/";
    string dstdir = "/local3/m17545/";

    //
    int NImages = 16;

    //
    vector<ImageFile> images;


    // load ori images
    ifstream input(argv[1]);

    string line;
    if(input.is_open())
    {
        while(getline(input, line))
        {
            ImageFile fn;
            fn.filename = line;
            images.push_back(fn);
        }
        input.close();
    }

//    for(long i=0; i<images.size(); i++)
//    {
//        cout<<" image "<<i<<" "<<images[i].filename<<" "<<images[i].visited<<endl;
//    }

    // check converting output
    int n=0, pre=-1;
    while(true)
    {
        ifstream proc(argv[2]);

        if(proc.is_open())
        {
            while(getline(proc, line))
            {
                if (line.find("_mon.tif") != std::string::npos)
                {
                    int pos = line.find("test");
                    string filename = line.substr(pos);

                    //cout<<filename<<endl;

                    // delete
                    for(long i=0; i<images.size(); i++)
                    {
                        if(images[i].filename.find(filename) != std::string::npos)
                        {
                            if(images[i].visited == false)
                            {
                                images[i].visited = true;
                                n++;

                                string dstfile = dstdir+filename;
                                cout<<"delete "<<dstfile<<endl;

                                if( remove(dstfile.c_str()) != 0)
                                {
                                    cout<<"Error deleting file"<<endl;
                                }
                            }
                        }
                    }
                }
            } // while
            proc.close();
        }

        //
        if(n>pre && n%16==0)
        {
            // copy
            cout<<"need to copy next 16 images"<<endl;

            pre = n;

            for(long i=0; i<NImages; i++)
            {
                string srcfile = srcdir + images[n+i].filename;
                string dstfile = dstdir + images[n+i].filename;

                cout<<"copy "<<n+i<<" "<<srcfile<<" to "<<dstfile<<endl;

//                ifstream src(srcfile.c_str(), std::ios::binary);
//                ofstream dst(dstfile.c_str(), std::ios::binary);

//                dst << src.rdbuf();

//                src.close();
//                dst.close();


                string command = "cp -L " + srcfile + " " + dstfile;

                //cout<<command<<endl;

                system(command.c_str());
            }
        }
    }

    return 0;
}
