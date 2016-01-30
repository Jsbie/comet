#include "input.h"

#include <chrono>
#include <thread>

#include "camera.h"
#include "camera_file.h"
#include "camera_kinect2.h"

#include "log.h"

Input::Input() :
    m_camera(nullptr),
    m_localFrame(new InputData()),
    m_thread(nullptr),
    m_listener(nullptr)
{
}

Input::~Input() {
    stop();
    release();
    delete m_localFrame;
    delete m_listener;
}

void Input::stop() {
    Log::print("stop", "INPUT");
    m_isRunning = false;
    if (m_thread != nullptr) {
        if (m_thread->joinable()) {
            Log::print("join thread", "INPUT");
            m_thread->join();
        }
        delete m_thread;
        m_thread = nullptr;
    }
}

void Input::release() {
    Log::print("release", "INPUT");
    delete m_camera;
    m_camera = nullptr;
}

bool Input::initialize(CameraType type, const char* path) {
    Log::print("init", "INPUT");
    stop();
    release();
    if (type == CameraType::CAMERA_KINECT2) {
        m_camera = new CameraKinect2();
    } else {
        m_camera = new CameraFile();
    }

    // Initialize camera
    if (!m_camera->initialize(path)) {
        delete m_camera;
        m_camera = nullptr;
        return false;
    }

    return true;
}

bool Input::start() {
    Log::print("start", "INPUT");
    stop();
    if (m_camera == nullptr) {
        Log::print("Cannot start input", "INPUT");
        return false;
    }

    m_isRunning = true;
    m_isPaused = false;

    if (m_thread != nullptr) {
        delete m_thread;
    }
    m_thread = new std::thread(&Input::run, this);

    Log::print("Started input", "INPUT");
    return true;
}

void Input::run() {
    while(m_isRunning == true) {
        std::chrono::milliseconds time(30);
        std::this_thread::sleep_for(time);

        if (m_isPaused) {
            continue;
        }

        if (m_camera != nullptr) {
            // Get new frame
            if (m_camera->getNextFrame(m_localFrame)) {
                // Pass frame to client
                m_listener->sendInputData(&m_localFrame);
            }
        }
    }
}

CameraParameters Input::getCameraParameters() {
    if (m_camera != nullptr) {
        return m_camera->m_params;
    } else {
        return CameraParameters();
    }
}
