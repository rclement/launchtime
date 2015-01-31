#ifndef __LAUNCHPAD_CONTROLLER__
#define __LAUNCHPAD_CONTROLLER__ 1

#include "LaunchpadInterface.h"
#include "HostInterface.h"
#include <string>

namespace launchtime
{

class LaunchpadPadState
{
public:

    enum PushState {
        Disabled = 0,
        Enabled,
        Disabling,
        Enabling
    };

    LaunchpadPadState() :
        m_PushState(Disabled),
        m_Color(LaunchpadInterface::ColorOff),
        m_Note(0x24),
        m_Velocity(127) {}
    virtual ~LaunchpadPadState() {}

    PushState pushState() const
    {
        return m_PushState;
    }
    LaunchpadInterface::Color color() const
    {
        return m_Color;
    }
    int note() const
    {
        return m_Note;
    }
    int velocity() const
    {
        return m_Velocity;
    }

    void setPushState(const PushState state)
    {
        m_PushState = state;
    }
    void setColor(const LaunchpadInterface::Color color)
    {
        m_Color = color;
    }
    void setNote(const int note)
    {
        m_Note = note;
    }
    void setVelocity(const int velocity)
    {
        m_Velocity = velocity;
    }

private:
    PushState m_PushState;
    LaunchpadInterface::Color m_Color;
    int m_Note;
    int m_Velocity;
};

class LaunchpadController : public LaunchpadInterface::Listener,
    public HostInterface::Listener
{
public:

    LaunchpadController(const std::string & name);
    virtual ~LaunchpadController();

    void setLaunchpad(LaunchpadInterface* launchpad);
    void setMidiHost(HostInterface* midi_host);

    std::string name() const;

    void enable(const bool launchpad = true, const bool midi_host = true);
    void disable(const bool launchpad = true, const bool midi_host = true);

    void padEvent(LaunchpadInterface* launchpad, const int x, const int y, const bool on);
    void bankEvent(LaunchpadInterface* launchpad, const int id, const bool on);
    void controlEvent(LaunchpadInterface* launchpad, const int id, const bool on);

    void midiMessageReceived(HostInterface* midi_host, const MidiMessage& message);

protected:

    void turnOnPadLED(const int x, const int y, const LaunchpadInterface::Color color);
    void turnOffPadLED(const int x, const int y);
    void turnOnBankLED(const int id, const LaunchpadInterface::Color color);
    void turnOffBankLED(const int id);
    void turnOnControlLED(const int id, const LaunchpadInterface::Color color);
    void turnOffControlLED(const int id);
    void turnOffAllLEDs();

    void sendMidiMessage(std::vector<unsigned char>* message);

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

protected:

    LaunchpadPadState mGrid[8][8];

private:

    LaunchpadController(const LaunchpadController&);
    LaunchpadController& operator=(const LaunchpadController&);

    std::string mName;
    LaunchpadInterface* mLaunchpad;
    HostInterface* mMidiHost;
    bool mLaunchpadEnabled;
    bool mMidiHostEnabled;
};

} /* namespace launchtime */

#endif /* __LAUNCHPAD_CONTROLLER__ */

