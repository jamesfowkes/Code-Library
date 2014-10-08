# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc

LIBS_DIR = $(PROJECTS_PATH)/Libs
DEL = python $(LIBS_DIR)/del.py

TARGET = $(TEST_NAME)/$(TEST_NAME)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS= -I$(LIBS_DIR)/Generics -I$(LIBS_DIR)/Utility -I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src

SYMBOLS=-DUNITY_TEST -std=c99 -Wall -Wextra

CLEANUP = $(DEL) $(TARGET)

.PHONY: memorypool linkedlist statemachine ringbuf averager button

all: default

default:
	@echo "Available tests:"
	@echo "memorypool"
	@echo "linkedlist"
	@echo "statemachine"
	@echo "ringbuf"
	@echo "averager"
	@echo "button"

## Specific dependencies and defines for each test
memorypool: SYMBOLS += -DMEMORY_POOL_BYTES=1024 -DTEST_HARNESS

statemachine: SYMBOLS += -DMEMORY_POOL_BYTES=1024
statemachine: SRC_FILES += ../statemachinemanager.c
statemachine: SRC_FILES += ../ringbuf.c
statemachine: SRC_FILES += ../memorypool.c

averager: SYMBOLS += -DMEMORY_POOL_BYTES=1024 -DINCLUDE_32BIT_AVERAGER
averager: SRC_FILES += ../memorypool.c

memorypool linkedlist statemachine ringbuf averager button: TEST_NAME=$@

memorypool linkedlist statemachine ringbuf averager button:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
