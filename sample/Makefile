# MCU configuration
MCU = atmega328p
FCPU = 16000000

# Programmer configuration
PROGRAMMER = arduino
BAUD = 115200
PORT = /dev/ttyACM0

# Host toolchain.
# It's not used but maybe it would be useful one day.
HOSTCC = gcc
HOSTCFLAGS = -O2 -Wall -Wextra
HOSTCXX = g++
HOSTCXXFLAGS = -O2 -Wall -Wextra
RM = rm # Windows user can modify this to 'del'

# AVR toolchain
OPTFLAGS = -Os -Wall -Wextra -mmcu=$(MCU) -DF_CPU=$(FCPU)ul
CC = avr-gcc
CFLAGS = -std=c99 $(OPTFLAGS)
CXX = avr-g++
CXXFLAGS = -std=c++03 $(OPTFLAGS)
AS = avr-as
AR = avr-ar
LD = avr-ld
OBJCOPY = avr-objcopy
AVRDUDE = sudo avrdude # Need root permission to operate serial ports
AVRDUDEFLAGS = -p $(MCU) -c $(PROGRAMMER) -b $(BAUD) -P $(PORT) -s -v

# Useful objects
all : main.hex main.eep

main.hex : main.elf
	$(OBJCOPY) -O ihex -R .eeprom main.elf main.hex

main.eep : main.elf
	$(OBJCOPY) -O ihex -j .eeprom main.elf main.eep

install : main.hex main.eep
	$(AVRDUDE) $(AVRDUDEFLAGS) -D -U flash:w:main.hex -U eeprom:w:main.eep

erase :
	$(AVRDUDE) $(AVRDUDEFLAGS) -e

# Depending
CFILE = $(wildcard *.c)
CXXFILE = $(wildcard *.cpp)
OBJ = $(patsubst %.c, %.o, $(CFILE)) \
$(patsubst %.cpp, %.o, $(CXXFILE))
CDEP = $(patsubst %.c, %.dep, $(CFILE))
CXXDEP = $(patsubst %.cpp, %.depxx, $(CXXFILE))

main.elf : $(OBJ) $(OBJXX)
	$(CC) $(CFLAGS) -o $@ $^
	chmod -x main.elf # Why we can execute AVR programs on UNIX ?

sinclude $(CDEP) $(CXXDEP)

%.dep : %.c
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.depxx : %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

.PHONY : clean
clean:
	$(RM) -f main.elf main.hex main.eep $(OBJ) $(CDEP) $(CXXDEP)
