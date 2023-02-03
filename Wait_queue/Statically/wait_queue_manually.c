#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>       //for open and release method
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>     //structures of type struct cdev used by kernel
#include <linux/device.h>
#include <linux/uaccess.h>      //copy to/from user
#include <linux/kthread.h>
#include <linux/wait.h>     //required for wait queues
#include <linux/slab.h>     //kmalloc

uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_hcwq);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev hcwq_cdev;
int wait_queue_flag = 0;

/*
**Function prototypes
*/
static int __init hcwq_driver_init(void);
static void __exit hcwq_driver_exit(void);

/**********************Driver Functions*******************/
static int hcwq_open(struct inode *inode, struct file *file);
static int hcwq_release(struct inode *inode, struct file *file);
static ssize_t hcwq_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t hcwq_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/*
**File operation structure
*/
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = hcwq_read,
    .write = hcwq_write,
    .open = hcwq_open,
    .release = hcwq_release,
};

/*
**Thread function
*/
static int wait_function(void *unused)
{
    while(1)
     {
        pr_info("Waiting for event.....\n");
        wait_event_interruptible(wait_queue_hcwq, wait_queue_flag != 0);
        if(wait_queue_flag == 2)
         {
            pr_info("Event came from Exit function\n");
            return 0;
         }
        pr_info("Event came from Read Function: %d",++read_count);
        wait_queue_flag = 0;
     }
    do_exit(0);
    return 0;
}

/*
**This function will be called when we open the Device File
*/
static int hcwq_open(struct inode *inode, struct file *file)
{
    pr_info("Device is opened!!!..\n");
    return 0;
}

/*
** This function will be called when we Close the Device File
*/
static int hcwq_release(struct inode *inode, struct file *file)
{
    pr_info("Device file is Closed!!!...\n");
    return 0;
}

/*
** This function will be called when we Read the Device File
*/
static ssize_t hcwq_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Welcome to Read function...\n");
    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_hcwq);
    return 0;
}

/*
**This function will be called when we Write the Device File
*/
static ssize_t hcwq_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    pr_info("Here in the Write Function....\n");
    return len;
}

/*
**Module init Function
*/
static int __init hcwq_driver_init(void)
{
    /***Allocating Major Numbers***/
    if((alloc_chrdev_region(&dev, 0, 1, "hcwq-dev")) <0)
     {
        pr_info("Cannot allocate the major number....\n");
        return -1;
     }
    pr_info("Major: %d\t Minor: %d\n",MAJOR(dev),MINOR(dev));

    /**Creating char Device to the System**/
    cdev_init(&hcwq_cdev, &fops);
    hcwq_cdev.owner = THIS_MODULE;
    hcwq_cdev.ops = &fops;

    /**Adding Char Device to the system**/
    if((cdev_add(&hcwq_cdev, dev, 1))<0)
     {
        pr_info("Cannot add the Device to the system...\n");
        goto r_class;
     }
    
    /**Creating Structure class**/
    if((dev_class = class_create(THIS_MODULE,"hcwq_class")) <0)
     {
        pr_info("Cannot create the struct class...\n");
        goto r_class;
     }

    /**Creating Device**/
    if((device_create(dev_class,NULL,dev,NULL,"hcwq_device")) == NULL)
     {
        pr_info("Cannot create the device....\n");
        goto r_device;
     }
    
    //Create the kernel thread with name 
    wait_thread = kthread_create(wait_function, NULL, "WaitThread");
    if(wait_thread)
     {
        pr_info("thread created Successfully!!!..\n");
        wake_up_process(wait_thread);
     }
    else
     {
        pr_info("Thread Creation Failed..\n");
     }

    pr_info("Device Driver Insert......Done!!!\n");
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
static void __exit hcwq_driver_exit(void)
{
    wait_queue_flag = 2;
    wake_up_interruptible(&wait_queue_hcwq);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&hcwq_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("Device Driver remove....done!!!");
}

module_init(hcwq_driver_init);
module_exit(hcwq_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEJU");
MODULE_DESCRIPTION("Simple linux driver(wait queue static method)");
MODULE_VERSION("1.7");

