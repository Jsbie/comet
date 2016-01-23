#include "camera_file.h"

#include "json/json.h"
#include <opencv2/highgui.hpp>

CameraFile::CameraFile()
{

}

CameraFile::~CameraFile() {

}

bool CameraFile::initialize(char *path) {
    m_framesCounter = 0;
    // Check path


    // Set supported channels
    m_depthAvailable        = true;
    m_irAvailable           = true;
    m_colorAvailable        = true;
    m_colorRegAvailable     = true;
    m_bodyMasksAvailable    = true;
    m_bodySkeletonAvailable = true;

    // Set camera parameters
    m_params.fps            = 30;
    m_params.range          = 4500;
    m_params.focalLength    = 368.f;
    m_params.depthResW      = 512;
    m_params.depthResH      = 424;
    m_params.colorResW      = 1920;
    m_params.colorResH      = 1080;

    return true;
}

bool CameraFile::getNextFrame(InputData *input) {
    if (m_framesCounter == 0) {
        cv::Mat tmp = cv::imread("..\\..\\src\\studio\\icons\\logo.png");
        input->color.updateSize(tmp.rows, tmp.cols, 3);
        input->color.copyData(tmp.data);
        m_framesCounter++;
        return true;
    } else {
        return false;
    }
}
