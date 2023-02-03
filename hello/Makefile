# Makefile for cross compilation

obj-m := hello.o
KERN_DIR = /lib/modules/5.15.45-v71+/build/

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) clean
