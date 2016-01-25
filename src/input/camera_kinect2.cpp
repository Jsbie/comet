#include "camera_kinect2.h"

#include "log.h"

CameraKinect2::CameraKinect2()
{
    // Set supported channels
    m_depthAvailable        = true;
    m_irAvailable           = true;
    m_colorAvailable        = true;
    m_colorRegAvailable     = true;
    m_bodyMasksAvailable    = true;
    m_bodySkeletonAvailable = true;

    // Set camera parameters
    m_params.fps            = 30;
    m_params.range          = 4500;
    m_params.focalLength    = 368.f;
    m_params.depthResW      = 512;
    m_params.depthResH      = 424;
    m_params.colorResW      = 1920;
    m_params.colorResH      = 1080;
}

CameraKinect2::~CameraKinect2() {
    SafeRelease( m_sourceReader );
    //SafeRelease( pDescription );
    if( m_sensor ){
        m_sensor->Close();
    }
    SafeRelease( m_sensor );
}

bool CameraKinect2::initialize(const char* path) {
    HRESULT hResult = S_OK;
    hResult = GetDefaultKinectSensor( &m_sensor );
    if( FAILED( hResult ) ){
        return false;
    }

    hResult = m_sensor->Open();
    if( FAILED( hResult ) ){
        return false;
    }

    DWORD channels;

    if (m_depthEnabled) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Depth;
    }

    if (m_irEnabled) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Infrared;
    }

    if (m_colorEnabled) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Color;
    }

    if (m_bodySkeletonEnabled) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Body;
    }

    if (m_bodyMasksEnabled) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_BodyIndex;
    }

    // Multi source reader
    hResult = m_sensor->OpenMultiSourceFrameReader(channels, &m_sourceReader);
    if( FAILED( hResult ) ){
        return false;
    }

    return true;
}

bool CameraKinect2::getNextFrame(InputData *input) {

//    IBodyFrame*         skel    = nullptr;
//    IBodyIndexFrame*    masks   = nullptr;

    HRESULT hResult = S_OK;
    IMultiSourceFrame* frame = nullptr;
    hResult = m_sourceReader->AcquireLatestFrame( &frame );
    if( FAILED( hResult ) ){
        return false;
    } else if (frame != nullptr) {
        IFrameDescription* desc;
        int width, height;
        UINT size;

        if (m_depthEnabled) {
            IDepthFrame* depth   = nullptr;
            IDepthFrameReference* depthRef;
            hResult = frame->get_DepthFrameReference( &depthRef );
            if ( SUCCEEDED( hResult ) ) {
                depthRef->AcquireFrame( &depth );
            }
            depth->get_FrameDescription(&desc);
            desc->get_Height(&height);
            desc->get_Width(&width);

            input->depth.updateSize(height, width, 2);
            UINT16* data;
            depth->AccessUnderlyingBuffer(&size, &data);
            input->depth.copyData((unsigned char*)data);

            SafeRelease(depthRef);
            SafeRelease(depth);
        }

        if (m_irEnabled) {
            IInfraredFrame* ir = nullptr;
            IInfraredFrameReference* irRef;
            hResult = frame->get_InfraredFrameReference( &irRef );
            if ( SUCCEEDED( hResult ) ) {
                irRef->AcquireFrame( &ir );
            }
            ir->get_FrameDescription(&desc);
            desc->get_Height(&height);
            desc->get_Width(&width);

            input->ir.updateSize(height, width, 2);
            UINT16* data;
            ir->AccessUnderlyingBuffer(&size, &data);
            input->ir.copyData((unsigned char*)data);

            SafeRelease(irRef);
            SafeRelease(ir);
        }

        if (m_colorEnabled) {
            IColorFrame* color = nullptr;
            IColorFrameReference* colorRef;
            hResult = frame->get_ColorFrameReference( &colorRef );
            if ( SUCCEEDED( hResult ) ) {
                colorRef->AcquireFrame( &color );
            }
            color->get_FrameDescription(&desc);
            desc->get_Height(&height);
            desc->get_Width(&width);

            input->color.updateSize(height, width, 3);
            BYTE* data;
            color->AccessRawUnderlyingBuffer(&size, &data);
            input->color.copyData((unsigned char*)data);

            SafeRelease(colorRef);
            SafeRelease(color);
        }

        SafeRelease(desc);
    }

    return true;
//    SafeRelease( skel );
//    SafeRelease( masks );
}
