#######################
# Compiler definition #
#######################

CC = clang
CXX = clang++

###########################
# Build config definition #
###########################

ifeq ($(CONFIG),)
	CONFIG = debug
endif

##############################
# Platform config definition #
##############################

ifeq ($(PLATFORM),)
UNAME_S := $(shell sh -c 'uname -s 2>/dev/null')
PLATFORM := unknown

ifeq ($(UNAME_S),Linux)
	PLATFORM := linux
endif

ifeq ($(UNAME_S),Darwin)
	PLATFORM := mac
endif

ifneq (,$(findstring MINGW,$(UNAME_S)))
	PLATFORM := windows
endif
endif

#######################
# Project definitions #
#######################

TARGET_NAME = launchtime

GENERIC_BUILD_DIR = build
BUILD_DIR = $(GENERIC_BUILD_DIR)/$(TARGET_NAME)/$(PLATFORM)/$(CONFIG)

HEADER_DIR = src
SOURCE_DIR = src
MODULES = app controller interface midi
MODULES_HEADER_DIR = $(addprefix $(HEADER_DIR)/, $(MODULES))
SOURCE_FILES = $(shell find $(SOURCE_DIR) -name *.cpp)
OBJECT_FILES = $(addprefix $(BUILD_DIR)/, $(SOURCE_FILES:%.cpp=%.o))

################################
# Compilation flags definition #
################################

ifeq ($(PLATFORM),mac)
	RTMIDI_CXXFLAGS += -D__MACOSX_CORE__
else ifeq ($(PLATFORM),linux)
	RTMIDI_CXXFLAGS += -D__LINUX_ALSA__
else ifeq ($(PLATFORM),windows)
	RTMIDI_CXXFLAGS += -D__WINDOWS_MM__
endif

ifeq ($(CONFIG), debug)
	CXXFLAGS += -g
endif

CXXFLAGS += -std=c++11 -stdlib=libc++ -Wall -Wshadow -O2
CXXFLAGS += -I$(HEADER_DIR)
CXXFLAGS += $(addprefix -I,$(MODULES_HEADER_DIR))
CXXFLAGS += $(RTMIDI_CXXFLAGS)

###########################
# Linker flags definition #
###########################

ifeq ($(PLATFORM),mac)
	RTMIDI_LDFLAGS += -framework CoreMIDI -framework CoreFoundation -framework CoreAudio
else ifeq ($(PLATFORM),linux)
	RTMIDI_LDFLAGS += -lasound -lpthread
else ifeq ($(PLATFORM),windows)
	RTMIDI_LDFLAGS += -lwinmm
endif

LDFLAGS += -lm
LDFLAGS += $(RTMIDI_LDFLAGS)

#####################
# Compilation rules #
#####################

.PHONY: build_info clean distclean help

all: build_info $(TARGET_NAME)

build_info:
	@echo '# Build Information'
	@echo '#   Target       : $(TARGET_NAME)'
	@echo '#   Platform     : $(PLATFORM)'
	@echo '#   Config       : $(CONFIG)'
	@echo '#   C++ compiler : $(CXX)'
	@echo '#   C++ Flags    : $(CXXFLAGS)'
	@echo '#   Linker Flags : $(LDFLAGS)'

$(TARGET_NAME): $(OBJECT_FILES)
	@echo 'linking $(notdir $@)'
	@$(CXX) $(OBJECT_FILES) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@echo 'compiling $(notdir $<)'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo 'cleaning $(TARGET_NAME)'
	@rm -rf $(BUILD_DIR) $(TARGET_NAME)

distclean:
	@echo 'cleaning everything'
	@rm -rf $(GENERIC_BUILD_DIR) $(TARGET_NAME)

help:
	@echo '# Available options:'
	@echo '#   CONFIG=[ debug | release ]'
	@echo '#   PLATFORM=[ mac | linux | windows ]'

