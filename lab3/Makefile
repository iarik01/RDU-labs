
obj-m += lab3_pavlenko.o


KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)


all:
	make -C $(KDIR) M=$(PWD) modules


clean:
	make -C $(KDIR) M=$(PWD) clean

test:
	sudo dmesg -C
	sudo insmod lab3_pavlenko.ko pid=1
	sudo rmmod lab3_pavlenko
	sudo dmesg | tail -n 30
