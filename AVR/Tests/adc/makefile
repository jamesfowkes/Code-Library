NAME = adc_test
CC = gcc 
FLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS

CFILES = \
adc_test.c \
../../lib_adc.c \

INCLUDES = \
-I../../../AVR \
-I../../../AVR/Harness \
-I../../../Generics \
-I../../../Utility
 
all:
	$(CC) $(FLAGS) $(INCLUDES) $(CFILES) -o $(NAME).exe
