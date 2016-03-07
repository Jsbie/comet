#ifndef RECORDER_H
#define RECORDER_H

#include <string>
#include <queue>
#include <thread>
#include <mutex>

class FramePack;
namespace cm {
    class Image;
}

class Recorder
{
public:
    Recorder();
    ~Recorder();

    void setEnabled(bool enabled);
    void processNewFrame(FramePack* frame);
    void saveFramePack(FramePack* frame);
    void saveImage(cm::Image* img, const char* type, bool convert = false);

    const char* m_moduleTag = "REC";

    int     m_recordingChannels;
    bool    m_enabled;
    std::string     m_path; ///< Path to pre-existing folder for recording

private:
    void saveConfigFile();
    void run();

    const int   MAX_QUEUE_LENGTH = 200;

    int         m_counter;  ///< Recorded frames counter
    std::mutex  m_mutex;    ///< Mutex to prevent invalid read/write to queue
    std::thread m_thread;   ///< Thread to save frames from queue
    bool        m_threadActive;         ///< Flag to indicate if thread is running
    std::queue<FramePack*>  m_queue;    ///< Queue to save framepacks for saving
};

#endif // RECORDER_H
