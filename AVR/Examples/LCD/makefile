include $(PROJECTS_PATH)/Libs/standard.mk

NAME=LCDExample

MCU_TARGET=atmega328p
AVRDUDE_PART=m328p

CFILES = \
	LCDExample.c \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/Devices/lcd/lcd.c \
	$(LIBS_DIR)/Generics/memorypool.c \
	$(LIBS_DIR)/Format/format.c
	
EXTRA_FLAGS = \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512

include $(LIBS_DIR)/AVR/make_avr.mk

