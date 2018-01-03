NAME = obj

# make        # build & write == write
# make build  # just make axf
# make clean  # clean files
# make write  # write to stm32

.PHONY: all build write clean

# settings
BIN2MOT     = ../tools/bin2mot/bin2mot
STM32WRITER = ../tools/stm32writer/stm32writer.py
USB_SERIAL_PORT = /dev/tty.SLAB_USBtoUART

# commands
GCC         = arm-none-eabi-gcc
OBJCOPY     = arm-none-eabi-objcopy
OBJDUMP     = arm-none-eabi-objdump
SIZE        = arm-none-eabi-size
RM          = rm -rf

# src
C_SRCS = src/main.c

C_HEADERS = src/stddef.h src/stm32f0.h src/vectors.h src/core_cm0.h src/c_init.h

LD = stm32f030x6.ld

# build
OBJS = main.o

EXECUTABLES = $(NAME).axf $(NAME).bin $(NAME).hex $(NAME).mot

all: $(NAME).mot $(NAME).axf
	$(MAKE) write

build: $(NAME).axf

$(OBJS): $(C_SRCS) $(C_HEADERS)
	$(GCC) -std=gnu99 -Os -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -o "$@" "$<"

$(NAME).axf: $(OBJS)
	$(GCC) -nostartfiles -Wl,-Map="$(NAME).map" -Xlinker --gc-sections -mthumb -mcpu=cortex-m0 -mthumb-interwork -T "$(LD)" -o $@ $(OBJS)
	$(SIZE) "$@"

%.hex: %.axf
	$(OBJCOPY) -O ihex "$<" "$@"

%.bin: %.axf
	$(OBJCOPY) -O binary "$<" "$@"

%.mot: %.bin
	$(BIN2MOT) "$<" > "$@"

clean:
	$(RM) $(OBJS) $(NAME).map $(EXECUTABLES) 

write: $(NAME).mot
	$(STM32WRITER) --port=$(USB_SERIAL_PORT) --go "$<"

.PRECIOUS: $(NAME).bin
