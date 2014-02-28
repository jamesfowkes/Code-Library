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

TARGET = $(TEST_NAME)/$(TEST_NAME)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_FOLDER)/src/unity.c ../lib_$(TEST_NAME).c $(TEST_NAME)/$(TEST_NAME)_test.c $(TEST_NAME)/$(TEST_NAME)_test_Runner.c

INC_DIRS=-I$(UNITY_FOLDER)/src -I$(UNITY_FOLDER)/extras/fixture/src -I../ -I../../Utility
SYMBOLS=-DUNITY_TEST

CLEANUP = $(DEL) $(TARGET)

ifeq ($(USE_FIX16), 0)
	LD_SUFFIX = -lm
endif

ifeq ($(USE_FIX16), 1)
	SYMBOLS += -DFIXMATH_OPTIMIZE_8BIT -DFIXMATH_NO_CACHE -DUSE_FIX16
	INC_DIRS += -I../../Utility/libfixmath/libfixmath
	SRC_FILES += ../../Utility/libfixmath/libfixmath/fix16.c ../../Utility/libfixmath/libfixmath/fix16_exp.c ../../Utility/libfixmath/libfixmath/fix16_str.c
endif

.PHONY: thermistor

all: default

default:
	@echo "Available tests:"
	@echo "thermistor"

thermistor : TEST_NAME=$@

thermistor:
	$(CLEANUP)
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET) $(LD_SUFFIX)
	./$(TARGET)

clean:
	$(CLEANUP)
