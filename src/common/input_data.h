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
    void copyTo(Image& img);
    void clear();
    bool empty();

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

    Image depth;        ///< Depth image
    Image ir;           ///< Infrared image
    Image color;        ///< Color image
    Image colorReg;     ///< Color image mapped to depth

};

#endif // INPUTDATA_H
