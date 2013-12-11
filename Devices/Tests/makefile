C_COMPILER=gcc
DEL=@rm -f

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

SYMBOLS = -Wall -Wextra -DTEST
INC_DIRS = -I.. -I../../Utility
SRC_FILES = ../lib_ds3231.c lib_ds3231_test.c ../../Utility/util_time.c ../../Utility/util_bcd.c

TARGET = lib_ds3231_test$(TARGET_EXTENSION)
CLEANUP = $(DEL) $(TARGET)

DS3231:
	$(CLEANUP)	
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

