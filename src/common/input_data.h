#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "platforms.h"

namespace cm {

enum JointType {
    JOINT_SPINEBASE     = 0,
    JOINT_SPINEMID      = 1,
    JOINT_NECK          = 2,
    JOINT_HEAD          = 3,
    JOINT_SHOULDERLEFT	= 4,
    JOINT_ELBOWLEFT     = 5,
    JOINT_WRISTLEFT     = 6,
    JOINT_HANDLEFT      = 7,
    JOINT_SHOULDERRIGHT	= 8,
    JOINT_ELBOWRIGHT	= 9,
    JOINT_WRISTRIGHT	= 10,
    JOINT_HANDRIGHT     = 11,
    JOINT_HIPLEFT       = 12,
    JOINT_KNEELEFT      = 13,
    JOINT_ANKLELEFT     = 14,
    JOINT_FOOTLEFT      = 15,
    JOINT_HIPRIGHT      = 16,
    JOINT_KNEERIGHT     = 17,
    JOINT_ANKLERIGHT	= 18,
    JOINT_FOOTRIGHT     = 19,
    JOINT_SPINESHOULDER	= 20,
    JOINT_HANDTIPLEFT	= 21,
    JOINT_THUMBLEFT     = 22,
    JOINT_HANDTIPRIGHT	= 23,
    JOINT_THUMBRIGHT	= 24,
    JOINT_COUNT         = 25
};

class DECLSPEC Point2i {
public:
    int x,y;
};

class DECLSPEC Point3f {
public:
    float x,y,z;
};

class DECLSPEC Joint {
public:
    cm::Point2i pos2d;
    cm::Point3f pos3d;
};

//class DECLSPEC BodySkeleton {
//public:

//};

class DECLSPEC InputUser {
public:
    int     m_id;
    bool    m_active;
    cm::Joint bodyJoints[JOINT_COUNT];
};


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

    InputUser users[6]; ///< Array of users

};

}

#endif // INPUTDATA_H
