#ifndef __LAUNCHPAD_CONTROLLER__
#define __LAUNCHPAD_CONTROLLER__ 1

#include "LaunchpadInterface.h"
#include "HostInterface.h"
#include <string>

namespace launchtime
{

class LaunchpadController : public LaunchpadInterface::Listener,
    public HostInterface::Listener
{
public:

    LaunchpadController(const std::string & name);
    virtual ~LaunchpadController();

    void setLaunchpad(LaunchpadInterface* launchpad);
    void setMidiHost(HostInterface* midi_host);

    std::string name() const;

    void enable();
    void disable();

protected:

    void turnOnPadLED(const int x, const int y, const LaunchpadInterface::Color color);
    void turnOffPadLED(const int x, const int y);
    void turnOnBankLED(const int id, const LaunchpadInterface::Color color);
    void turnOffBankLED(const int id);
    void turnOnControlLED(const int id, const LaunchpadInterface::Color color);
    void turnOffControlLED(const int id);
    void turnOffAllLEDs();

    void sendMidiMessage(std::vector<unsigned char>* message);

private:

    LaunchpadController(const LaunchpadController&);
    LaunchpadController& operator=(const LaunchpadController&);

    void padEvent(LaunchpadInterface* launchpad, const int x, const int y, const bool on);
    void bankEvent(LaunchpadInterface* launchpad, const int id, const bool on);
    void controlEvent(LaunchpadInterface* launchpad, const int id, const bool on);

    void midiMessageReceived(HostInterface* midi_host, const MidiMessage& message);

    virtual void updateGridLEDs() = 0;
    virtual void updateBankLEDs() = 0;
    virtual void updateControlsLEDs() = 0;

    virtual void eventPadPressed(const int x, const int y) = 0;
    virtual void eventPadReleased(const int x, const int y) = 0;
    virtual void eventBankPressed(const int id) = 0;
    virtual void eventBankReleased(const int id) = 0;
    virtual void eventControlPressed(const int id) = 0;
    virtual void eventControlReleased(const int id) = 0;
    virtual void eventMidiMessage(const MidiMessage& message) = 0;

    std::string mName;
    LaunchpadInterface* mLaunchpad;
    HostInterface* mMidiHost;
    bool mLaunchpadEnabled;
    bool mMidiHostEnabled;
};

} /* namespace launchtime */

#endif /* __LAUNCHPAD_CONTROLLER__ */

