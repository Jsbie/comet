QT += widgets

TARGET = studio

INCLUDEPATH     = \
    ../../include \
    ../../src/sdk \
    ../../src/common \
    ../../3rdparty/opencv/include

HEADERS         = \
    ../../src/studio/main_window.h \
    ../../src/studio/dir_widget.h

SOURCES         = \
    ../../src/common/log.cpp \
    ../../src/studio/main.cpp \
    ../../src/studio/main_window.cpp

CONFIG(debug, debug | release) : {
    DESTDIR = ../debug
    LIBS += ../../bin/debug/sdk.lib
} else {
    DESTDIR = ../release
    LIBS +=  ../../bin/release/sdk.lib
}

LIBS +=  \
        ../../3rdparty/opencv/lib/windows/Release/opencv_core310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgcodecs310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_imgproc310.lib \
        ../../3rdparty/opencv/lib/windows/Release/opencv_highgui310.lib

RESOURCES       = ../../src/studio/resources.qrc
