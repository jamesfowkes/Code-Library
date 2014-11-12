# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=g++

LIBS_DIR = $(PROJECTS_PATH)/Code-Library
DEL = python "$(LIBS_DIR)/del.py"

TARGET = $(TEST_NAME)/$(TEST_NAME)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c $(TEST_NAME)/$(TEST_NAME).cpp $(TEST_NAME)/Tests/$(TEST_NAME)_test.cpp $(TEST_NAME)/Tests/$(TEST_NAME)_test_Runner.c

INC_DIRS= -I$(TEST_NAME)/ -I$(UNITY_FOLDER)/src

SYMBOLS=-DUNITY_TEST -Wall -Wextra

CLEANUP = $(DEL) $(TARGET)

.PHONY: LatrineSensor

all: default

default:
	@echo "Available tests:"
	@echo "LatrineSensor"

## Specific dependencies and defines for each test
LatrineSensor: TEST_NAME=$@

LatrineSensor:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
