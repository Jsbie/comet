#ifndef INPUT_H
#define INPUT_H

#include "platforms.h"
#include "camera_types.h"
#include "input_data.h"
#include "new_input_event.h"
#include "camera_types.h"

// Forward declarations
namespace std {
    class thread;
}
class Camera;
class LogWriter;

class DECLSPEC Input {

public:
    Input();
    ~Input();

    bool initialize(CameraType type, const char* path = nullptr);
    void release();

    bool start();
    void stop();
    void setLogWriter(LogWriter* writer);

    CameraParameters getCameraParameters();

    template<class Object, class Callback>
    void registerListener(Object& object, Callback callback) {
        delete m_listener;
        m_listener = new NewInputEvent<Object, Callback>(&object, callback);
    }

    int     m_activeChannels;
    const char* m_moduleTag = "INP";

private:
    bool    m_isRunning;
    bool    m_isPaused;
    //bool    m_playForward;
    //bool    m_continousPlayback;

    NewInputEventBase*  m_listener;     ///< Listener that will receive new input events.

    Camera*     m_camera;
    InputData*  m_localFrame;

    std::thread* m_thread;

    void run();
};

#endif // INPUT_H
