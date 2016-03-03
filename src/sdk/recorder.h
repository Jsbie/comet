#ifndef RECORDER_H
#define RECORDER_H

#include <string>

class FramePack;
class Image;

class Recorder
{
public:
    Recorder();

    void setEnabled(bool enabled);
    void processNewFrame(FramePack* frame);
    void saveFramePack(FramePack* frame);
    void saveImage(Image* img, const char* type);

    int     m_counter;
    int     m_recordingChannels;
    bool    m_enabled;
    std::string     m_path;

private:
    void saveConfigFile();
};

#endif // RECORDER_H
