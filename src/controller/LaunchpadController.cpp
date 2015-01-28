#include "LaunchpadController.h"

namespace launchtime
{

LaunchpadController::LaunchpadController(const std::string & name) :
    mLaunchpad(NULL),
    mMidiHost(NULL),
    mLaunchpadEnabled(false),
    mMidiHostEnabled(false)
{
}

LaunchpadController::~LaunchpadController()
{
    disable();

    mLaunchpad = NULL;
    mMidiHost = NULL;
}

void LaunchpadController::setLaunchpad(LaunchpadInterface* launchpad)
{
    mLaunchpad = launchpad;
}

void LaunchpadController::setMidiHost(HostInterface* midi_host)
{
    mMidiHost = midi_host;
}

std::string LaunchpadController::name() const
{
    return mName;
}

void LaunchpadController::enable(const bool launchpad, const bool midi_host)
{
    if (mLaunchpad && !mLaunchpadEnabled && launchpad)
    {
        mLaunchpad->addListener(this);
        mLaunchpadEnabled = true;
        updateGridLEDs();
        updateBankLEDs();
        updateControlsLEDs();
    }

    if (mMidiHost && !mMidiHostEnabled && midi_host)
    {
        mMidiHost->addListener(this);
        mMidiHostEnabled = true;
    }
}

void LaunchpadController::disable(const bool launchpad, const bool midi_host)
{
    turnOffAllLEDs();

    if (mLaunchpad && mLaunchpadEnabled && launchpad)
    {
        mLaunchpadEnabled = false;
        mLaunchpad->removeListener(this);
    }

    if (mMidiHost && mMidiHostEnabled && midi_host)
    {
        mMidiHostEnabled = false;
        mMidiHost->removeListener(this);
    }
}

void LaunchpadController::padEvent(LaunchpadInterface* launchpad, const int x, const int y, const bool on)
{
    if (on)
    {
        eventPadPressed(x, y);
    }
    else
    {
        eventPadReleased(x, y);
    }
}

void LaunchpadController::bankEvent(LaunchpadInterface* launchpad, const int id, const bool on)
{
    if (on)
    {
        eventBankPressed(id);
    }
    else
    {
        eventBankReleased(id);
    }
}

void LaunchpadController::controlEvent(LaunchpadInterface* launchpad, const int id, const bool on)
{
    if (on)
    {
        eventControlPressed(id);
    }
    else
    {
        eventControlReleased(id);
    }
}

void LaunchpadController::midiMessageReceived(HostInterface* midi_host, const MidiMessage& message)
{
    eventMidiMessage(message);
}

void LaunchpadController::turnOnPadLED(const int x, const int y, const LaunchpadInterface::Color color)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOnPadLED(x, y, color);
        mGrid[x][y].setColor(color);
    }
}

void LaunchpadController::turnOffPadLED(const int x, const int y)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOffPadLED(x, y);
        mGrid[x][y].setColor(LaunchpadInterface::ColorOff);
    }
}

void LaunchpadController::turnOnBankLED(const int id, const LaunchpadInterface::Color color)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOnBankLED(id, color);
    }
}

void LaunchpadController::turnOffBankLED(const int id)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOffBankLED(id);
    }
}

void LaunchpadController::turnOnControlLED(const int id, const LaunchpadInterface::Color color)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOnControlLED(id, color);
    }
}

void LaunchpadController::turnOffControlLED(const int id)
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOffControlLED(id);
    }
}

void LaunchpadController::turnOffAllLEDs()
{
    if (mLaunchpadEnabled)
    {
        mLaunchpad->turnOffAllLEDs();
    }
}

void LaunchpadController::sendMidiMessage(std::vector<unsigned char>* message)
{
    if (mMidiHostEnabled)
    {
        mMidiHost->sendMidiMessage(MidiMessage(*message));
    }
}

} /* namespace launchtime */

