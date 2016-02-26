#include "platforms.h"

#include <mutex>

class Image;
class Input;
class InputData;
class FramePack;
class Recorder;

class DECLSPEC SDK_Dev {

public:
    SDK_Dev();
    ~SDK_Dev();

    bool initialize(int cameraType, const char* path = nullptr);

    bool start();
    void stop();

    void onNewInput(InputData** newData);

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
    bool getColorDepth(Image& out);
    bool getColorIr(Image& out);

    Input*      m_io;
    FramePack*  m_data;
    bool m_newDataReady;

private:
    std::mutex  m_mutex;
    Recorder*   m_recorder;
};
