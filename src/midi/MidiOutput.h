#ifndef __MIDI_OUTPUT__
#define __MIDI_OUTPUT__ 1

#include "MidiMessage.h"
#include <string>
#include <vector>
#include <memory>

namespace launchtime
{

/**
 * This class provides access to MIDI output ports, by opening ports and
 * receiving MIDI messages.
 */
class MidiOutput
{
public:

    /**
     * Create a MIDI output.
     *
     * @param name Name for the MIDI output
     */
    MidiOutput(const std::string& name = "Midi Output Client");

    /**
     * Destroy the MIDI output.
     */
    virtual ~MidiOutput();

    /**
     * Open a MIDI output port.
     *
     * @param number Index of the port (between 0 and (numPorts()-1))
     * @param name Name to give to the opened port
     */
    void openPort(unsigned int number = 0,
                  const std::string& name = "Midi Input");

    /**
     * Open a virtual MIDI output port.
     *
     * @param name Name to give to the opened port
     */
    void openVirtualPort(const std::string& name = "RtMidi Input");

    /**
     * Close the currently opened MIDI output port (if any).
     */
    void closePort(void);

    /**
     * Check if MIDI output port is already opened.
     *
     * @return true if a port is already opened, else false
     */
    bool openedPort(void) const;

    /**
     * Inspect how many MIDI output ports are available.
     *
     * @return The number of available ports
     */
    unsigned int numPorts(void) const;

    /**
     * Find the name of a given MIDI output port.
     *
     * @param number Index of the port (between 0 and (numPorts()-1))
     * @return The name of the selected port
     */
    std::string portName(const int number) const;

    /**
     * Send a MIDI message to the opened output port.
     *
     * @param message MIDI message
     */
    void sendMessage(const MidiMessage& message);

private:

    MidiOutput(const MidiOutput&);
    MidiOutput& operator=(const MidiOutput&);

    class Private;
    std::unique_ptr<Private> mData;
};

} /* namespace launchtime */

#endif // __MIDI_OUTPUT__

