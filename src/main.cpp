#include "Launchtime.h"
#include "MidiInput.h"
#include "MidiOutput.h"
#include <string>
#include <iostream>

#define USAGE_STRING \
    "Usage\n" \
    "   launchtime [options]\n" \
    "\n" \
    "Options\n" \
    "   -l : list all available MIDI devices\n" \
    "   -d <device> : name of the launchpad device (default: Launchpad)\n" \
    "   -p <port>   : name of the virtual port (default: Launchtime)\n"

static void printUsage()
{
    std::cout << USAGE_STRING;
}

static void listAllMidiDevices()
{
    launchtime::MidiInput in;
    launchtime::MidiOutput out;

    std::cout << in.numPorts() << " Midi input ports" << std::endl;
    for (unsigned int i = 0; i < in.numPorts(); i++)
    {
        std::cout << "  " << i << " : " << in.portName(i) << std::endl;
    }

    std::cout << out.numPorts() << " Midi output ports" << std::endl;
    for (unsigned int i = 0; i < out.numPorts(); i++)
    {
        std::cout << "  " << i << " : " << in.portName(i) << std::endl;
    }
}

static void runLaunchtimeApp(const std::string& devicename, const std::string& portname)
{
    launchtime::Launchtime app(devicename, portname);

    std::string command;
    while (command != "exit")
    {
        std::cout << "Type 'exit' to quit Launchtime" << std::endl;

        std::cin >> command;
    }
}

int main(int argc, char **argv)
{
    bool shouldexit = false;
    bool shouldprintusage = false;

    std::string devicename("Launchpad");
    std::string portname("Launchtime");

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            std::string argstr(argv[i]);

            if (argstr == "-l")
            {
                listAllMidiDevices();
                shouldexit = true;
                shouldprintusage = false;
            }
            else if (argstr == "-d")
            {
                i++;

                if (i < argc)
                {
                    devicename = argv[i];
                }
                else
                {
                    shouldexit = true;
                    shouldprintusage = true;
                }
            }
            else if (argstr == "-p")
            {
                i++;

                if (i < argc)
                {
                    portname = argv[i];
                }
                else
                {
                    shouldexit = true;
                    shouldprintusage = true;
                }
            }
            else
            {
                shouldexit = true;
                shouldprintusage = true;
            }
        }
    }

    if (!shouldexit)
    {
        runLaunchtimeApp(devicename, portname);
    }

    if (shouldprintusage)
    {
        printUsage();
    }

    return 0;
}

