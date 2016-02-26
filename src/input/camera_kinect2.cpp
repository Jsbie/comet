#include "camera_kinect2.h"

#include "log.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

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
    SafeRelease( m_coordMapper );

    if( m_sensor ){
        m_sensor->Close();
    }
    SafeRelease( m_sensor );
}

bool CameraKinect2::initialize(const char* path) {
    Log::d("init", "KIN2");
    HRESULT hResult = S_OK;
    hResult = GetDefaultKinectSensor( &m_sensor );
    if( FAILED( hResult ) ){
        Log::e("Could not find the sensor", "KIN2");
        return false;
    }

    hResult = m_sensor->Open();
    if( FAILED( hResult ) ){
        Log::e("Could not open the sensor", "KIN2");
        return false;
    }

    m_sensor->get_CoordinateMapper(&m_coordMapper);

    DWORD channels = 0;

    if (m_enabledChannels & CHANNEL_DEPTH) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Depth;
    }

    if (m_enabledChannels & CHANNEL_IR) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Infrared;
    }

    if (m_enabledChannels & CHANNEL_COLOR) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Color;
    }

    if (m_enabledChannels & CHANNEL_BODY_SKELETON) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_Body;
    }

    if (m_enabledChannels & CHANNEL_BODY_MASK) {
        channels = channels | FrameSourceTypes::FrameSourceTypes_BodyIndex;
    }

    // Multi source reader
    hResult = m_sensor->OpenMultiSourceFrameReader(channels, &m_sourceReader);
    if( FAILED( hResult ) ){
        Log::e("Could not open the frame reader", "KIN2");
        return false;
    }

    Log::d("init ok", "KIN2");
    return true;
}

bool CameraKinect2::getNextFrame(InputData *input) {

//    IBodyFrame*         skel    = nullptr;
//    IBodyIndexFrame*    masks   = nullptr;

    HRESULT hResult = S_OK;
    IMultiSourceFrame* frame = nullptr;
    hResult = m_sourceReader->AcquireLatestFrame( &frame );
    if( FAILED( hResult ) ){
        Log::e("Could not get frame", "KIN2");
        return false;
    } else if (frame != nullptr) {
        IFrameDescription* desc;
        int width, height;
        UINT size;

        if (m_enabledChannels & CHANNEL_DEPTH) {
            IDepthFrame* depth   = nullptr;
            IDepthFrameReference* depthRef;
            hResult = frame->get_DepthFrameReference( &depthRef );
            if ( SUCCEEDED( hResult ) ) {
                depthRef->AcquireFrame( &depth );
            }
            if (depth != nullptr) {
                depth->get_FrameDescription(&desc);
                desc->get_Height(&height);
                desc->get_Width(&width);

                input->depth.updateSize(height, width, 2);
                UINT16* data;
                depth->AccessUnderlyingBuffer(&size, &data);
                input->depth.copyData((unsigned char*)data);
            }
            SafeRelease(depthRef);
            SafeRelease(depth);
        }

        if (m_enabledChannels & CHANNEL_IR) {
            IInfraredFrame* ir = nullptr;
            IInfraredFrameReference* irRef;
            hResult = frame->get_InfraredFrameReference( &irRef );
            if ( SUCCEEDED( hResult ) ) {
                irRef->AcquireFrame( &ir );
            }
            if (ir != nullptr) {
                ir->get_FrameDescription(&desc);
                desc->get_Height(&height);
                desc->get_Width(&width);

                input->ir.updateSize(height, width, 2);
                UINT16* data;
                ir->AccessUnderlyingBuffer(&size, &data);
                input->ir.copyData((unsigned char*)data);
            }
            SafeRelease(irRef);
            SafeRelease(ir);
        }

        if (m_enabledChannels & CHANNEL_COLOR) {
            IColorFrame* color = nullptr;
            IColorFrameReference* colorRef;
            hResult = frame->get_ColorFrameReference( &colorRef );
            if ( SUCCEEDED( hResult ) ) {
                colorRef->AcquireFrame( &color );
            }
            if (color != nullptr) {
                color->get_FrameDescription(&desc);
                desc->get_Height(&height);
                desc->get_Width(&width);

                input->color.updateSize(height, width, 3);
                BYTE* data;
                hResult = color->AccessRawUnderlyingBuffer(&size, &data);
                if ( SUCCEEDED( hResult ) ) {
                    cv::Mat in = cv::Mat(height, width, CV_8UC2, data);
                    cv::Mat out = cv::Mat(height, width, CV_8UC3, input->color.data);
                    cv::cvtColor(in, out, CV_YUV2RGB_YUYV);
                }
            }
            SafeRelease(colorRef);
            SafeRelease(color);
        }

        SafeRelease(desc);
    }

    Log::d("got next frame", "KIN2");

    return true;
//    SafeRelease( skel );
//    SafeRelease( masks );
}
