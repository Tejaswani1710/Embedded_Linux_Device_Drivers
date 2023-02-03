# Makefile for native compilation of module paramaters

obj-m := mod_par.o

KERNELDIR = /lib/modules/5.4.0-136-generic/build

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules install

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
