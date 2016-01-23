#ifndef FRAMEPACK_H
#define FRAMEPACK_H

class InputData;

class FramePack
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
