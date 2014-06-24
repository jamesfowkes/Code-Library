# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc

LIBS_DIR = $(PROJECTS_PATH)\Libs
DEL = python $(LIBS_DIR)\del.py

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	UNITY_FOLDER=E:/WorkDir/Unity
else
	TARGET_EXTENSION=.out
	UNITY_FOLDER=/home/james/Documents/Unity
endif

TARGET = $(TEST_NAME)/$(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES = \
	$(UNITY_FOLDER)/src/unity.c \
	../$(TEST_NAME).c \
	$(TEST_NAME)/$(TEST_NAME)_test.c \
	$(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS = \
	-I$(UNITY_FOLDER)/src \
	-I$(UNITY_FOLDER)/extras/fixture/src \
	-I$(LIBS_DIR)/Generics \
	-I$(LIBS_DIR)/Format
	
SYMBOLS = \
	-DUNITY_TEST \
	-std=c99 \
	-Wall \
	-Wextra \
	-DFORMAT_32_BIT_INTS
CLEANUP = $(DEL) $(TARGET)

.PHONY: format

all: default

default:
	@echo "Available tests:"
	@echo "format"

## Specific dependencies and defines for each test
format: TEST_NAME=$@

format:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
