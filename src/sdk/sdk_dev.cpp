#include "sdk_dev.h"

#include <algorithm>

#include "frame_pack.h"
#include "input.h"

#include "log.h"

#include <opencv2/highgui.hpp>

SDK_Dev::SDK_Dev() :
    m_io(nullptr),
    m_data(new FramePack())
{
}

SDK_Dev::~SDK_Dev(){
    Log::print("~SDK()", "SDK");
    delete m_io;
    delete m_data;
}

bool SDK_Dev::initialize(int cameraType, const char* path) {
    Log::print("SDK()", "SDK");
    m_io = new Input();
    m_io->initialize(CameraType::CAMERA_FILE, path);
    m_io->registerListener(*this, &SDK_Dev::onNewInput);
    m_io->start();

    return true;
}

void SDK_Dev::onNewInput(InputData** newData) {
    std::swap(m_data->m_input, *newData);

    cv::Mat tmp = cv::Mat(m_data->m_input->color.rows, m_data->m_input->color.cols, CV_8UC3, m_data->m_input->color.data);
    cv::imshow("hello2", tmp);
    cv::waitKey(1);
}

void SDK_Dev::processNewFrame() {

}
