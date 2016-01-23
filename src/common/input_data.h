#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "platforms.h"

class DECLSPEC Image {
public:
    Image();
    Image(int newRows, int newCols, int newBytesPerPixel);
    ~Image();

    void updateSize(int newRows, int newCols, int newBytesPerPixel);
    void swapData(unsigned char** newData);
    void copyData(const unsigned char* newData);

    int cols;
    int rows;
    int bytesPerPixel;
    unsigned char* data;
};

class DECLSPEC InputData
{
public:
    InputData();
    ~InputData();

    Image depth;
    Image ir;
    Image color;

};

#endif // INPUTDATA_H
