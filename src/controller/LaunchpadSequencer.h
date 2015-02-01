#ifndef __LAUNCHPAD_SEQUENCER__
#define __LAUNCHPAD_SEQUENCER__ 1

#include "LaunchpadController.h"

namespace launchtime
{

class LaunchpadSequencer : public LaunchpadController
{
public:

    class PadState
    {
    public:

        enum PushState {
            Disabled = 0,
            Enabled,
            Disabling,
            Enabling
        };

        PadState() :
            pushState(Disabled),
            color(LaunchpadInterface::ColorOff),
            note(0x24),
            velocity(127) {}

        virtual ~PadState() {}

        PushState pushState;
        LaunchpadInterface::Color color;
        int note;
        int velocity;
    };

    LaunchpadSequencer();
    virtual ~LaunchpadSequencer();

    void setMidiChannel(const int channel);

private:

    LaunchpadSequencer(const LaunchpadSequencer&);
    LaunchpadSequencer& operator=(const LaunchpadSequencer&);

    void updateCurrentBeat();
    void sendMidiNotes();
    void resetGrid();

    void updateGridLEDs();
    void updateBankLEDs();
    void updateControlsLEDs();

    void eventPadPressed(const int x, const int y);
    void eventPadReleased(const int x, const int y);
    void eventBankPressed(const int id);
    void eventBankReleased(const int id);
    void eventControlPressed(const int id);
    void eventControlReleased(const int id);
    void eventMidiMessage(const MidiMessage& message);

    bool m_Running;
    int m_LinesCount;
    int m_MidiClockCount;
    int m_MidiClockCountBar;
    int m_MidiClockSteps;
    bool m_MidiClockNewStepsPending;
    int m_MidiClockNewSteps;
    int m_MidiChannel;
    int m_CurrPosX;
    int m_CurrPosY;
    int m_PrevPosX;
    int m_PrevPosY;
    int m_MidiNotes[8];
    PadState::PushState m_Muted[8];
    PadState mGrid[8][8];
};

} /* namespace launchtime */

#endif /* __LAUNCHPAD_SEQUENCER__ */

