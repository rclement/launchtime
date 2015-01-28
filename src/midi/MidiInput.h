#ifndef __MIDI_INPUT__
#define __MIDI_INPUT__ 1

#include "MidiMessage.h"
#include <string>
#include <vector>
#include <memory>

namespace launchtime
{

/**
 * This class provides access to MIDI input ports, by opening ports and 
 * receiving MIDI messages.
 */
class MidiInput
{
    public:

        /**
         * MidiInput listener class to be notified with MIDI messages.
         * Inherit from this class and register to a MidiInput class
         * to receive MIDI messages.
         */
        class Listener
        {
            public:

                /**
                 * Notifiy when a new MIDI message is received.
                 *
                 * @param timestamp Time-stamp associated to the MIDI message
                 * @param message New MIDI message
                 * @see MidiMessage
                 */
                virtual void midiMessage(double timestamp,
                                         const MidiMessage& message) = 0;
        };

    public:

        /**
         * Create a MIDI input.
         *
         * @param name Name for the MIDI input
         */
        MidiInput(const std::string& name = "Midi Input Client");

        /**
         * Destroy the MIDI input.
         */
        virtual ~MidiInput();

        /**
         * Open a MIDI input port.
         *
         * @param number Index of the port (between 0 and (numPorts()-1))
         * @param name Name to give to the opened port
         */
        void openPort(unsigned int number = 0,
                      const std::string& name = "Midi Input");

        /**
         * Open a virtual MIDI input port.
         *
         * @param name Name to give to the opened port
         */
        void openVirtualPort(const std::string& name = "Midi Input");

        /**
         * Close the currently opened MIDI input port (if any).
         */
        void closePort(void);

        /**
         * Check if MIDI input port is already opened.
         *
         * @return true if a port is already opened, else false
         */
        bool openedPort(void) const;

        /**
         * Inspect how many MIDI input ports are available.
         *
         * @return The number of available ports
         */
        unsigned int numPorts(void) const;

        /**
         * Find the name of a given MIDI input port.
         *
         * @param number Index of the port (between 0 and (numPorts()-1))
         * @return The name of the selected port
         */
        std::string portName(const int number) const;

        /**
         * Specify if specific types of MIDI messages should be ignored.
         * By default System Exclusive (SysEx), timing and sensing messages
         * are ignored.
         *
         * @param sysex Ignore sysex messages
         * @param time Ignore timing messages
         * @param sense Ignore sensing messages
         */
        void ignoreMidiMessages(bool sysex = true,
                                bool time = true,
                                bool sense = true);

        /**
         * Register a MIDI input listener to be notify with MIDI messages.
         *
         * @param listener Listener to be registered
         */
        void addListener(MidiInput::Listener* const listener);

        /**
         * Remove a registered MIDI input listener.
         *
         * @param listener Listener to be removed
         */
        void removeListener(MidiInput::Listener* const listener);

    private:

        MidiInput(const MidiInput&);
        MidiInput& operator=(const MidiInput&);

        class Private;
        std::unique_ptr<Private> mData;
};

} /* namespace launchtime */

#endif // __MIDI_INPUT__
