NAME = obj

# settings
STM32WRITER = ../tools/stm32writer/stm32writer.py
USB_SERIAL_PORT = /dev/tty.SLAB_USBtoUART

write: $(NAME).mot
	$(STM32WRITER) --port=$(USB_SERIAL_PORT) --go "$<"
