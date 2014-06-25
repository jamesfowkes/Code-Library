NAME = tmr16_pwm_test
CC = gcc 
FLAGS = -Wall -Wextra -DTEST_HARNESS -DF_CPU=16000000 -Wfatal-errors
MMCU = iom328p
AVR_DIR = $(AVR32_HOME)\avr\include\avr
HARNESS_DIR = $(PROJECTS_PATH)\Libs\AVR\Harness\avr

CFILES = \
tmr16_pwm_test.c \
../../Harness/lib_tmr16_harness_functions.c \
../../Harness/lib_io_harness.c \
../../lib_tmr16_pwm.c \
../../lib_tmr16.c \
../../lib_io.c \
../../lib_clk.c \
../../lib_fuses.c \

INCLUDES = \
-I../../../AVR \
-I../../../AVR/Harness \
-I../../../Utility
 
all:
	python ..\..\Harness\generate_test_io.py $(MMCU) $(AVR_DIR) $(HARNESS_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $(CFILES) -o $(NAME).exe
	$(NAME).exe
