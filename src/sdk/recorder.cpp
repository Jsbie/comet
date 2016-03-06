#include "recorder.h"
#include "recording_types.h"

#include "log.h"
#include "input_data.h"
#include "frame_pack.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

Recorder::Recorder() :
    m_recordingChannels(RECORDING_NONE),
    m_enabled(false),
    m_threadActive(false)
{
}

Recorder::~Recorder() {
    setEnabled(false);
}

void Recorder::setEnabled(bool enabled) {
    bool prevEnabled = m_enabled;
    m_enabled = enabled;
    if (prevEnabled == false && m_enabled == true) {
        std::stringstream msg;
        msg << "Started recording to folder: " << m_path;
        Log::d(msg.str().c_str(), m_moduleTag);
        saveConfigFile();
        m_counter = 0;
        // Add log exception to always print recording messages no matter what log level is set
        Log::addException(m_moduleTag);
    } else if (prevEnabled == true && m_enabled == false) {
        Log::d("Stopped recording", m_moduleTag);
        // Wait for the recording thread to finish
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
}

void Recorder::processNewFrame(FramePack* frame) {
    if (!m_enabled) {
        return;
    }
    if (m_path == "") {
        Log::e("Path is empty.", m_moduleTag);
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.size() >= MAX_QUEUE_LENGTH) {
        Log::d("Queue reached maximum length.", m_moduleTag);
        return;
    }
    FramePack* copy = new FramePack();
    frame->copyTo(*copy);
    m_queue.push(copy);

    std::stringstream s;
    s << "Added to queue. Queue length: " << m_queue.size();
    Log::t(s.str().c_str(), m_moduleTag);

    if (!m_threadActive) {
        if (m_thread.joinable()) {
            m_thread.join();
        }
        m_threadActive = true;
        m_thread = std::thread(&Recorder::run, this);
    }
}

void Recorder::saveConfigFile() {

}

void Recorder::saveFramePack(FramePack* frame) {

    if (frame == nullptr)
        return;

    if (m_recordingChannels & RECORDING_DEPTH) {
        saveImage(&frame->m_input->depth, "depth");
    }

    if (m_recordingChannels & RECORDING_IR) {
        saveImage(&frame->m_input->ir, "ir");
    }

    if (m_recordingChannels & RECORDING_COLOR) {
        saveImage(&frame->m_input->color, "color");
    }

    if (m_recordingChannels & RECORDING_COLOR_REG) {
        saveImage(&frame->m_input->colorReg, "colorReg");
    }

    m_counter++;
}

void Recorder::saveImage(Image* img, const char* typeName) {
    if (img == nullptr) {
        Log::e("empty", "REC");
        return;
    }
    std::stringstream path;
    path << m_path << "/" << typeName << "_(" << m_counter << ").png";
    Image& in = *img;
    if (!in.empty()) {
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
                Log::e(err.str().c_str(), m_moduleTag);
                return;
        }
        cv::Mat out = cv::Mat(in.rows, in.cols, matType, in.data);
        cv::imwrite(path.str(), out);
    } else {
        std::stringstream err;
        err << "Empty " << typeName;
        Log::w(err.str().c_str(), m_moduleTag);
    }
}

void Recorder::run() {
    while (true) {
        std::chrono::milliseconds time(5);
        std::this_thread::sleep_for(time);

        FramePack* tmp = nullptr;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_queue.empty()) {
                m_threadActive = false;
                return;
            }
            tmp = m_queue.front();
            m_queue.pop();

            std::stringstream s;
            s << "Written frame " << m_counter << ". Remaining " << m_queue.size();
            Log::t(s.str().c_str(), m_moduleTag);
        }
        saveFramePack(tmp);
        delete tmp;
    }
}
