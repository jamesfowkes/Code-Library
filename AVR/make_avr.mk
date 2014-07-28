CC=avr-gcc

OPT_LEVEL=3

INCLUDE_DIRS = \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Format \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Protocols \
	-I$(LIBS_DIR)/Utility \
	$(EXTRA_INCLUDES)

OPTS = \
	-g \
	-Wall \
	-Wextra \
	-std=c99 \
	-ffunction-sections \
	-fdata-sections \
	-O$(OPT_LEVEL) \
	-mmcu=$(MCU_TARGET) \
	$(EXTRA_FLAGS)

ALL_OPTS = \
	$(OPTS) \
	-Werror \
	-Wfatal-errors \

OBJDEPS=$(CFILES:.c=.o)

LDFLAGS = \
	-Wl,--gc-sections,--relax

all: clean-output update_compile_time $(OTHER_TARGETS) $(NAME).elf $(NAME).hex

update_compile_time:
	python $(LIBS_DIR)/compiletime.py

$(NAME).elf: $(OBJDEPS)
	$(CC) $(INCLUDE_DIRS) $(ALL_OPTS) $(LDFLAGS) -o $@ $^ $(OTHER_OBJS) $(LD_SUFFIX)

%.o:%.c
	$(CC) $(INCLUDE_DIRS) $(ALL_OPTS) -c $< -o $@

$(NAME).hex:
	avr-objcopy -R .eeprom -O ihex $(NAME).elf  $(NAME).hex

upload-arduino: $(NAME).hex
	avrdude -p$(AVRDUDE_PART) -P$(COMPORT) -carduino -b57600 -Uflash:w:$(NAME).hex:a

upload-isp: $(NAME).hex
	avrdude -p $(AVRDUDE_PART) -c usbtiny -Uflash:w:$(NAME).hex:a

reset-isp:
	avrdude -p $(AVRDUDE_PART) -c usbtiny
	
size:
	$(SIZE) -C $(NAME).elf --mcu=$(MCU_TARGET)

clean-output:
	$(DEL) $(NAME).elf
	$(DEL) $(NAME).hex

clean: clean-output
	$(DEL) $(OBJDEPS)
