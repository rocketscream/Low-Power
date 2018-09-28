#   A Makefile that will produce a ./lib/<MCU>/liblowpower.a file per supported MCU.
#   Copyright (C) 2018  Pieter du Preez <pdupreez@gmail.com>
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <https://www.gnu.org/licenses/>.

# NOTE: All libs will be compiled with an 8MHz clock,
#       unless CPU_FREQ is set at make-time.
# eg. overriding all mcus to 16Mhz:
#       CPU_FREQ=16000000L make clean all

CPU_FREQ ?= 8000000L

# NOTE: The atsamd21g18a is not supported by this Makefile.
SUPPORTED_MCUS = \
atmega168 \
atmega328p \
atmega32u4 \
atmega2560 \
atmega256rfr2

# Default to the standard Linux tool paths.
TOOLCHAIN_PATH ?= /usr/bin

CC = $(abspath $(TOOLCHAIN_PATH))/avr-gcc
CXX = $(abspath $(TOOLCHAIN_PATH))/avr-g++
AR = $(abspath $(TOOLCHAIN_PATH))/avr-ar
SIZE = $(abspath $(TOOLCHAIN_PATH))/avr-size

TARGETS = $(addprefix lib/,$(addsuffix /liblowpower.a,$(SUPPORTED_MCUS)))
SIZES = $(addsuffix _size,$(SUPPORTED_MCUS))

# The following line is a work-around for issue #14:
atmega168_DEFINES = -DSLEEP_MODE_EXT_STANDBY=SLEEP_MODE_STANDBY

all: $(TARGETS)

%_dir:
	mkdir -p lib/$*

lib/%/liblowpower.a: lib/%/LowPower.o
	$(AR) -r $@ $<
	$(MAKE) $*_size

%_size: lib/%/liblowpower.a
	$(SIZE) --format=avr --mcu=$* lib/$*/liblowpower.a

lib/%/LowPower.o: LowPower.cpp LowPower.h %_dir
	$(CXX) -c -std=c++11 -pedantic -fverbose-asm -Werror -Os -I$(AVR_INCLUDE) -DF_CPU=$(CPU_FREQ) $($*_DEFINES) -mmcu=$* -o $@ $<

clean:
	rm -rf lib
