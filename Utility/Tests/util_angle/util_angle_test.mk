# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

CC=gcc
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

SRC_FILES=$(UNITY_FOLDER)/src/unity.c $(UTIL_DIR)/$(TEST_NAME).c $(TEST_DIR)/$(TEST_NAME)_test.c $(TEST_DIR)/$(TEST_NAME)_test_Runner.c

OBJS = $(SRC_FILES:.c=.o)

INCLUDES = -I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src -I$(UTIL_DIR)
CCFLAGS = -Wall -Wextra -DTEST_HARNESS -DUNITY_TEST -std=c99

CLEANUP = $(DEL) $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@
	
all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o ./$(TARGET) $^
	./$(TARGET)

clean:
	$(CLEANUP)
	