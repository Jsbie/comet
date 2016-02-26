#include "platforms.h"

#include <mutex>

class Image;
class Input;
class InputData;
class FramePack;

class DECLSPEC SDK_Dev {

public:
    SDK_Dev();
    ~SDK_Dev();

    bool initialize(int cameraType, const char* path = nullptr);

    bool start();
    void stop();

    void onNewInput(InputData** newData);

    void processNewFrame();

    void lock();
    void unlock();

    // Getters / setters
    bool getColorDepth(Image& out);
    bool getColorIr(Image& out);

    Input*      m_io;
    FramePack*  m_data;
    bool m_newDataReady;

    std::mutex m_mutex;
};
