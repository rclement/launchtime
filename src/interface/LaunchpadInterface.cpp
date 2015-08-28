#include "LaunchpadInterface.h"
#include <iostream>

namespace launchtime
{

LaunchpadInterface::LaunchpadInterface() :
    mInput("Launchpad Interface Input"),
    mOutput("Launchpad Interface Output"),
    mDeviceName(),
    mConnected(false),
    mListeners()
{
}

LaunchpadInterface::~LaunchpadInterface()
{
    disconnect();
    mListeners.clear();
}

bool LaunchpadInterface::connect(const std::string& devicename)
{
    int input_port = -1;
    int output_port = -1;

    if (mConnected)
    {
        disconnect();
    }

    for (unsigned int i = 0; i < mInput.numPorts(); i++)
    {
        if (mInput.portName(i).compare(devicename) == 0)
        {
            std::cout << devicename << " found on input port " << i << std::endl;
            input_port = i;
            break;
        }
    }

    for (unsigned int i = 0; i < mOutput.numPorts(); i++)
    {
        if (mOutput.portName(i).compare(devicename) == 0)
        {
            std::cout << devicename << " found on output port " << i << std::endl;
            output_port = i;
            break;
        }
    }

    if (input_port >= 0 && output_port >= 0)
    {
        mInput.openPort(input_port, "Launchpad Input");
        mOutput.openPort(output_port, "Launchpad Output");

        mInput.addListener(this);

        mConnected = true;
        mDeviceName = devicename;

        std::cout << devicename << ": MIDI I/O connected" << std::endl;
    }
    else
    {
        mConnected = false;

        std::cout << devicename << ": cannot connect MIDI I/O" << std::endl;
    }

    return mConnected;
}

void LaunchpadInterface::disconnect()
{
    if (mConnected)
    {
        mInput.closePort();
        mOutput.closePort();
        mConnected = false;

        std::cout << mDeviceName << ": MIDI I/O disconnected" << std::endl;
    }
}

void LaunchpadInterface::addListener(Listener* listener)
{
    mListeners.push_back(listener);
}

void LaunchpadInterface::removeListener(Listener* listener)
{
    mListeners.remove(listener);
}

void LaunchpadInterface::turnOnAllLEDs()
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::CC);
    data.push_back(0x00);
    data.push_back(0x7F);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::turnOffAllLEDs()
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::CC);
    data.push_back(0x00);
    data.push_back(0x00);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::turnOnPadLED(const int x, const int y,
                                      const LaunchpadInterface::Color color)
{
    const char pad = y * 16 + x;

    std::vector<unsigned char> data;

    data.push_back(MidiMessage::Voice);
    data.push_back(pad);
    data.push_back((char) color);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::turnOffPadLED(const int x, const int y)
{
    turnOnPadLED(x, y, LaunchpadInterface::ColorOff);
}

void LaunchpadInterface::turnOnBankLED(const int id,
                                       const LaunchpadInterface::Color color)
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::Voice);
    data.push_back(id * 16 + 8);
    data.push_back(color);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::turnOffBankLED(const int id)
{
    turnOnBankLED(id, LaunchpadInterface::ColorOff);
}

void LaunchpadInterface::turnOnControlLED(const int id,
        const LaunchpadInterface::Color color)
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::CC);
    data.push_back(id + 0x68);
    data.push_back(color);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::turnOffControlLED(const int id)
{
    turnOnControlLED(id, LaunchpadInterface::ColorOff);
}

void LaunchpadInterface::enableXYLayout()
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::CC);
    data.push_back(0x00);
    data.push_back(0x01);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::enableDrumRackLayout()
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::CC);
    data.push_back(0x00);
    data.push_back(0x02);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::displayMessage(const std::string & message)
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::SysEx);
    data.push_back(0x00);
    data.push_back(0x20);
    data.push_back(0x29);
    data.push_back(0x09);

    data.push_back(0x7C);

    data.push_back(0x04);

    for (unsigned int i = 0; i < message.size(); i++)
    {
        data.push_back(message[i]);
    }

    data.push_back(0xF7);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::stopDisplayingMessage()
{
    std::vector<unsigned char> data;

    data.push_back(MidiMessage::SysEx);
    data.push_back(0x00);
    data.push_back(0x20);
    data.push_back(0x29);
    data.push_back(0x09);
    data.push_back(0x00);
    data.push_back(0xF7);

    MidiMessage msg(data);

    mOutput.sendMessage(msg);
}

void LaunchpadInterface::midiMessage(double timestamp,
                                     const MidiMessage& message)

{
    // std::cout << mDeviceName << " : MIDI input message" << std::endl;

    MidiMessage::Type type = message.type();

    if (type == MidiMessage::Voice)
    {
        // std::cout << "    Voice"<< std::endl;

        const int pad = message.data().at(1);
        const int y = pad / 16;
        const int x = pad - (y * 16);
        const bool on = (((int) message.data().at(2)) == 0x7F);

        if ((x / 8) == 1)
        {
            for (std::list<Listener*>::iterator it = mListeners.begin();
                 it != mListeners.end();
                 it++)
            {
                (*it)->bankEvent(this, y, on);
            }
        }
        else
        {
            for (std::list<Listener*>::iterator it = mListeners.begin();
                 it != mListeners.end();
                 it++)
            {
                (*it)->padEvent(this, x, y, on);
            }
        }
    }
    else if (type == MidiMessage::CC)
    {
        // std::cout << "    CC"<< std::endl;

        const int id = message.data().at(1) - 0x68;
        const bool on = (((int) message.data().at(2)) == 0x7F);

        for (std::list<Listener*>::iterator it = mListeners.begin();
             it != mListeners.end();
             it++)
        {
            (*it)->controlEvent(this, id, on);
        }
    }
    else if (type == MidiMessage::SysEx)
    {
        // std::cout << "    SysEx"<< std::endl;
    }
    else
    {
        // std::cout << "    Unknown"<< std::endl;
    }
}

} /* namespace launchtime */

