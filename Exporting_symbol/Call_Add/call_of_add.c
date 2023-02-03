#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "kern_add.h"

MODULE_LICENSE("GPL");  /*tells that module bears free license*/
MODULE_AUTHOR("Tejaswani");  /*name of author*/

static int one = 1;
static int two = 2;

static int __init add_init(void)
{
    printk(KERN_ALERT "\n We are going to add 🖋\n");
    printk(KERN_ALERT "\n Result of Addition is : %d 😊 \n",my_add(one,two));
    return 0;
}

static void __exit add_exit(void)
{
    printk(KERN_ALERT "\n We are leaving...🙋");
}

module_init(add_init);
module_exit(add_exit);
