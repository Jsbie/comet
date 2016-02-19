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
    m_isRunning(false),
    m_isPaused(false),
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
    if (!m_isRunning) {
        return;
    }
    Log::d("stop", "INPUT");
    m_isRunning = false;
    if (m_thread != nullptr) {
        if (m_thread->joinable()) {
            Log::d("join thread", "INPUT");
            m_thread->join();
        }
        delete m_thread;
        m_thread = nullptr;
    }
}

void Input::release() {
    if (m_camera == nullptr) {
        return;
    }
    Log::d("release", "INPUT");
    delete m_camera;
    m_camera = nullptr;
}

bool Input::initialize(CameraType type, const char* path) {
    stop();
    release();
    Log::d("init", "INPUT");
    if (type == CameraType::CAMERA_KINECT2) {
        m_camera = new CameraKinect2();
    } else {
        m_camera = new CameraFile();
    }

    // Set up channels
    m_camera->m_depthEnabled = true;
    m_camera->m_irEnabled = true;
    m_camera->m_colorEnabled = true;

    // Initialize camera
    if (!m_camera->initialize(path)) {
        Log::d("could not init. Relase", "INPUT");
        delete m_camera;
        m_camera = nullptr;
        return false;
    }

    return true;
}

bool Input::start() {
    Log::d("start", "INPUT");
    stop();
    if (m_camera == nullptr) {
        Log::d("Cannot start input", "INPUT");
        return false;
    }

    m_isRunning = true;
    m_isPaused = false;

    if (m_thread != nullptr) {
        delete m_thread;
    }
    m_thread = new std::thread(&Input::run, this);

    Log::d("Started input", "INPUT");
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
