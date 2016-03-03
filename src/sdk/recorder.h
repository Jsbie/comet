#ifndef RECORDER_H
#define RECORDER_H

#include <string>

class FramePack;

class Recorder
{
public:
    Recorder();

    void setEnabled(bool enabled);
    void processNewFrame(FramePack* frame);

    int     m_recordingChannels;
    bool    m_enabled;
    std::string     m_path;

private:
    void saveConfigFile();
};

#endif // RECORDER_H