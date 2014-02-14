TARGET = lib_uart_test.hex

CFLAGS = -Wall -Wextra -c -O3
LDFLAGS=

FCPU = 16000000
DEVICE = attiny2313

EXTRA_DEFINES = \
-DMEMORY_POOL_BYTES=128

INC_DIRS = \
../ \
../../Generics

INC = $(foreach d, $(INC_DIRS), -I$d)

CC = avr-gcc
MV = mv

OBJS = \
lib_uart_test.o \
../lib_uart.o \
../lib_clk.o \
../lib_fuses.o \
../../Generics/ringbuf.o \
../../Generics/memorypool.o

all: $(OBJS) $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -mmcu=$(DEVICE) -DF_CPU=$(FCPU) $(EXTRA_DEFINES) $< -o $@

clean:
	rm $(OBJS)