#include "camera_kinect2.h"

#include "log.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

CameraKinect2::CameraKinect2() :
    m_sensor(nullptr),
    m_coordMapper(nullptr),
    m_sourceReader(nullptr),
    m_colorPoints(nullptr)
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

    delete[] m_colorPoints;
}

bool CameraKinect2::initialize(const char* path) {
    Log::d("init", m_moduleTag);
    HRESULT hResult = S_OK;
    hResult = GetDefaultKinectSensor( &m_sensor );
    if( FAILED( hResult ) ){
        Log::e("Could not find the sensor", m_moduleTag);
        return false;
    }

    hResult = m_sensor->Open();
    if( FAILED( hResult ) ){
        Log::e("Could not open the sensor", m_moduleTag);
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
        Log::e("Could not open the frame reader", m_moduleTag);
        return false;
    }

    Log::d("init ok", m_moduleTag);
    return true;
}

bool CameraKinect2::getNextFrame(cm::InputData *input) {

//    IBodyFrame*         skel    = nullptr;
//    IBodyIndexFrame*    masks   = nullptr;

    HRESULT hResult = S_OK;
    IMultiSourceFrame* frame = NULL;
    hResult = m_sourceReader->AcquireLatestFrame( &frame );
    if( FAILED( hResult ) ){
        Log::t("Could not get frame", m_moduleTag);
        SafeRelease(frame);
        return false;
    } else if (frame != NULL) {
        IFrameDescription* desc;
        int width, height;
        UINT size;

        if (m_enabledChannels & CHANNEL_DEPTH) {
            IDepthFrame* depth   = NULL;
            IDepthFrameReference* depthRef = NULL;
            hResult = frame->get_DepthFrameReference( &depthRef );
            if ( SUCCEEDED( hResult ) ) {
                depthRef->AcquireFrame( &depth );
            }
            if (depth != NULL) {
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
            IInfraredFrame* ir = NULL;
            IInfraredFrameReference* irRef = NULL;
            hResult = frame->get_InfraredFrameReference( &irRef );
            if ( SUCCEEDED( hResult ) ) {
                irRef->AcquireFrame( &ir );
            }
            if (ir != NULL) {
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
            IColorFrame* color = NULL;
            IColorFrameReference* colorRef = NULL;
            hResult = frame->get_ColorFrameReference( &colorRef );
            if ( SUCCEEDED( hResult ) ) {
                colorRef->AcquireFrame( &color );
            }
            if (color != NULL) {
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
                } else {
                    input->color.clear();
                }
            }
            SafeRelease(colorRef);
            SafeRelease(color);
        }

        if ((m_enabledChannels & CHANNEL_COLOR_REG) && (m_enabledChannels & CHANNEL_DEPTH) && !input->color.empty() && !input->depth.empty()) {
            cm::Image& depth = input->depth;
            input->colorReg.updateSize(depth.rows, depth.cols, 3);
            input->colorReg.clear();
            const int depthPixelsNum = depth.cols * depth.rows;
            // Translate depth image to color image dimensions
            if (m_colorPoints == nullptr) {
                m_colorPoints = new ColorSpacePoint[input->color.rows * input->color.cols];
            }
            hResult = m_coordMapper->MapDepthFrameToColorSpace((UINT)depthPixelsNum, (UINT16*)depth.data, (UINT)depthPixelsNum, m_colorPoints);
            if (SUCCEEDED(hResult)) {
                for (int i = 0; i < depthPixelsNum; ++i) {

                    // Check if pixel is valid
                    UINT16 pixValue = (UINT16)depth.data[i];
                    if (pixValue <= 0 || pixValue > m_params.range) {
                        continue;
                    }

                    // Get coordinates of corresponding color point
                    ColorSpacePoint colorPoint = m_colorPoints[i];

                    // Check if mapping is correct
                    if (colorPoint.X == -std::numeric_limits<float>::infinity() || colorPoint.Y == -std::numeric_limits<float>::infinity()) {
                        Log::t("Improper mapping", m_moduleTag);
                        continue;
                    }

                    int colorX = static_cast<int>(colorPoint.X + 0.5f);
                    int colorY = static_cast<int>(colorPoint.Y + 0.5f);

                    if (colorX >= 0 && colorX < input->color.cols && colorY > 0 && colorY < input->color.rows) {
                        int idx = colorY * input->color.cols + colorX;
                        input->colorReg.data[3 * i] = input->color.data[3 * idx];
                        input->colorReg.data[3 * i + 1] = input->color.data[3 * idx + 1];
                        input->colorReg.data[3 * i + 2] = input->color.data[3 * idx + 2];
                    }
                }
            } else {
                Log::w("Coordinate mapping failed", m_moduleTag);
                input->colorReg.clear();
            }
        }

        if (m_enabledChannels & CHANNEL_BODY_SKELETON) {
            IBodyFrame* body = NULL;
            IBodyFrameReference* bodyRef = NULL;
            hResult = frame->get_BodyFrameReference( &bodyRef );
            if ( SUCCEEDED( hResult ) ) {
                bodyRef->AcquireFrame( &body );
            }
            if (body != NULL) {
                IBody* bodies[BODY_COUNT] = { 0 };
                hResult = body->GetAndRefreshBodyData(_countof(bodies), bodies);
                if (SUCCEEDED(hResult)) {
                    getBodySkeletons(bodies, input);
                }
                for (int i = 0; i < _countof(bodies); ++i) {
                    SafeRelease(bodies[i]);
                }
            }
            SafeRelease(bodyRef);
            SafeRelease(body);
        }

        SafeRelease(desc);
    }
    SafeRelease(frame);

    Log::t("got next frame", m_moduleTag);

    return true;
//    SafeRelease( skel );
//    SafeRelease( masks );
}

void CameraKinect2::getBodySkeletons(IBody** bodies, cm::InputData* input) {
    for (int i = 0; i < BODY_COUNT; ++i) {
        cm::InputUser& user = input->users[i];

        BOOLEAN tracked;
        bodies[i]->get_IsTracked(&tracked);
        user.m_active = (tracked == TRUE);

        UINT64 id;
        bodies[i]->get_TrackingId(&id);
        user.m_id = (int)id;

        Joint joints[JointType_Count];
        bodies[i]->GetJoints(_countof(joints), joints);
        for (int j = 0; j < _countof(joints); ++j) {

            DepthSpacePoint depthPos = {0};
            m_coordMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthPos);

            user.bodyJoints[j].pos2d.x = (int)depthPos.X;
            user.bodyJoints[j].pos2d.y = (int)depthPos.Y;

            user.bodyJoints[j].pos3d.x = joints[j].Position.X;
            user.bodyJoints[j].pos3d.y = joints[j].Position.Y;
            user.bodyJoints[j].pos3d.z = joints[j].Position.Z;
        }

        //user.bodyJoints
    }
}
