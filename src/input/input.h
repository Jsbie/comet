#ifndef INPUT_H
#define INPUT_H

#include "platforms.h"
#include "input_data.h"
#include "new_input_event.h"
#include "camera_types.h"

// Forward declarations
namespace std {
    class thread;
}
class Camera;

enum CameraType {
    CAMERA_KINECT2,
    CAMERA_FILE
};

class DECLSPEC Input {

public:
    Input();
    ~Input();

    bool initialize(CameraType type, const char* path = nullptr);

    void start();
    void stop();
    void run();

    CameraParameters getCameraParameters();

    template<class Object, class Callback>
    void registerListener(Object& object, Callback callback) {
        delete m_listener;
        m_listener = new NewInputEvent<Object, Callback>(&object, callback);
    }

private:
    bool    m_isRunning;
    bool    m_isPaused;
    //bool    m_playForward;
    //bool    m_continousPlayback;

    NewInputEventBase*  m_listener;     ///< Listener that will receive new input events.

    Camera*     m_camera;
    InputData*  m_localFrame;

    std::thread* m_thread;
};

#endif // INPUT_H
