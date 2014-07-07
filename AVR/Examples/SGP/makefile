include $(PROJECTS_PATH)/Libs/standard.mk

NAME=SGPExample

MCU_TARGET=atmega128
AVRDUDE_PART=m128

LD=avr-ld
SIZE=avr-size

SGP_DIR = $(LIBS_DIR)/SGP

EXTRA_FLAGS = \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DSGP_MODEL_$(SGP_MODEL) \
	-O$(OPT_LEVEL)

AVR_FILES = \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/Devices/lcd/lcd.c \
	$(LIBS_DIR)/Generics/memorypool.c
	
CFILES = main.c $(SGP_FILES) $(AVR_FILES)

EXTRA_INCLUDES = \
	-I$(SGP_DIR)
	
LD_SUFFIX = -lm -lc

OTHER_OBJS = $(SGP_OBJDEPS)

include $(PROJECTS_PATH)/make_avr.mk
include $(SGP_DIR)/sgp.mk

