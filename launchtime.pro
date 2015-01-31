CONFIG += qt silent
QT += core gui

TARGET = launchtime
TEMPLATE = app

BUILD_DIR = build
OBJECTS_DIR = $${BUILD_DIR}/obj
MOC_DIR = $${BUILD_DIR}/moc
DESTDIR = .

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -Wall -Wshadow -O2
QMAKE_LFLAGS += -lm

macx {
    QMAKE_CXXFLAGS += -D__MACOSX_CORE__
    QMAKE_LFLAGS += -framework CoreMIDI -framework CoreFoundation -framework CoreAudio
}
unix:!macx {
    QMAKE_CXXFLAGS += -D__LINUX_ALSA__
    QMAKE_LFLAGS += -lasound -lpthread
}
win32 {
    QMAKE_CXXFLAGS += -D__WINDOWS_MM__
    QMAKE_LFLAGS += -lwinmm
}

INCLUDEPATH += \
    src \
    src/ui \
    src/app \
    src/controller \
    src/interface \
    src/midi \
    src/midi/rtmidi \

SOURCES += \
    src/main.cpp \
    src/ui/LaunchtimeWindow.cpp \
    src/app/Launchtime.cpp \
    src/controller/LaunchpadController.cpp \
    src/controller/LaunchpadSequencer.cpp \
    src/interface/HostInterface.cpp \
    src/interface/LaunchpadInterface.cpp \
    src/midi/MidiInput.cpp \
    src/midi/MidiOutput.cpp \
    src/midi/MidiMessage.cpp \
    src/midi/rtmidi/RtMidi.cpp \

HEADERS += \
    src/ui/LaunchtimeWindow.h \
