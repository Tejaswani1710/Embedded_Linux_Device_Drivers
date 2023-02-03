# Makefile for native compilation of Export symbol
obj-y := Call_Add/ Defn_add/

KERNELDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

Default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a
remove:
	rm -rf /lib/modules/$(shell uname -r)/build/extra/*
	depmod -a
