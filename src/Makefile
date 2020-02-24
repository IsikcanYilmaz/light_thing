
ARDUINO_DIR = /home/jon/PROGRAMS/arduino-1.8.12/
CC = "/usr/bin/avr-gcc"
CPP = "/usr/bin/avr-g++"
AR = "/usr/bin/avr-ar"
OBJ_COPY = "/usr/bin/avr-objcopy"

MAIN_SKETCH = main_app.cpp

F_CPU = 16000000
MCU = atmega328p

# compile flags
GENERAL_FLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=$(F_CPU)L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=106
CPP_FLAGS = $(GENERAL_FLAGS) -fno-exceptions
CC_FLAGS  = $(GENERAL_FLAGS)

# location of include files
INCLUDE_FILES = "-I$(ARDUINO_DIR)hardware/arduino/cores/arduino" "-I$(ARDUINO_DIR)hardware/arduino/avr/variants/standard" "-I$(ARDUINO_DIR)hardware/arduino/avr/cores/arduino/"

# library sources
LIBRARY_DIR = "$(ARDUINO_DIR)hardware/arduino/avr/cores/arduino/"

build:
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(MAIN_SKETCH) -o $(MAIN_SKETCH).o
	#$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/malloc.c -o malloc.c.o
	#$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)avr-libc/realloc.c -o realloc.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WInterrupts.c -o WInterrupts.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring.c -o wiring.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_analog.c -o wiring_analog.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_digital.c -o wiring_digital.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_pulse.c -o wiring_pulse.c.o
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)wiring_shift.c -o wiring_shift.c.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)CDC.cpp -o CDC.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial.cpp -o HardwareSerial.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial0.cpp -o HardwareSerial0.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial1.cpp -o HardwareSerial1.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial2.cpp -o HardwareSerial2.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)HardwareSerial3.cpp -o HardwareSerial3.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)../../libraries/HID/src/HID.cpp -o HID.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)IPAddress.cpp -o IPAddress.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)main.cpp -o main.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)new.cpp -o new.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Print.cpp -o Print.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Stream.cpp -o Stream.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)Tone.cpp -o Tone.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)USBCore.cpp -o USBCore.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)PluggableUSB.cpp -o PluggableUSB.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WMath.cpp -o WMath.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)WString.cpp -o WString.cpp.o
	$(CC) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)hooks.c -o hooks.cpp.o
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $(LIBRARY_DIR)abi.cpp -o abi.cpp.o

all: build
