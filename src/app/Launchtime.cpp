#include "Launchtime.h"
#include "MidiInput.h"
#include "MidiOutput.h"

namespace launchtime
{

Launchtime::Launchtime()
{
    mLaunchpad = new LaunchpadInterface();
    mHost = new HostInterface();
    mSequencer = std::unique_ptr<LaunchpadSequencer>(new LaunchpadSequencer());

    mConnected = false;
}

Launchtime::~Launchtime()
{
    mSequencer->disable();

    delete mLaunchpad;
    delete mHost;
}

std::vector< std::string > Launchtime::availableMidiDevices()
{
    std::vector< std::string > devices;
    MidiInput in;

    for (unsigned int i = 0; i < in.numPorts(); i++)
    {
        devices.push_back(in.portName(i));
    }

    return devices;
}

bool Launchtime::isConnected()
{
    return mConnected;
}

bool Launchtime::connect(const std::string& devicename,
                         const std::string& virtualport)
{
    bool launchpadConnected = mLaunchpad->connect(devicename);
    bool hostConnected = mHost->connect(virtualport);

    mSequencer->setLaunchpad(mLaunchpad);
    mSequencer->setMidiHost(mHost);
    mSequencer->enable();

    mConnected = launchpadConnected && hostConnected;

    return mConnected;
}

bool Launchtime::disconnect()
{
    mSequencer->disable();
    mLaunchpad->disconnect();
    mHost->disconnect();

    mConnected = false;

    return !mConnected;
}

} /* namespace launchtime */

