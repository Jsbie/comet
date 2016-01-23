#include "frame_pack.h"
#include "input_data.h"

FramePack::FramePack()
{
    m_input = new InputData();
}

FramePack::~FramePack() {
    delete m_input;
}
