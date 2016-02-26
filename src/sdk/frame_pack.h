#ifndef FRAMEPACK_H
#define FRAMEPACK_H

#include "platforms.h"

class InputData;
//class Image;

class DECLSPEC FramePack
{
public:
    FramePack();
    ~FramePack();

    InputData*  m_input;

    // TODO:
    // Debug data
    // Processing data
    // Users data
    // - Gesture data
    // - Hands
    // -- Hand skeleton
    // - Body skeleton
};

#endif // FRAMEPACK_H
