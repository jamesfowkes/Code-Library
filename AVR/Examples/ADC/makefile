include $(PROJECTS_PATH)/Libs/standard.mk

NAME=ADCExample

MCU_TARGET=atmega328p
AVRDUDE_PART=m328p

CFILES = \
	ADCExample.c \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/AVR/lib_adc.c \
	$(LIBS_DIR)/Generics/memorypool.c \

EXTRA_FLAGS = \
	-DMEMORY_POOL_BYTES=128 \
	-DF_CPU=16000000

include $(PROJECTS_PATH)/make_avr.mk

