C_COMPILER=gcc

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
	DEL = del
else
	TARGET_EXTENSION=.out
	DEL = @rm -f
endif

SYMBOLS = -Wall -Wextra -DTEST
INC_DIRS = -I.. -I../../Utility

CLEANUP = $(DEL) $(TARGET)

DS3231_SRC_FILES = ../lib_ds3231.c lib_ds3231_test.c ../../Utility/util_time.c ../../Utility/util_bcd.c
DS3231_TARGET = lib_ds3231_test$(TARGET_EXTENSION)

STP_SRC_FILES = ../lib_stp.c ../lib_stp_barcodes.c ../lib_stp_chars.c ../lib_stp_formatting.c lib_stp_test.c
STP_TARGET = lib_stp_test$(TARGET_EXTENSION)

DS3231: TARGET = $(DS3231_TARGET)
DS3231: SRC_FILES = $(DS3231_SRC_FILES)
DS3231: buildandrun

STP: TARGET = $(STP_TARGET)
STP: SRC_FILES = $(STP_SRC_FILES)
STP: buildandrun

buildandrun: 
	$(CLEANUP)	
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)
