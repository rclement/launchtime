#include "LaunchpadSequencer.h"
#include <iostream>

namespace launchtime
{

LaunchpadSequencer::LaunchpadSequencer() :
    LaunchpadController("Launchpad Sequencer"),
    m_Running(false),
    m_LinesCount(1),
    m_MidiClockCount(0),
    m_MidiClockCountBar(0),
    m_MidiClockSteps(24),
    m_MidiClockNewStepsPending(false),
    m_MidiClockNewSteps(m_MidiClockSteps),
    m_MidiChannel(0),
    m_CurrPosX(0),
    m_CurrPosY(0),
    m_PrevPosX(0),
    m_PrevPosY(0)
{
    for (int i = 0; i < 8; i++) {
        m_Muted[i] = LaunchpadPadState::Disabled;
    }

    m_MidiNotes[0] = 0x24;
    m_MidiNotes[1] = 0x25;
    m_MidiNotes[2] = 0x26;
    m_MidiNotes[3] = 0x27;
    m_MidiNotes[4] = 0x28;
    m_MidiNotes[5] = 0x29;
    m_MidiNotes[6] = 0x2A;
    m_MidiNotes[7] = 0x2B;
}

LaunchpadSequencer::~LaunchpadSequencer()
{
}

void LaunchpadSequencer::setMidiChannel(const int channel)
{
    if (channel >= 0 && channel < 16) {
        m_MidiChannel = channel;
    }
}

void LaunchpadSequencer::updateGridLEDs()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (mGrid[i][j].pushState() != LaunchpadPadState::Disabled) {
                turnOnPadLED(i, j, LaunchpadInterface::ColorFullAmber);
            } else {
                turnOffPadLED(i, j);
            }
        }
    }

    if (! m_Running) {
        for (int i = 0; i < 8; i += 8/m_LinesCount) {
            for (int j = 0; j < 8; j++) {
                if (mGrid[j][i].pushState() == LaunchpadPadState::Disabled) {
                    turnOnPadLED(j, i, LaunchpadInterface::ColorFullRed);
                }
            }
        }
    }
}

void LaunchpadSequencer::updateBankLEDs()
{
    for (int i = 0; i < 8; i++) {
        if (m_Muted[i] == LaunchpadPadState::Disabled) {
            turnOnBankLED(i, LaunchpadInterface::ColorFullYellow);
        } else {
            turnOffBankLED(i);
        }
    }
}

void LaunchpadSequencer::updateControlsLEDs()
{
    if (m_MidiClockSteps < 96) {
        turnOnControlLED(0, LaunchpadInterface::ColorFullGreen);
    } else {
        turnOnControlLED(0, LaunchpadInterface::ColorDimGreen);
    }

    if (m_MidiClockSteps > 3) {
        turnOnControlLED(1, LaunchpadInterface::ColorFullGreen);
    } else {
        turnOnControlLED(1, LaunchpadInterface::ColorDimGreen);
    }

    if (m_LinesCount > 1) {
        turnOnControlLED(2, LaunchpadInterface::ColorFullGreen);
    } else {
        turnOnControlLED(2, LaunchpadInterface::ColorDimGreen);
    }

    if (m_LinesCount < 8) {
        turnOnControlLED(3, LaunchpadInterface::ColorFullGreen);
    } else {
        turnOnControlLED(3, LaunchpadInterface::ColorDimGreen);
    }

    turnOnControlLED(7, LaunchpadInterface::ColorFullOrange);
}

void LaunchpadSequencer::eventPadPressed(const int x, const int y)
{
    if (mGrid[x][y].pushState() == LaunchpadPadState::Disabled) {
        mGrid[x][y].setPushState(LaunchpadPadState::Enabling);
        turnOnPadLED(x, y, LaunchpadInterface::ColorFullAmber);
    } else if (mGrid[x][y].pushState() == LaunchpadPadState::Enabled) {
        mGrid[x][y].setPushState(LaunchpadPadState::Disabling);
    }
}

void LaunchpadSequencer::eventPadReleased(const int x, const int y)
{
    if (mGrid[x][y].pushState() == LaunchpadPadState::Enabling) {
        mGrid[x][y].setPushState(LaunchpadPadState::Enabled);
    } else if (mGrid[x][y].pushState() == LaunchpadPadState::Disabling) {
        mGrid[x][y].setPushState(LaunchpadPadState::Disabled);
        turnOffPadLED(x, y);
    }
}

void LaunchpadSequencer::eventBankPressed(const int id)
{
    if (m_Muted[id] == LaunchpadPadState::Disabled) {
        m_Muted[id] = LaunchpadPadState::Enabling;
    } else if (m_Muted[id] == LaunchpadPadState::Enabled) {
        m_Muted[id] = LaunchpadPadState::Disabling;
        turnOnBankLED(id, LaunchpadInterface::ColorFullYellow);
    }
}

void LaunchpadSequencer::eventBankReleased(const int id)
{
    if (m_Muted[id] == LaunchpadPadState::Enabling) {
        m_Muted[id] = LaunchpadPadState::Enabled;
        turnOffBankLED(id);
    } else if (m_Muted[id] == LaunchpadPadState::Disabling) {
        m_Muted[id] = LaunchpadPadState::Disabled;
    }
}

void LaunchpadSequencer::eventControlPressed(const int id)
{
    if (id == 0 && m_MidiClockNewSteps < 96) {
        m_MidiClockNewSteps = m_MidiClockSteps << 1;
        m_MidiClockNewStepsPending = true;
        turnOnControlLED(id, LaunchpadInterface::ColorDimAmber);
    } else if (id == 1 && m_MidiClockNewSteps > 3) {
        m_MidiClockNewSteps = m_MidiClockSteps >> 1;
        m_MidiClockNewStepsPending = true;
        turnOnControlLED(id, LaunchpadInterface::ColorDimAmber);
    } else if (id == 2 && m_LinesCount > 1 && ! m_Running) {
        m_LinesCount /= 2;
        updateGridLEDs();
        updateControlsLEDs();
    } else if (id == 3 && m_LinesCount < 8 && ! m_Running) {
        m_LinesCount *= 2;
        updateGridLEDs();
        updateControlsLEDs();
    } else if (id == 7) {
        resetGrid();
    }
}

void LaunchpadSequencer::eventControlReleased(const int id)
{
}

void LaunchpadSequencer::eventMidiMessage(const MidiMessage& message)
{
    if (message.data().at(0) == 0xFA) {
        m_Running = true;

        m_MidiClockCount = 1;
        m_MidiClockCountBar = m_MidiClockSteps;
        m_CurrPosX = 0;
        m_PrevPosX = 0;
        m_CurrPosY = 0;
        m_PrevPosY = 0;

        updateGridLEDs();
        updateCurrentBeat();
        sendMidiNotes();
    } else if (message.data().at(0) == 0xF8 || message.data().at(0) == 0xFB) {
        if (m_MidiClockCount == m_MidiClockSteps) {
            m_MidiClockCount = 0;

            if (m_MidiClockCountBar == 96) {
                m_MidiClockCountBar = 0;

                if (m_MidiClockNewStepsPending) {
                    m_MidiClockNewStepsPending = false;
                    m_MidiClockSteps = m_MidiClockNewSteps;
                }

                updateControlsLEDs();
            }

            m_MidiClockCountBar += m_MidiClockSteps;

            m_PrevPosX = m_CurrPosX;
            m_CurrPosX = (m_CurrPosX + 1) % 8;

            if (m_CurrPosX == 0) {
                m_PrevPosY = m_CurrPosY;
                m_CurrPosY = (m_CurrPosY + 8/m_LinesCount) % 8;
            } else if (m_CurrPosX == 1) {
                m_PrevPosY = m_CurrPosY;
            }

            updateCurrentBeat();
            sendMidiNotes();
        }

        m_MidiClockCount++;
    } else if (message.data().at(0) == 0xFC) {
        m_Running = false;

        updateGridLEDs();
    }
}

void LaunchpadSequencer::updateCurrentBeat()
{
    turnOffPadLED(m_PrevPosX, m_PrevPosY);

    if (mGrid[m_PrevPosX][m_PrevPosY].pushState() != LaunchpadPadState::Disabled) {
        turnOnPadLED(m_PrevPosX, m_PrevPosY, LaunchpadInterface::ColorFullAmber);
    }

    turnOnPadLED(m_CurrPosX, m_CurrPosY, LaunchpadInterface::ColorFullRed);
}

void LaunchpadSequencer::sendMidiNotes()
{
    std::vector<unsigned char> msg;

    msg.push_back(0x90 + m_MidiChannel);
    msg.push_back(0x00);
    msg.push_back(0x7F);

    const int start_line = m_CurrPosY;
    const int end_line = start_line + 8 / m_LinesCount;

    for (int i = start_line, m = 0; i < end_line; i++, m++) {
        if (m_Muted[i] == LaunchpadPadState::Disabled) {
            msg[1] = m_MidiNotes[m];

            if (mGrid[m_CurrPosX][i].pushState() == LaunchpadPadState::Disabled) {
                msg[0] = 0x80;
                msg[2] = 0x00;
            } else {
                msg[0] = 0x90;
                msg[2] = 0x7F;
            }

            sendMidiMessage(& msg);
        }
    }
}

void LaunchpadSequencer::resetGrid()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mGrid[i][j].setPushState(LaunchpadPadState::Disabled);
        }
    }

    updateGridLEDs();
    updateBankLEDs();
    updateControlsLEDs();
}

} /* namespace launchtime */

