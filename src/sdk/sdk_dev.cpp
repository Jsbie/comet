#include "sdk_dev.h"

#include <algorithm>

#include "frame_pack.h"
#include "input.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
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
    m_io->m_activeChannels = CHANNEL_NONE;
    m_io->m_activeChannels = m_io->m_activeChannels | CHANNEL_DEPTH;
    m_io->m_activeChannels = m_io->m_activeChannels | CHANNEL_IR;
    m_io->m_activeChannels = m_io->m_activeChannels | CHANNEL_COLOR;
    bool result = m_io->initialize((CameraType)(cameraType), path);
    return result;
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

bool SDK_Dev::getColorDepth(Image& out) {
    Image& in = m_data->m_input->depth;
    if (in.data == nullptr) {
        return false;
    }

    out.updateSize(in.rows, in.cols, 3);
    cv::Mat beg = cv::Mat(in.rows, in.cols, CV_16UC1, in.data);
    cv::Mat dst = cv::Mat(out.rows, out.cols, CV_8UC3, out.data);
    cv::Mat tmp;
    beg.convertTo(tmp, CV_8UC1, 255.0/4500, 0);
    cv::applyColorMap(tmp, dst, cv::COLORMAP_JET);
    return true;
}

bool SDK_Dev::getColorIr(Image& out) {
    Image& in = m_data->m_input->ir;
    if (in.data == nullptr) {
        return false;
    }

    out.updateSize(in.rows, in.cols, 3);
    cv::Mat beg = cv::Mat(in.rows, in.cols, CV_16UC1, in.data);
    cv::Mat dst = cv::Mat(out.rows, out.cols, CV_8UC3, out.data);
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(beg, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::Mat tmp;
    beg.convertTo(tmp, CV_8UC1, 255.f / (maxVal - minVal), 255.f * minVal / (minVal - maxVal));
    cv::cvtColor(tmp, dst, CV_GRAY2RGB);
    return true;
}
