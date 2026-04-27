obj-m += GPIO.o
obj-m += GPIO-legacy.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o app app.c
	sudo chmod 0777 /dev/gpio_device
	sudo chmod 0777 /dev/gpio_device_legacy

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f app