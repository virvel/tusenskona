USE_FATFS=1
# Project Name

TARGET = granular

ROOT_DIR = ../../..
VIRVEL_DIR = ../virveldsp/src

# Sources
CPP_SOURCES = granular.cpp $(ROOT_DIR)/src/tusenskona.cpp $(VIRVEL_DIR)/granulator.cpp

# Library Locations
LIBDAISY_DIR = $(ROOT_DIR)/libDaisy/
DAISYSP_DIR = $(ROOT_DIR)/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

CPPFLAGS += -I$(ROOT_DIR)/src \
			-I$(VIRVEL_DIR)
