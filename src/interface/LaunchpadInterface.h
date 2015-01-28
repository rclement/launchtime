#ifndef __LAUNCHPAD_INTERFACE__
#define __LAUNCHPAD_INTERFACE__ 1

#include "MidiInput.h"
#include "MidiOutput.h"
#include <list>

namespace launchtime
{



class LaunchpadInterface : public MidiInput::Listener
{
    public:

        class Listener
        {
            public:

                virtual void padEvent(LaunchpadInterface* launchpad,
                                      const int x,
                                      const int y,
                                      const bool on) = 0;
                virtual void bankEvent(LaunchpadInterface* launchpad,
                                       const int id,
                                       const bool on) = 0;
                virtual void controlEvent(LaunchpadInterface* launchpad,
                                          const int id,
                                          const bool on) = 0;
        };

        enum Color
        {
            ColorOff = 0x0C,

            ColorDimRed = 0x0D,
            ColorMidRed = 0x0E,
            ColorFullRed = 0x0F,

            ColorDimGreen = 0x1C,
            ColorMidGreen = 0x2C,
            ColorFullGreen = 0x3C,

            ColorDimOrange = 0x1E,
            ColorMidOrange = 0x1F,
            ColorFullOrange = 0x2F,

            ColorDimYellow = 0x2D,
            ColorMidYellow = 0x3D,
            ColorFullYellow = 0x3E,

            ColorDimAmber = 0x1D,
            ColorMidAmber = 0x2E,
            ColorFullAmber = 0x3F
        };

        LaunchpadInterface();
        virtual ~LaunchpadInterface();

        bool connect(const std::string& devicename);
        void disconnect();

        void addListener(Listener* listener);
        void removeListener(Listener* listener);

        void turnOnAllLEDs();
        void turnOffAllLEDs();
        void turnOnPadLED(const int x, const int y, const Color color);
        void turnOffPadLED(const int x, const int y);
        void turnOnBankLED(const int id, const Color color);
        void turnOffBankLED(const int id);
        void turnOnControlLED(const int id, const Color color);
        void turnOffControlLED(const int id);
        void enableXYLayout();
        void enableDrumRackLayout();
        void displayMessage(const std::string & message);
        void stopDisplayingMessage();
        void midiMessageReceived(std::vector<unsigned char>* message);

    private:

        LaunchpadInterface(const LaunchpadInterface&);
        LaunchpadInterface& operator=(const LaunchpadInterface&);

        virtual void midiMessage(double timestamp, const MidiMessage& message);

        MidiInput mInput;
        MidiOutput mOutput;
        std::string mDeviceName;
        bool mConnected;
        std::list<Listener*> mListeners;
};

} /* namespace launchtime */

#endif /* __LAUNCHPAD_INTERFACE__ */

