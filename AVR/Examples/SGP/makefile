NAME=SGPExample

MCU_TARGET=atmega128

CC=avr-gcc 
LD=avr-ld
SIZE=avr-size

LIBS_DIR = $(PROJECTS_PATH)/Libs
DEL = python $(LIBS_DIR)/del.py

SGP_DIR = $(LIBS_DIR)/SGP

SGP_MODEL = SGP4

SGP_FILES = \
	$(SGP_DIR)/sgp_conv.c \
	$(SGP_DIR)/sgp_deep.c \
	$(SGP_DIR)/sgp_math.c \
	$(SGP_DIR)/sgp_time.c
	
ifeq ($(SGP_MODEL), SGP0)
	SGP_FILES += $(SGP_DIR)/sgp0.c
endif

ifeq ($(SGP_MODEL), SGP4)
	SGP_FILES += $(SGP_DIR)/sgp4sdp4.c
endif

ifeq ($(SGP_MODEL), SDP4)
	SGP_FILES += $(SGP_DIR)/sgp4sdp4.c
endif

ifeq ($(SGP_MODEL), SGP8)
	SGP_FILES += $(SGP_DIR)/sgp8sdp8.c
endif

ifeq ($(SGP_MODEL), SDP8)
	SGP_FILES += $(SGP_DIR)/sgp8sdp8.c
endif

OPT_LEVEL=3

CCFLAGS = \
	-mmcu=$(MCU_TARGET) \
	-g \
	-std=c99 \
	-Wall \
	-Wextra \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DSGP_MODEL_$(SGP_MODEL) \
	-ffunction-sections \
	-fdata-sections \
	-O$(OPT_LEVEL)

LDFLAGS = \
	--gc-sections --relax

LD_SUFFIX = -lm -lc

AVR_FILES = \
	$(LIBS_DIR)/AVR/lib_clk.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(LIBS_DIR)/AVR/lib_tmr8.c \
	$(LIBS_DIR)/AVR/lib_tmr8_tick.c \
	$(LIBS_DIR)/Devices/lcd/lcd.c \
	$(LIBS_DIR)/Generics/memorypool.c
	
ALL_DEPENDENCY_FILES = main.c $(SGP_FILES) $(AVR_FILES)
ALL_DEPENDENCIES = $(ALL_DEPENDENCY_FILES:.c=.o)

INCLUDES = \
	-I$(SGP_DIR) \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Utility
	
# all
all : $(NAME).elf

# SGPExample.elf
$(NAME).elf : $(ALL_DEPENDENCIES)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $^ $(LD_SUFFIX)

%.o:%.c
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

size:
	$(SIZE) -C $(NAME).elf --mcu=$(MCU_TARGET)
clean :
	$(DEL) $(ALL_DEPENDENCIES)
