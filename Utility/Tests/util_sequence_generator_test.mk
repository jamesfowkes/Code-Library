# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
TARGET_BASE=test
UNITY_FOLDER=E:/WorkDir/Unity

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../$(TEST_NAME).c $(TEST_NAME)_test.c $(TEST_NAME)_test_Runner.c $(PROJECTS_PATH)/Libs/Generics/memorypool.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src -I$(PROJECTS_PATH)/Libs/Generics
SYMBOLS=-DTEST_HARNESS -DUNITY_TEST -DMEMORY_POOL_BYTES=4096 -std=c99

CLEANUP = rm -f build/*.o ; rm -f $(TARGET)

all: clean default

default:
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	
	./$(TARGET)

clean:
	$(CLEANUP)
	
