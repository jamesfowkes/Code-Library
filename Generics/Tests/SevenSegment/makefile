C_COMPILER=gcc
DEL=@rm -f

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

TEST_NAME=seven_segment_map
TARGET = $(TEST_NAME)_test$(TARGET_EXTENSION)

SRC_FILES= ../../$(TEST_NAME).c $(TEST_NAME)_test.c

INC_DIRS=-I../../

CLEANUP = $(DEL) $(TARGET)

all:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
