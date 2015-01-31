#include "HostInterface.h"

namespace launchtime
{

HostInterface::HostInterface() :
    mName(),
    mInput("Host Interface Input"),
    mOutput("Host Interface Output"),
    mConnected(false),
    mListeners()
{
}

HostInterface::~HostInterface()
{
    disconnect();
}

bool HostInterface::connect(const std::string& hostname)
{
    if (mConnected) {
        disconnect();
    }

    mInput.openVirtualPort(hostname);
    mOutput.openVirtualPort(hostname);

    mInput.ignoreMidiMessages(true, false, true);
    mInput.addListener(this);

    mConnected = true;
    mName = hostname;

    return mConnected;
}

void HostInterface::disconnect()
{
    mInput.closePort();
    mOutput.closePort();
    mConnected = false;
}

void HostInterface::addListener(Listener* listener)
{
    mListeners.push_back(listener);
}

void HostInterface::removeListener(Listener* listener)
{
    mListeners.remove(listener);
}

void HostInterface::sendMidiMessage(const MidiMessage& message)
{
    mOutput.sendMessage(message);
}

void HostInterface::midiMessage(double timestamp,
                                const MidiMessage& message)
{
    for (std::list<Listener*>::iterator it = mListeners.begin();
         it != mListeners.end();
         it++) {
        (*it)->midiMessageReceived(this, message);
    }
}

} /* namespace launchtime */

