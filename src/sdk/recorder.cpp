#include "recorder.h"
#include "recording_types.h"

#include "frame_pack.h"

Recorder::Recorder() :
    m_recordingChannels(RECORDING_NONE),
    m_enabled(false)
{
}

void Recorder::setEnabled(bool enabled) {
    if (m_enabled == false && enabled == true) {
        saveConfigFile();
    }
    m_enabled = enabled;
}

void Recorder::processNewFrame(FramePack* frame) {
    if (!m_enabled || m_path == "") {
        return;
    }

}

void Recorder::saveConfigFile() {

}
