include $(PROJECTS_PATH)/Libs/standard.mk

NAME=EncoderExample

MCU_TARGET=atmega328p
AVRDUDE_PART=m328p

CFILES = \
	EncoderExample.c \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/AVR/lib_encoder.c \
	$(LIBS_DIR)/AVR/lib_pcint.c \
	$(LIBS_DIR)/Generics/memorypool.c
	
EXTRA_FLAGS = \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DENCODER_PCINT1 \
	-DSUPPRESS_PCINT1

include $(PROJECTS_PATH)/make_avr.mk

