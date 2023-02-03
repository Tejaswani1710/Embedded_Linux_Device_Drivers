/*********************************************************************************************************************************
 *	\file		Kernel Timers												 *
 *																 *
 *	\Author		Teju													 *
 *																 *
 *********************************************************************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>		//__init and __exit macros
#include <linux/module.h>	// for module_init and module_exit
#include <linux/kdev_t.h>		// major & minor numbers
#include <linux/fs.h>		// device numbers,structure of file operations
#include <linux/cdev.h>		//strucutre used by kernel for char device registration
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/err.h>

//Timer variable
#define TIMEOUT 5000	//millisecond

static struct timer_list etx_timer;
static unsigned int count = 0;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

/**************************Driver function***************************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);
/********************************************************************/

//File operation structure
static struct file_operations fops =
{
	.owner	 = THIS_MODULE,
	.read	 = etx_read,
	.write	 = etx_write,
	.open	 = etx_open,
	.release = etx_release,
};

//Timer callback function.This will be called when timer expires
void timer_callback(struct timer_list *data)
{
	/*our timer stuff here*/
	pr_info("Timer Callback Function called: %d\n",++count);

	/*
	 ** Re-enable timer,. Because this function will be called only first time
	 ** If we Re-enable this it will work like periodic timer
	 */
	mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

/*
 **This function will be called when we open the device file
 */
static int etx_open(struct inode *inode, struct file *file)
{
	pr_info(" Opened...!!!\n Welcome to Device File......\n");
	return 0;
}

/*
 **This function will be called when we close the device file
 */
static int etx_release(struct inode *inode, struct file *file)
{
	pr_info(" Going out from Device File\n Closed...!!!\n");
	return 0;
}

/*
 **this function will be called when we read the device file
 */
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info(" We are in Read function\n");
	return 0;
}

/*
 **this function will be called when we write the device file
 */
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	pr_info(" We are in Write Function\n");
	return 0;
}	

/*
 **Module Init Function
 */
static int __init etx_driver_init(void)
{	
	/*Allocating major numbers*/
	if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev"))<0)
	{
		pr_err(" Cannot allocate the major number.\n");
		return -1;
	}
	pr_info(" Major: %d\t Minor: %d\n",MAJOR(dev),MINOR(dev));

	/*Creating cdev structure*/
	cdev_init(&etx_cdev,&fops);

	/*Adding character device to the system*/
	if((cdev_add(&etx_cdev, dev, 1))<0)
	{
		pr_err(" Cannot add the device to the system\n");
		goto r_class;
	}

	/*Creating struct class*/
	if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class")))
	{
		pr_err(" Cannot create the struct class\n");
		goto r_class;
	}

	/*Creating Device*/
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device")))
	{
		pr_err(" Cannot create the device\n");
		goto r_device;
	}
	
	/*Set up your timer to call my_timer_callback*/
	timer_setup(&etx_timer, timer_callback, 0);

	/*Setup timer interval tp based on TIMEOUT macro*/
	mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));

	pr_info(" Device Drivers Inserion Done....\n");
	return 0;

r_device:
	class_destroy(dev_class);

r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

/*
 **Module exit function
 */
static void __exit etx_driver_exit(void)
{
	/*remove kernel timer when unloading module*/
	del_timer(&etx_timer);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	pr_info(" Device drivers Removed....\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEJU");
MODULE_DESCRIPTION("KERNEL TIMER");

