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
        std::stringstream path;
        path << m_path << "/" << "depth_(" << m_counter << ").png";
        Image& in = frame->m_input->depth;
        if (in.rows > 0 && in.cols > 0 && in.data != nullptr) {
            cv::Mat out = cv::Mat(in.rows, in.cols, CV_16UC1, in.data);
            cv::imwrite(path.str(), out);
        } else {
            Log::w("Depth empty", "REC");
        }
    }

    if (m_recordingChannels & RECORDING_IR) {
        std::stringstream path;
        path << m_path << "/" << "ir_(" << m_counter << ").png";
        Image& in = frame->m_input->ir;
        if (in.rows > 0 && in.cols > 0 && in.data != nullptr) {
            cv::Mat out = cv::Mat(in.rows, in.cols, CV_16UC1, in.data);
            cv::imwrite(path.str(), out);
        } else {
            Log::w("Ir empty", "REC");
        }
    }

    if (m_recordingChannels & RECORDING_COLOR) {
        std::stringstream path;
        path << m_path << "/" << "color_(" << m_counter << ").png";
        Image& in = frame->m_input->color;
        if (in.rows > 0 && in.cols > 0 && in.data != nullptr) {
            cv::Mat out = cv::Mat(in.rows, in.cols, CV_8UC3, in.data);
            cv::imwrite(path.str(), out);
        } else {
            Log::w("Color empty", "REC");
        }
    }

    m_counter++;
}
