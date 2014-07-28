CC = gcc 
CCFLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS -std=c99 -Wfatal-errors $(EXTRA_FLAGS)

AVR_DIR = $(AVR32_HOME)/avr/include/avr
HARNESS_ROOT_DIR = $(PROJECTS_PATH)/Libs/AVR/Harness
HARNESS_AVR_DIR = $(HARNESS_ROOT_DIR)/AVR

INCLUDE_DIRS = \
	-I$(HARNESS_ROOT_DIR) \
	-I$(LIBS_DIR)/TestHarness \
	$(EXTRA_INCLUDE_DIRS)
	
CFILES = \
	$(HARNESS_ROOT_DIR)/lib_io_harness.c \
	$(HARNESS_ROOT_DIR)/lib_pcint_harness.c \
	$(HARNESS_ROOT_DIR)/lib_tmr8_tick_harness.c \
	$(HARNESS_ROOT_DIR)/lib_tmr16_harness.c \
	$(HARNESS_ROOT_DIR)/lib_eeprom_harness.c \
	$(LIBS_DIR)/TestHarness/test_harness.c \
	$(EXTRA_CFILES)
	
OBJDEPS=$(CFILES:.c=.o)

.PRECIOUS: $(NAME).exe

all: update_compile_time io $(OTHER_TARGETS) $(NAME).exe
	
update_compile_time:
	python $(LIBS_DIR)/compiletime.py

$(NAME).exe: $(OBJDEPS)
	$(CC) $(CCFLAGS) $(INCLUDE_DIRS) $(OPTS) $^ $(OTHER_OBJS) -o $(NAME).exe $(LINKER_SUFFIX)
	./$(NAME).exe
	
io:
	python $(HARNESS_ROOT_DIR)/generate_test_io.py $(MMCU) $(AVR_DIR) $(HARNESS_ROOT_DIR)/avr	

%.o:%.c
	$(CC) $(INCLUDE_DIRS) $(CCFLAGS) -c $< -o $@
	
clean:
	$(DEL) $(OBJDEPS) $(NAME).exe

