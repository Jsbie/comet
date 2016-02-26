#ifndef CAMERA_TYPES_H
#define CAMERA_TYPES_H

enum CameraType {
    CAMERA_KINECT2 = 0,
    CAMERA_FILE = 1
};

enum CameraChannels {
    CHANNEL_NONE = 0,
    CHANNEL_DEPTH = 2,
    CHANNEL_IR = 4,
    CHANNEL_COLOR = 8,
    CHANNEL_COLOR_REG = 16,
    CHANNEL_BODY_SKELETON = 32,
    CHANNEL_BODY_MASK = 64
};

struct CameraParameters {
    int     fps;            ///< Camera's framerate
    int     range;          ///< Max range of depth camera in mm
    float   focalLength;    ///< Focal length of depth camera in mm
    int     depthResW;      ///< Depth resolution - width
    int     depthResH;      ///< Depth resolution - height
    int     colorResW;      ///< Color resolution - width
    int     colorResH;      ///< Color resolution - height
};

#endif // CAMERA_TYPES_H
