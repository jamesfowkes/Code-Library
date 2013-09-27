# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
DEL=@rm -f
UNITY_FOLDER=E:/WorkDir/Unity

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TARGET = $(TEST_NAME)/$(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src
SYMBOLS=-DUNITY_TEST

CLEANUP = $(DEL) $(TARGET)

.PHONY: memorypool linkedlist

all: default

default:
	@echo "Available tests:"
	@echo "memorypool"
	@echo "linkedlist"

memorypool: SYMBOLS += -DMEMORY_POOL_BYTES=1024
memorypool linkedlist: TEST_NAME=$@

memorypool linkedlist:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
