#include "Launchtime.h"

namespace launchtime
{

Launchtime::Launchtime(const std::string& devicename,
                       const std::string& virtualport)
{
    mLaunchpad = new LaunchpadInterface();
    mLaunchpad->connect(devicename);

    mHost = new HostInterface();
    mHost->connect(virtualport);

    mSequencer = std::unique_ptr<LaunchpadSequencer>(new LaunchpadSequencer());
    mSequencer->setLaunchpad(mLaunchpad);
    mSequencer->setMidiHost(mHost);
    mSequencer->enable();
}

Launchtime::~Launchtime()
{
    mSequencer->disable();

    delete mLaunchpad;
    delete mHost;
}

} /* namespace launchtime */

