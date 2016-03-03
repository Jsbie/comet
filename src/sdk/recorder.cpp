#include "recorder.h"
#include "recording_types.h"

#include "log.h"
#include "input_data.h"
#include "frame_pack.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

Recorder::Recorder() :
    m_recordingChannels(RECORDING_NONE),
    m_enabled(false)
{
}

void Recorder::setEnabled(bool enabled) {
    if (m_enabled == false && enabled == true) {
        std::stringstream msg;
        msg << "Started recording to folder: " << m_path;
        Log::d(msg.str(), "REC");
        saveConfigFile();
        m_counter = 0;
    } else if (m_enabled == true && enabled == false) {
        Log::d("Stopped recording", "REC");
    }
    m_enabled = enabled;
}

void Recorder::processNewFrame(FramePack* frame) {
    if (!m_enabled || m_path == "") {
        return;
    }
    saveFramePack(frame);
}

void Recorder::saveConfigFile() {

}

void Recorder::saveFramePack(FramePack* frame) {

    if (m_recordingChannels & RECORDING_DEPTH) {
        saveImage(&frame->m_input->depth, "depth");
    }

    if (m_recordingChannels & RECORDING_IR) {
        saveImage(&frame->m_input->ir, "ir");
    }

    if (m_recordingChannels & RECORDING_COLOR) {
        saveImage(&frame->m_input->color, "color");
    }

    m_counter++;
}

void Recorder::saveImage(Image* img, const char* typeName) {
    std::stringstream path;
    path << m_path << "/" << typeName << "_(" << m_counter << ").png";
    Image& in = *img;
    if (in.rows > 0 && in.cols > 0 && in.data != nullptr) {
        int matType;
        switch(in.bytesPerPixel) {
            case 1:
                matType = CV_8UC1;
                break;
            case 2:
                matType = CV_16UC1;
                break;
            case 3:
                matType = CV_8UC3;
                break;
            default:
                std::stringstream err;
                err << "Incorrect image type: " << in.bytesPerPixel;
                Log::e(err.str(), "REC");
                return;
        }
        cv::Mat out = cv::Mat(in.rows, in.cols, matType, in.data);
        cv::imwrite(path.str(), out);
    } else {
        std::stringstream err;
        err << "Empty " << typeName;
        Log::w(err.str(), "REC");
    }
}
