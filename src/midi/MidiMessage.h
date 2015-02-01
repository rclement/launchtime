#ifndef __MIDI_MESSAGE__
#define __MIDI_MESSAGE__ 1

#include <vector>
#include <string>

namespace launchtime
{

class MidiMessage
{
public:

    enum Type {
        Voice = 0x90,
        SysEx = 0xF0,
        CC = 0xB0,
        Unknown = 0xFF
    };

    MidiMessage(std::vector<unsigned char>& message);
    virtual ~MidiMessage();

    MidiMessage::Type type() const;
    std::vector<unsigned char> data() const;

protected:

    MidiMessage::Type mType;
    std::vector<unsigned char> mData;
};

} /* namespace launchtime */

#endif // __MIDI_MESSAGE__
