#include "platforms.h"

#include <mutex>

class Input;
namespace cm {
    class Image;
    class InputData;
}

class FramePack;
class Recorder;
class LogWriter;

class DECLSPEC SDK_Dev {

public:    
    SDK_Dev();
    ~SDK_Dev();

    bool initialize(int cameraType, const char* path = nullptr);

    bool start();
    void stop();

    void onNewInput(cm::InputData** newData);

    void processNewFrame();

    // Recording
    void setRecordingChannels(int channels);
    void setRecordingPath(const char* path);
    void setRecording(bool enabled);
    bool getRecording();

    // TODO: remove
    void lock();
    void unlock();

    // Getters / setters
    bool getColorDepth(cm::Image& out);
    bool getColorIr(cm::Image& out);
    bool getColor(cm::Image& out);
    bool getColorReg(cm::Image& out);

    Input*      m_io;
    FramePack*  m_data;
    bool m_newDataReady;

    LogWriter*  m_logWriter;
private:
    std::mutex  m_mutex;
    Recorder*   m_recorder;
};
