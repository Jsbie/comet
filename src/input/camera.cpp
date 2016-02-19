#include "camera.h"

Camera::Camera() :
    m_depthAvailable(false),
    m_irAvailable(false),
    m_colorAvailable(false),
    m_colorRegAvailable(false),
    m_bodySkeletonAvailable(false),
    m_bodyMasksAvailable(false),
    m_depthEnabled(false),
    m_irEnabled(false),
    m_colorEnabled(false),
    m_colorRegEnabled(false),
    m_bodySkeletonEnabled(false),
    m_bodyMasksEnabled(false),
    m_framesCounter(0)
{
}

Camera::~Camera() {

}
