# Project Name
TARGET = tusenskona

ROOT_DIR = ../../..
DSP_DIR = $(ROOTDIR)/dsp-headers/dsp

# Sources
CPP_SOURCES = daisytusen.cpp $(ROOT_DIR)/src/tusenskona.cpp $(DSP_DIR)/interpolation.cpp $(DSP_DIR)/trigger.cpp $(DSP_DIR)/grain.cpp $(DSP_DIR)/granulator.cpp $(DSP_DIR)/buffer.cpp $(DSP_DIR)/envelope.cpp
#
# Library Locations
LIBDAISY_DIR = $(ROOT_DIR)/libDaisy/
DAISYSP_DIR = $(ROOT_DIR)/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
