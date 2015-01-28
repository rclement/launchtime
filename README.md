# Description

**Launchtime** is a multi-platform application allowing the Novation Launchpad
hardware to be used as standalone a step-sequencer synchronized with any
MIDI-enabled host software (DAW).

# Compilation

`$ make CONFIG=[debug|release] PLATFORM=[macosx|linux|windows]`

# Usage

`$ ./launchtime`

# Notes

Launchtime uses the RtMidi library to perform MIDI I/O operations, and most
notably opening virtual MIDI ports to communication with MIDI hosts.
For now, only the MacOSX and Linux platforms support this feature.

# Contact

Any questions or comments about Launchtime? Let me know at [contact@romainclement.com](mailto:contact@romainclement.com).

# License

The MIT License (MIT)

Copyright (c) 2014 Romain Clement
