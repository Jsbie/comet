#include "platforms.h"

class Input;
class InputData;
class FramePack;

class DECLSPEC SDK_Dev {

public:
    SDK_Dev();
    ~SDK_Dev();

    bool initialize(int cameraType, const char* path = nullptr);

    void onNewInput(InputData** newData);

    void processNewFrame();

    Input*      m_io;
    FramePack*  m_data;
};
