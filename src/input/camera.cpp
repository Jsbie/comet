#include "camera.h"

Camera::Camera() :
    m_depthAvailable(false),
    m_irAvailable(false),
    m_colorAvailable(false),
    m_colorRegAvailable(false),
    m_bodySkeletonAvailable(false),
    m_bodyMasksAvailable(false),
    m_enabledChannels(CHANNEL_NONE),
    m_framesCounter(0)
{
}

Camera::~Camera() {

}
