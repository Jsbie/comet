#ifndef CAMERA_H
#define CAMERA_H

#include "input_data.h"
#include "camera_types.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();

    virtual bool initialize(char* path = nullptr) = 0;
    virtual bool getNextFrame(InputData* input) = 0;

    CameraParameters    m_params;
    bool    m_depthAvailable;
    bool    m_irAvailable;
    bool    m_colorAvailable;
    bool    m_colorRegAvailable;
    bool    m_bodySkeletonAvailable;
    bool    m_bodyMasksAvailable;

    bool    m_depthEnabled;
    bool    m_irEnabled;
    bool    m_colorEnabled;
    bool    m_colorRegEnabled;
    bool    m_bodySkeletonEnabled;
    bool    m_bodyMasksEnabled;

    int     m_framesCounter;

private:
};

#endif // CAMERA_H
