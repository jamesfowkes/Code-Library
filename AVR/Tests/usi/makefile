NAME = usi_test
CC = gcc 
FLAGS = -Wall -Wextra -lpthread -DTEST_HARNESS

CFILES = \
usi_test.c \
../../lib_usi.c \
../../Harness/lib_usi_harness.c \

INCLUDES = \
-I../../../AVR \
-I../../../AVR/Harness \
-I../../../Generics \
-I../../../Utility
 
all:
	$(CC) $(FLAGS) $(INCLUDES) $(CFILES) -o $(NAME).exe
	$(NAME).exe
