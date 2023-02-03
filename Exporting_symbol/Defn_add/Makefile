# Makefile for native compilation of module paramaters

obj-m := defn_Of_Add.o

KERNELDIR = /lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

