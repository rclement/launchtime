#include "MidiMessage.h"

namespace launchtime
{

MidiMessage::MidiMessage(std::vector<unsigned char>& message) :
    mType(Unknown),
    mData(message)
{
    if (message[0] == 0x90)
    {
        mType = Voice;
    }
    else if (message[0] == 0xF0)
    {
        mType = SysEx;
    }
    else if (message[0] == 0xB0)
    {
        mType = CC;
    }
}

MidiMessage::~MidiMessage()
{
}

MidiMessage::Type MidiMessage::type() const
{
    return mType;
}

std::vector<unsigned char> MidiMessage::data() const
{
    return mData;
}

} /* namespace launchtime */

