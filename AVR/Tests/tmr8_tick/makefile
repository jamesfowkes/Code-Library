NAME = tmr8_test
CC = gcc 
FLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS -DMEMORY_POOL_BYTES=1024

CFILES = \
tmr8_tick_test.c \
../../lib_tmr8_tick.c \
../../Harness/lib_tmr8_tick_harness.c \
../../../Generics/memorypool.c

INCLUDES = \
-I../../../AVR \
-I../../../AVR/Harness \
-I../../../Generics \
-I../../../Utility
 
all:
	$(CC) $(FLAGS) $(INCLUDES) $(CFILES) -o $(NAME).exe
