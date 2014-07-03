# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 
# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
UNITY_FOLDER=E:/WorkDir/Unity
LIBS_DIR=$(PROJECTS_PATH)/Libs
UTIL_DIR=$(LIBS_DIR)/Utility
TEST_DIR=$(UTIL_DIR)/Tests/$(TEST_NAME)

DEL = python $(LIBS_DIR)/del.py

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TARGET = $(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES= \
$(UNITY_FOLDER)/src/unity.c \
$(UTIL_DIR)/$(TEST_NAME).c \
$(TEST_DIR)/$(TEST_NAME)_test.c \
$(TEST_DIR)/$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src -I$(UTIL_DIR)
SYMBOLS=-DTEST_HARNESS -DUNITY_TEST -std=c99 -DUNIX_TIME_TYPE=int32_t

CLEANUP = $(DEL) $(TARGET)

all: clean default

default:
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	
	./$(TARGET)

clean:
	$(CLEANUP)
	

