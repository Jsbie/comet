#ifndef CAMERAFILE_H
#define CAMERAFILE_H

#include "camera.h"

class CameraFile : public Camera
{
public:
    CameraFile();
    ~CameraFile();

    bool initialize(const char* path = nullptr);
    bool getNextFrame(cm::InputData *input);
};

#endif // CAMERAFILE_H
