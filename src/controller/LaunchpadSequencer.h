#ifndef __LAUNCHPAD_SEQUENCER__
#define __LAUNCHPAD_SEQUENCER__ 1

#include "LaunchpadController.h"

namespace launchtime
{

class LaunchpadSequencer : public LaunchpadController
{
    public:

        LaunchpadSequencer();
        virtual ~LaunchpadSequencer();

    protected:

        virtual void updateGridLEDs();
        virtual void updateBankLEDs();
        virtual void updateControlsLEDs();

        virtual void eventPadPressed(const int x, const int y);
        virtual void eventPadReleased(const int x, const int y);
        virtual void eventBankPressed(const int id);
        virtual void eventBankReleased(const int id);
        virtual void eventControlPressed(const int id);
        virtual void eventControlReleased(const int id);
        virtual void eventMidiMessage(const MidiMessage& message);

        void updateCurrentBeat();
        void sendMidiNotes();
        void resetGrid();

    private:

        LaunchpadSequencer(const LaunchpadSequencer&);
        LaunchpadSequencer& operator=(const LaunchpadSequencer&);

        bool m_Running;
        int m_LinesCount;
        int m_MidiClockCount;
        int m_MidiClockCountBar;
        int m_MidiClockSteps;
        bool m_MidiClockNewStepsPending;
        int m_MidiClockNewSteps;
        int m_CurrPosX;
        int m_CurrPosY;
        int m_PrevPosX;
        int m_PrevPosY;
        int m_MidiNotes[8];
        LaunchpadPadState::PushState m_Muted[8];
};

} /* namespace launchtime */

#endif /* __LAUNCHPAD_SEQUENCER__ */

