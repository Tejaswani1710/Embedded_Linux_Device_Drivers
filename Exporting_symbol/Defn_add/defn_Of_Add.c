#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include "kern_add.h"

MODULE_LICENSE("GPL");  /*tells that module bears free license*/
MODULE_AUTHOR("Tejaswani");  /*name of author*/

/*This is addtion function that we goin to export as symbol*/
static int my_add(int  a, int b)
{
    return(a+b);
}

/*Command to export symbol into krernel symbol table*/
EXPORT_SYMBOL(my_add);

/*To initialize this module and load it into kernel symbol table*/
static int __init hello_init(void)
{
    /*printk behaves similar to printf but it works without the use of c lib*/
    /*KERN_ALERT is the priority msg; decides the seriousness of msg*/
    printk(KERN_ALERT "\n HELLO TO ALL 😉\n");
    return 0;
}

/*This removes module from kernel symbol table*/
static void __exit hello_exit(void)
{
    printk(KERN_ALERT "\n Bye to ALL 🙋\n");
}

module_init(hello_init);
module_exit(hello_exit);
