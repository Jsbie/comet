#include "frame_pack.h"
#include "input_data.h"

FramePack::FramePack()
{
    m_input = new InputData();
}

FramePack::~FramePack() {
    delete m_input;
}

void FramePack::copyTo(FramePack& newPack) {
    m_input->depth.copyTo(newPack.m_input->depth);
    m_input->ir.copyTo(newPack.m_input->ir);
    m_input->color.copyTo(newPack.m_input->color);
    m_input->colorReg.copyTo(newPack.m_input->colorReg);
}
