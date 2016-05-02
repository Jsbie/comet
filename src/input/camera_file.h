#ifndef CAMERAFILE_H
#define CAMERAFILE_H

#include "camera.h"
#include "string"

class CameraFile : public Camera
{
public:
    CameraFile();
    ~CameraFile();

    bool initialize();
    bool getNextFrame(cm::InputData *input);

    std::string path;
};

#endif // CAMERAFILE_H
