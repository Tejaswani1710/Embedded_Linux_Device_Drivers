#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// MODULE_LICENSE("GPL"); /*tells that module bears free license*/
MODULE_AUTHOR("Tejaswani"); /*name of the author*/

/*variables are declared as static to keep there scope local to this module and avoid namespace pollution*/
static char* charvar = "module";
static int intvar = 10;

/*using the following mcro, var's are enable to be modified from command line*/
/*module_param takes 3 agruments: var name, type of var, permission*/

module_param(charvar, charp, S_IRUGO);
module_param(intvar, int, S_IRUGO);

static int __init param_init(void)
{
    printk(KERN_ALERT "\n We are in init function\n");
    printk(KERN_ALERT "\n The value of charvar is %s\n",charvar);
    return 0;
}

static void __exit param_exit(void)
{
    printk(KERN_ALERT"\n GoodBye \n");
}

module_init(param_init);
module_exit(param_exit);
