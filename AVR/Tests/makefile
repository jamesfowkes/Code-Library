# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
DEL=@rm -f

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	UNITY_FOLDER=E:/WorkDir/Unity
else
	TARGET_EXTENSION=.out
	UNITY_FOLDER=/home/james/Documents/Unity
endif

LIBS_DIR = $(PROJECTS_PATH)/Libs

TARGET = $(TEST_NAME)/$(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../lib_$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src

INC_DIRS+= \
	-I$(LIBS_DIR)/AVR \
	-I$(LIBS_DIR)/AVR/Harness \
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Devices \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Protocols \
	-I$(LIBS_DIR)/Utility \
	
SYMBOLS=-DUNITY_TEST -std=c99 -Wall -Wextra

CLEANUP = $(DEL) $(TARGET)

.PHONY: swserial

all: default

default:
	@echo "Available tests:"
	@echo "swserial"

## Specific dependencies and defines for each test
swserial: SYMBOLS += -DTEST_HARNESS -DTX_BUFFER_SIZE=32
swserial: SRC_FILES+= ../lib_fuses.c ../lib_io.c
swserial: TEST_NAME=$@

swserial:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
