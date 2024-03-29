TARGET = input

TEMPLATE = lib

INCLUDEPATH += \
        ../../include \
        ../../src/common \
        ../../3rdparty/jsoncpp \
        ../../3rdparty/opencv/include \
        ../../3rdparty/kinect_sdk/inc \
        ../../3rdparty/visual_leak_detector/include

HEADERS = \
        ../../include/platforms.h \
        ../../include/camera_types.h \
        ../../src/common/log.h \
        ../../src/common/input_data.h \
        ../../src/input/camera.h \
        ../../src/input/input.h \
        ../../src/input/camera_file.h \
        ../../src/input/camera_kinect2.h \
        ../../src/input/new_input_event.h


SOURCES = \
        ../../src/common/log.cpp \
        ../../src/common/input_data.cpp \
        ../../src/input/camera.cpp \
        ../../src/input/input.cpp \
        ../../src/input/camera_file.cpp \
        ../../src/input/camera_kinect2.cpp
		
LIBS += \
        ../../3rdparty/kinect_sdk/lib/Kinect20.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_core310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgcodecs310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgproc310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_highgui310.lib \
        ../../3rdparty/visual_leak_detector/lib/Win64/vld.lib

CONFIG(debug, debug | release) : {
    DESTDIR = ../debug
} else {
    DESTDIR = ../release
}
