#ifndef __LAUNCHTIME__
#define __LAUNCHTIME__ 1

#include "HostInterface.h"
#include "LaunchpadInterface.h"
#include "LaunchpadSequencer.h"
#include <memory>
#include <string>
#include <vector>

namespace launchtime
{

class Launchtime
{
    public:

        Launchtime();
        virtual ~Launchtime();

        std::vector< std::string > availableMidiDevices();

        bool isConnected();
        bool connect(const std::string& devicename,
                     const std::string& virtualport);
        bool disconnect();

    private:

        Launchtime(const Launchtime&);
        Launchtime& operator=(const Launchtime&);

        HostInterface* mHost;
        LaunchpadInterface* mLaunchpad;
        std::unique_ptr<LaunchpadSequencer> mSequencer;
        bool mConnected;
};

} /* namespace launchtime */

#endif /* __LAUNCHTIME__ */

