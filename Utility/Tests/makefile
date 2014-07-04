include $(TEST_NAME)/$(TEST_NAME)_test.mk

CC=gcc
UNITY_FOLDER=E:/WorkDir/Unity
LIBS_DIR=$(PROJECTS_PATH)/Libs
UTIL_DIR=$(LIBS_DIR)/Utility
TEST_DIR=$(UTIL_DIR)/Tests/$(TEST_NAME)

DEL = python $(LIBS_DIR)/del.py

SRC_FILES = \
	$(UNITY_FOLDER)/src/unity.c \
	$(UTIL_DIR)/$(TEST_NAME).c \
	$(TEST_DIR)/$(TEST_NAME)_test.c \
	$(TEST_DIR)/$(TEST_NAME)_test_Runner.c

SRC_FILES += $(EXTRA_SRC_FILES)

INCLUDES = \
	-I$(UNITY_FOLDER)/src \
	-I$(UNITY_FOLDER)/extras/fixture/src \
	-I$(UTIL_DIR) \
	$(EXTRA_INCLUDES)

CCFLAGS = -Wall -Wextra -std=c99 -DTEST_HARNESS -DUNITY_TEST $(EXTRA_CCFLAGS)

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TARGET = $(TEST_NAME)_test$(TARGET_EXTENSION)
OBJS = $(SRC_FILES:.c=.o)
CLEANUP = $(DEL) $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@
	
all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o ./$(TARGET) $^
	./$(TARGET)

clean:
	$(CLEANUP)