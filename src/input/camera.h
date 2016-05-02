#ifndef CAMERA_H
#define CAMERA_H

#include "input_data.h"
#include "camera_types.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();

    virtual bool initialize() = 0;
    virtual bool getNextFrame(cm::InputData* input) = 0;

    CameraParameters    m_params;
    bool    m_depthAvailable;
    bool    m_irAvailable;
    bool    m_colorAvailable;
    bool    m_colorRegAvailable;
    bool    m_bodySkeletonAvailable;
    bool    m_bodyMasksAvailable;

    int     m_enabledChannels;
    int     m_framesCounter;

private:
};

#endif // CAMERA_H
