#include "sdk_dev.h"

#include <algorithm>

#include "frame_pack.h"
#include "input.h"

#include "log.h"

SDK_Dev::SDK_Dev() :
    m_io(new Input()),
    m_data(new FramePack()),
    m_newDataReady(false)
{
    Log::print("SDK()", "SDK");
}

SDK_Dev::~SDK_Dev(){
    Log::print("~SDK()", "SDK");
    stop();
    delete m_data;
    delete m_io;
}

bool SDK_Dev::initialize(int cameraType, const char* path) {
    Log::print("Init", "SDK");
    stop();
    m_io->registerListener(*this, &SDK_Dev::onNewInput);
    return m_io->initialize((CameraType)(cameraType), path);
}

bool SDK_Dev::start() {
    return m_io->start();
}

void SDK_Dev::stop() {
    m_io->stop();
    m_newDataReady = false;
}

void SDK_Dev::onNewInput(InputData** newData) {
    Log::print("SDK()", "New frame");
    std::swap(m_data->m_input, *newData);
    m_newDataReady = true;
}

void SDK_Dev::processNewFrame() {

}
