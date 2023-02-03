# Make file for wait queue statically
obj-m:=wait_queue_manually.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
        
clean:  
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

