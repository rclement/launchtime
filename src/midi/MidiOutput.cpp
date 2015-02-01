#include "MidiOutput.h"
#include "rtmidi/RtMidi.h"
#include <list>

namespace launchtime
{

class MidiOutput::Private
{
public:

    Private(const std::string & name) :
        mName(name),
        mDevice(RtMidi::UNSPECIFIED, mName)
    {}

    ~Private()
    {}

    std::string mName;
    RtMidiOut mDevice;

private:

    Private(const Private&);
    Private& operator=(const Private&);
};

/* ========================================================================= */

MidiOutput::MidiOutput(const std::string & name) :
    mData(new Private(name))
{
}

MidiOutput::~MidiOutput()
{
}

void MidiOutput::openPort(unsigned int number, const std::string& name)
{
    mData->mDevice.openPort(number, name);
}

void MidiOutput::openVirtualPort(const std::string& name)
{
    mData->mDevice.openVirtualPort(name);
}

void MidiOutput::closePort(void)
{
    mData->mDevice.closePort();
}

bool MidiOutput::openedPort(void) const
{
    return mData->mDevice.isPortOpen();
}

unsigned int MidiOutput::numPorts(void) const
{
    return mData->mDevice.getPortCount();
}

std::string MidiOutput::portName(const int number) const
{
    return mData->mDevice.getPortName(number);
}

void MidiOutput::sendMessage(const MidiMessage& message)
{
    std::vector<unsigned char> msg = message.data();
    mData->mDevice.sendMessage(& msg);
}

} /* namespace launchtime */

