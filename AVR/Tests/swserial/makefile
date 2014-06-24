# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

TEST_NAME=swserial

C_COMPILER=gcc
DEL=python $(LIBS_DIR)/del.py

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	UNITY_FOLDER=E:/WorkDir/Unity
else
	TARGET_EXTENSION=.out
	UNITY_FOLDER=/home/james/Documents/Unity
endif

MMCU = iom328p

LIBS_DIR = $(PROJECTS_PATH)/Libs

AVR_DIR = $(AVR32_HOME)\avr\include\avr
HARNESS_DIR = $(PROJECTS_PATH)\Libs\AVR\Harness\avr

TARGET = $(TEST_NAME)$(TARGET_EXTENSION)

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src

MAKE_AVR_IO = python $(HARNESS_DIR)/../generate_test_io.py $(MMCU) $(AVR_DIR) $(HARNESS_DIR)

SRC_FILES+= \
	swserial_test.c \
	swserial_test_Runner.c \
	$(UNITY_FOLDER)/src/unity.c \
	$(LIBS_DIR)/AVR/lib_swserial.c \
	$(LIBS_DIR)/Format/format.c \
	$(LIBS_DIR)/AVR/lib_fuses.c \
	$(LIBS_DIR)/AVR/lib_io.c \
	$(HARNESS_DIR)/../lib_io_harness.c
	
INC_DIRS+= \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/AVR/Harness \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Protocols \
	-I$(LIBS_DIR)/Utility \
	-I$(LIBS_DIR)/Format
	
SYMBOLS=-DUNITY_TEST -std=c99 -Wall -Wextra -DTEST_HARNESS -DTX_BUFFER_SIZE=32

OBJDEPS=$(SRC_FILES:.c=.o)

all: clean
	$(CLEANUP)
	$(MAKE_AVR_IO)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(DEL) $(TARGET) $(OBJDEPS)
