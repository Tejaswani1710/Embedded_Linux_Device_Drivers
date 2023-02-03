#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/fs.h>		//required for various structes related to files like fops

dev_t dev = 0;
static struct cdev sema_cdev;
struct semaphore my_sema;

//Function Prototypes
static int __init sema_driver_init(void);
static void __exit sema_driver_exit(void);


/*********************Driver Functions*********************************/
static int sema_open(struct inode *inode, struct file *file);
static int sema_release(struct inode *inode, struct file *file);
static ssize_t sema_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t sema_write(struct file *filp, const char *buf, size_t len, loff_t *off);
/**********************************************************************/

//File Operation Structure
static struct file_operations fops =
{
	.owner	 = THIS_MODULE,
	.open	 = sema_open,
	.release = sema_release,
	.read	 = sema_read,
	.write	 = sema_write,
};

/*
 ** This function will be called when we open the device file
 */
static int sema_open(struct inode *inode, struct file *file)
{
	down(&my_sema);
	printk("Opened......\nWelcome to Device File....!!!!\n");
	return 0;
}

/*
 **This function will be called when we close the device file
 */
static int sema_release(struct inode *inode, struct file *file)
{
	up(&my_sema);
	printk("Going out from Device File.....\n Closed!!!\n");
	return 0;
}

/*
 **This function will be called when we read thr device file
 */
static ssize_t sema_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	printk("We are in Read Function....\n");
	return 0;
}

/*
 **This function will be called when we write thr device file
 */
static ssize_t sema_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	printk("We are in the Write Function...\n");
	return 0;
}

/*
 **Module init Function
 */
static int __init sema_driver_init(void)
{
	/***Allocating Major Numbers***/
	if((alloc_chrdev_region(&dev, 0, 1, "sema_dev")) <0)
	{
		printk("Cannot allocate the major number!!!");
		return -1;
	}
	printk("Major: %d\t Minor: %d\n",MAJOR(dev),MINOR(dev));

	/**Creating char Device to the System**/
    	cdev_init(&sema_cdev, &fops);
    	sema_cdev.owner = THIS_MODULE;
    	sema_cdev.ops = &fops;

	/***Adding char to the device system***/
	if((cdev_add(&sema_cdev, dev, 1))<0)
	{
		printk("Cannot add the char to the device system!!!!\n");
		unregister_chrdev_region(dev,1);
		return -1;
	}
	printk("Char is added to the device system successfully....\n");

	printk("Device created successfully.....\n");
	printk("Device Driver insertion...Done!!!");
	return 0;
}

/*
 **Module Exit Function
 */
static void __exit sema_driver_exit(void)
{
	unregister_chrdev_region(dev,1);
	pr_info(" Device drivers Removed....\n");
}

module_init(sema_driver_init);
module_exit(sema_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEJU");
MODULE_DESCRIPTION("SEMAPHORE");

