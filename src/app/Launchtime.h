#ifndef __LAUNCHTIME__
#define __LAUNCHTIME__ 1

#include "HostInterface.h"
#include "LaunchpadInterface.h"
#include "LaunchpadSequencer.h"
#include <memory>
#include <string>

namespace launchtime
{

class Launchtime
{
    public:

        Launchtime(const std::string& devicename,
                   const std::string& virtualport);
        virtual ~Launchtime();

    private:

        Launchtime(const Launchtime&);
        Launchtime& operator=(const Launchtime&);

        HostInterface* mHost;
        LaunchpadInterface* mLaunchpad;
        std::unique_ptr<LaunchpadSequencer> mSequencer;
};

} /* namespace launchtime */

#endif /* __LAUNCHTIME__ */

