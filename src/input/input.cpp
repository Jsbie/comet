#include "input.h"

#include <chrono>
#include <thread>

#include "camera.h"
#include "camera_file.h"
#include "camera_kinect2.h"

#include "log.h"
#include "vld.h"

Input::Input() :
    m_activeChannels(CHANNEL_NONE),
    m_isRunning(false),
    m_isPaused(false),
    m_listener(nullptr),
    m_camera(nullptr),
    m_localFrame(new cm::InputData()),
    m_thread(nullptr)
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
    Log::d("stop", m_moduleTag);
    m_isRunning = false;
    if (m_thread != nullptr) {
        if (m_thread->joinable()) {
            Log::d("join thread", m_moduleTag);
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
    Log::d("release", m_moduleTag);
    delete m_camera;
    m_camera = nullptr;
}

bool Input::initialize(CameraType type, const char* path) {
    stop();
    release();
    Log::d("init", m_moduleTag);
    if (type == CameraType::CAMERA_KINECT2) {
        m_camera = new CameraKinect2();
    } else {
        m_camera = new CameraFile();
    }

    // Set up channels
    m_camera->m_enabledChannels = m_activeChannels;

    // Initialize camera
    if (!m_camera->initialize(path)) {
        Log::d("could not init. Relase", m_moduleTag);
        delete m_camera;
        m_camera = nullptr;
        return false;
    }

    return true;
}

bool Input::start() {
    Log::d("start", m_moduleTag);
    stop();
    if (m_camera == nullptr) {
        Log::d("Cannot start input", m_moduleTag);
        return false;
    }

    m_isRunning = true;
    m_isPaused = false;

    if (m_thread != nullptr) {
        delete m_thread;
    }
    m_thread = new std::thread(&Input::run, this);

    Log::d("Started input", m_moduleTag);
    return true;
}

void Input::setLogWriter(LogWriter* writer) {
    Log::setWriter(writer);
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
