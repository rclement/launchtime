#ifndef __HOST_INTERFACE__
#define __HOST_INTERFACE__ 1

#include "MidiInput.h"
#include "MidiOutput.h"
#include <string>
#include <list>

namespace launchtime
{

class HostInterface : public MidiInput::Listener
{
    public:

        class Listener
        {
            public:
                virtual void midiMessageReceived(HostInterface* midi_host,
                    const MidiMessage& message) = 0;
        };

        HostInterface();
        virtual ~HostInterface();

        bool connect(const std::string& hostname);
        void disconnect();

        void addListener(Listener* listener);
        void removeListener(Listener* listener);

        void sendMidiMessage(const MidiMessage& message);

    private:

        HostInterface(const HostInterface&);
        HostInterface& operator=(const HostInterface&);

        virtual void midiMessage(double timestamp,
                                 const MidiMessage& message);

        std::string mName;
        MidiInput mInput;
        MidiOutput mOutput;
        bool mConnected;
        std::list<Listener*> mListeners;
};

} /* namespace launchtime */

#endif /* __HOST_INTERFACE__ */

