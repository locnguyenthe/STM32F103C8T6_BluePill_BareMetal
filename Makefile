TARGET = main

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

MCU     = -mcpu=cortex-m3 -mthumb
INCLUDES = -I. -IHeader -IInc

CFLAGS  = $(MCU) $(INCLUDES) -O0 -g -Wall -ffreestanding -nostdlib
LDFLAGS = $(MCU) -T linkerscript.ld -nostdlib -Wl,-Map=$(TARGET).map

# Collect .c files from root and Src/ (if present)
SRCS    = $(wildcard *.c) $(wildcard Src/*.c)
OBJS    = $(SRCS:.c=.o)

all: $(TARGET).elf $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(TARGET).elf verify reset exit"

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map

.PHONY: all flash clean