set(options "")
set(libraries "")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(core-midi CoreMIDI)
    find_library(core-foundation CoreFoundation)
    find_library(core-audio CoreAudio)

    set(options "-D__MACOSX_CORE__")
    set(libraries ${core-midi} ${core-foundation} ${core-audio})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_library(win-mm winmm)

    set(options "-D__WINDOWS_MM__")
    set(libraries ${win-mm})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_library(libasound asound)
    find_library(libpthread pthread)

    set(options "-D__LINUX_ALSA__")
    set(libraries ${libasound} ${libpthread})
endif()

set(rtmidi_SOURCES
    RtMidi.cpp
    RtMidi.h
)

add_library(rtmidi STATIC ${rtmidi_SOURCES})
target_include_directories(rtmidi PUBLIC .)
target_compile_options(rtmidi PUBLIC ${options})
target_link_libraries(rtmidi PUBLIC ${libraries})

