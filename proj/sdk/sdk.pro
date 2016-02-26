TARGET = sdk

TEMPLATE = lib

# Use only MPL2 licensed part of eigen
DEFINES += EIGEN_MPL2_ONLY

QMAKE_CXXFLAGS += -DJSON_IS_AMALGAMATION

INCLUDEPATH += \
        ../../include \
        ../../src/input \
        ../../src/common \
        ../../3rdparty/eigen \
        ../../3rdparty/jsoncpp \
        ../../3rdparty/opencv/include

HEADERS = \
        ../../include/platforms.h \
        ../../include/user_types.h \
        ../../include/skeleton_types.h \
        ../../include/gesture_types.h \
        ../../src/common/license_checker.h \
        ../../src/sdk/sdk_dev.h \
        ../../src/common/log.h \
        ../../src/common/input_data.h \
        ../../src/sdk/frame_pack.h \
    ../../include/camera_types.h \
    ../../include/recording_types.h


SOURCES = \
        ../../src/sdk/sdk_dev.cpp \
        ../../src/common/log.cpp \
        ../../src/common/input_data.cpp \
        ../../src/sdk/frame_pack.cpp

CONFIG(debug, debug | release) : {
    DESTDIR = ../debug
    LIBS += ../../bin/debug/input.lib
} else {
    DESTDIR = ../release
    LIBS += ../../bin/release/input.lib
}

LIBS += \
        ../../3rdparty/opencv/lib/windows/Release/opencv_core310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgcodecs310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgproc310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_highgui310.lib

