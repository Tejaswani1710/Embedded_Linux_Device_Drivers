#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_AUTHOR("Teju");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World MOdule");

static int helloModule_init(void) {
	printk(KERN_ALERT "Hello World!\n Inserting Module into kernel\n");
	return 0;
}

static void helloModule_exit(void) {
	printk(KERN_ALERT "Good bye....\n Removing Module from kernel\n");
}

module_init(helloModule_init);
module_exit(helloModule_exit);
