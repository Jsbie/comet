#ifndef FRAMEPACK_H
#define FRAMEPACK_H

#include "platforms.h"

namespace cm {
class InputData;
}

class DECLSPEC FramePack
{
public:
    FramePack();
    ~FramePack();

    cm::InputData*  m_input;

    // TODO:
    // Debug data
    // Processing data
    // Users data
    // - Gesture data
    // - Hands
    // -- Hand skeleton
    // - Body skeleton

    void copyTo(FramePack&);
};

#endif // FRAMEPACK_H
