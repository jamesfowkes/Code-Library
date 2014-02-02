# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
UNITY_FOLDER=C:/Unity
DEL=@rm -f

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TARGET = $(TEST_NAME)\$(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src
SYMBOLS=-DUNITY_TEST

CLEANUP = $(DEL) $(TARGET)

.PHONY: LLAP

all: default

default:
	@echo "Available tests:"
	@echo "LLAP"

## Specific dependencies and defines for each test
#LLAP: #No dependencies

LLAP: TEST_NAME=$@

LLAP:
	$(CLEANUP)
	$(C_COMPILER) -Wall -Wextra $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
