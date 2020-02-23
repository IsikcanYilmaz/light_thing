
ARDUINO_DIR = /home/jon/PROGRAMS/arduino-1.8.12/
CC = "/usr/bin/avr-gcc"
CPP = "/usr/bin/avr-g++"
AR = "/usr/bin/avr-ar"
OBJ_COPY = "/usr/bin/avr-objcopy"

MAIN_SKETCH = main.cpp

F_CPU = 16000000
MCU = atmega328p

# compile flags
GENERAL_FLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=$(F_CPU)L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=106
CPP_FLAGS = $(GENERAL_FLAGS) -fno-exceptions
CC_FLAGS  = $(GENERAL_FLAGS)

# location of include files
INCLUDE_FILES = "-I$(ARDUINO_DIR)hardware/arduino/cores/arduino" "-I$(ARDUINO_DIR)hardware/arduino/avr/variants/standard" "-I$(ARDUINO_DIR)hardware/arduino/avr/cores/arduino/"

# library sources
LIBRARY_DIR = "$(ARDUINO_DIR)hardware/arduino/cores/arduino/"

build:
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(MAIN_SKETCH) -o $(MAIN_SKETCH).o

all: build
