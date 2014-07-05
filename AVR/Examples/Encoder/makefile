NAME=EncoderExample

MCU_TARGET=atmega328p
AVRDUDE_PART=m328p

CC=avr-gcc

LIBS_DIR = $(PROJECTS_PATH)/Libs

DEL = python $(LIBS_DIR)/del.py

OPT_LEVEL=3

INCLUDE_DIRS = \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Utility

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
	
OPTS = \
	-g \
	-std=c99 \
	-Wall \
	-Wextra \
	-DF_CPU=16000000 \
	-DMEMORY_POOL_BYTES=512 \
	-DENCODER_PCINT1 \
	-DSUPPRESS_PCINT1
	
LDFLAGS =

OBJDEPS=$(CFILES:.c=.o)

all: $(NAME).elf

$(NAME).elf: $(OBJDEPS)
	$(CC) $(INCLUDE_DIRS) $(OPTS) $(LDFLAGS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -o $@ $^

%.o:%.c
	$(CC) $(INCLUDE_DIRS) $(OPTS) -O$(OPT_LEVEL) -mmcu=$(MCU_TARGET) -c $< -o $@

upload:
	avr-objcopy -R .eeprom -O ihex $(NAME).elf  $(NAME).hex
	avrdude -p$(AVRDUDE_PART) -P$(COMPORT) -carduino -b57600 -Uflash:w:$(NAME).hex:a
clean:
	$(DEL) $(NAME).elf
	$(DEL) $(OBJDEPS)
