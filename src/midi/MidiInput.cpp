#include "MidiInput.h"
#include "rtmidi/RtMidi.h"
#include <list>

namespace launchtime
{

class MidiInput::Private
{
    public:

        Private(const std::string & name) :
            mName(name),
            mDevice(RtMidi::UNSPECIFIED, mName)
        {
            mDevice.setCallback(midiMessageCallback, this);
        }

        ~Private()
        {
            mDevice.cancelCallback();
        }

        static void midiMessageCallback(double timestamp,
                                        std::vector<unsigned char>* message,
                                        void* userdata)
        {
            MidiInput::Private* obj = reinterpret_cast<MidiInput::Private*>(userdata);
            MidiMessage msg(*message);

            if (obj)
            {
                for (std::list<MidiInput::Listener*>::iterator it = obj->mListener.begin();
                     it != obj->mListener.end();
                     it++)
                {
                    
                    (*it)->midiMessage(timestamp, msg);
                }
            }
        }

        std::string mName;
        RtMidiIn mDevice;
        std::list<MidiInput::Listener*> mListener;

    private:

        Private(const Private&);
        Private& operator=(const Private&);
};

/* ========================================================================= */

MidiInput::MidiInput(const std::string & name) :
    mData(new Private(name))
{
}

MidiInput::~MidiInput()
{
}

void MidiInput::openPort(unsigned int number, const std::string& name)
{
    mData->mDevice.openPort(number, name);
}

void MidiInput::openVirtualPort(const std::string& name)
{
    mData->mDevice.openVirtualPort(name);
}

void MidiInput::closePort(void)
{
    mData->mDevice.closePort();
}

bool MidiInput::openedPort(void) const
{
    return mData->mDevice.isPortOpen();
}

unsigned int MidiInput::numPorts(void) const
{
    return mData->mDevice.getPortCount();
}

std::string MidiInput::portName(const int number) const
{
    return mData->mDevice.getPortName(number);
}

void MidiInput::ignoreMidiMessages(bool sysex, bool time, bool sense)
{
    mData->mDevice.ignoreTypes(sysex, time, sense);
}

void MidiInput::addListener(MidiInput::Listener* const listener)
{
    mData->mListener.push_back(listener);
}

void MidiInput::removeListener(MidiInput::Listener* const listener)
{
    mData->mListener.remove(listener);
}

} /* namespace launchtime */

