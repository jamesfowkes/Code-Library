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
	-I$(LIBS_DIR)/Common \
	-I$(LIBS_DIR)/Utility
	
SYMBOLS=-DUNITY_TEST -std=c99 -Wall -Wextra

CLEANUP = $(DEL) $(TARGET)

.PHONY: i2c

all: default

default:
	@echo "Available tests:"
	@echo "i2c"

## Specific dependencies and defines for each test
i2c: TEST_NAME=$@
i2c: SYMBOLS += -DI2C_MR -DI2C_MT
i2c: SRC_FILES += $(LIBS_DIR)/Common/lib_i2c_mr.c $(LIBS_DIR)/Common/lib_i2c_mt.c
i2c: INC_DIRS += -I$(LIBS_DIR)/Common/Tests/i2c
i2c:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)

