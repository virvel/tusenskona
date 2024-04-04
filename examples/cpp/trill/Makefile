# Project Name
TARGET = trill

ROOT_DIR = ../../..
DSP_DIR = $(ROOT_DIR)/dsp-headers/dsp

# Sources
CPP_SOURCES = trill.cpp th.cpp $(ROOT_DIR)/src/tusenskona.cpp $(DSP_DIR)/wavetable.cpp $(DSP_DIR)/interpolation.cpp

# Library Locations
LIBDAISY_DIR = $(ROOT_DIR)/libDaisy
DAISYSP_DIR = $(ROOT_DIR)/DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

CPPFLAGS += -I$(ROOT_DIR) \

						

