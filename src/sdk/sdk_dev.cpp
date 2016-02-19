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
    Log::d("SDK()", "SDK");
}

SDK_Dev::~SDK_Dev(){
    Log::d("~SDK()", "SDK");
    stop();
    delete m_data;
    delete m_io;
}

bool SDK_Dev::initialize(int cameraType, const char* path) {
    Log::d("Init", "SDK");
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
    std::lock_guard<std::mutex> tmp(m_mutex);
    Log::d("SDK()", "New frame");
    std::swap(m_data->m_input, *newData);
    m_newDataReady = true;
}

void SDK_Dev::processNewFrame() {

}

void SDK_Dev::lock() {
    m_mutex.lock();
}

void SDK_Dev::unlock() {
    m_mutex.unlock();
}
