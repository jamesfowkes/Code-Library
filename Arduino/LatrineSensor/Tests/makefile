# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=g++

LIBS_DIR = $(PROJECTS_PATH)/Code-Library
DEL = python "$(LIBS_DIR)/del.py"

TARGET = $(TEST_NAME)/$(TEST_NAME)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS= -I../

SYMBOLS=-DUNITY_TEST -std=c99 -Wall -Wextra

CLEANUP = $(DEL) $(TARGET)

.PHONY: latrinesensor

all: default

default:
	@echo "Available tests:"
	@echo "latrinesensor"

## Specific dependencies and defines for each test
latrinesensor: TEST_NAME=$@

latrinesensor:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
