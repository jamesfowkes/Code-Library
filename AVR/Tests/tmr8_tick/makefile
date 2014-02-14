NAME = tmr8_test
CC = gcc 
FLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS

CFILES = \
tmr8_tick_test.c \
../../lib_tmr8_tick.c \
../../Harness/lib_tmr8_tick_harness_functions.c \
../../../Generics/Harness/memorypool_harness.c

INCLUDES = \
-I../../../AVR \
-I../../../AVR/Harness \
-I../../../Generics \
-I../../../Utility
 
all:
	$(CC) $(FLAGS) $(INCLUDES) $(CFILES) -o $(NAME).exe
