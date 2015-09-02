find_package(Qt4 REQUIRED)

set(launchtime_SOURCES
    main.cpp
    app/Launchtime.cpp
    app/Launchtime.h
    controller/LaunchpadController.cpp
    controller/LaunchpadController.h
    controller/LaunchpadSequencer.cpp
    controller/LaunchpadSequencer.h
    interface/HostInterface.cpp
    interface/HostInterface.h
    interface/LaunchpadInterface.cpp
    interface/LaunchpadInterface.h
    midi/MidiInput.cpp
    midi/MidiInput.h
    midi/MidiOutput.cpp
    midi/MidiOutput.h
    midi/MidiMessage.cpp
    midi/MidiMessage.h
    ui/LaunchtimeWindow.cpp
    ui/LaunchtimeWindow.h
)

qt_wrap_cpp(Launchtime launchtime_MOC
    ui/LaunchtimeWindow.h
)

set(exec_type "")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(exec_type "MACOSX_BUNDLE")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(exec_type "WIN32")
endif()

add_executable(Launchtime ${exec_type}
    ${launchtime_SOURCES}
    ${launchtime_MOC}
)

add_dependencies(Launchtime rtmidi)
target_link_libraries(Launchtime Qt4::QtCore Qt4::QtGui rtmidi)

target_include_directories(Launchtime PUBLIC
    ui
    app
    controller
    interface
    midi
    midi/rtmidi
)

generate_source_group("${launchtime_SOURCES}" "Sources")
generate_source_group("${launchtime_MOC}" "Generated")

# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
# install(
#     CODE "
#     include(BundleUtilities)
#     fixup_bundle(Launchtime.app \"\" \"\")
#     "
# )

