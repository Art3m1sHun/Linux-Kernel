#include <linux/module.h>     /* Thu vien dung de dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h>	        /* Thu vien nay dung de dinh nghia cac ham allocate major & minor number */
#include <linux/device.h>     /* Thu vien nay dinh nghia cac ham class/create/device_create */
#include <linux/cdev.h>       /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/slab.h>       /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/uaccess.h>    /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */

#define DRIVER_AUTHOR "MinhHung xxxx@gmail.com"
#define DRIVER_DESC "Hello world kernel module"

#define NPAGES 1

struct m_foo_dev{
    int size;
	  dev_t dev_num; 
    char *kmallic_ptr;
  	struct class *m_class;
	  struct cdev m_cdev;
}mdev;

/* Function Prototypes */
static int 	__init hello_world_init(void);
static void 	__exit hello_world_exit(void);
static int 	m_open(struct inode *inode, struct file *file);
static int	m_release(struct inode *inode, struct file *file);
static ssize_t	m_read(struct file *flip, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t	m_write(struct file *flip, const char __user *user_buf, size_t size, loff_t *offset);

static struct file_operations fops = 
{
	.owner		= THIS_MODULE,
	.read 		= m_read,
	.write 		= m_write,
	.open		  = m_open,
	.release	= m_release,
};

/* This function will be called when we open Device file */
static int 	m_open(struct inode *inode, struct file *file)
{
  pr_info("System call open() called...!!!\n");
  return 0;
}

/* This function will be called when we closed Device file */
static int	m_release(struct inode *inode, struct file *file)
{
  pr_info("System call close() called...!!!\n");
  return 0;
}

/* This function will be called when we read Device file */
static ssize_t	m_read(struct file *flip, char __user *user_buf, size_t size, loff_t *offset)
{
  pr_info("System call read() called...!!!\n");
  return 0;
}

/* This function will be called when we write Device file */
static ssize_t	m_write(struct file *flip, const char __user *user_buf, size_t size, loff_t *offset)
{
  pr_info("System call write() called...!!!\n");
  return size;
}


// Constructor
static int 
__init hello_world_init(void)
{
	/* 1.1 Dynamic allocating device number (cat /pro/devices) */
	if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "m-cdev") < 0) {
		pr_err("Failed to alloc chrdev region\n");
		return -1;
	}

	/* 1.2 Static allocationg device number (cat /pro/devices)*/
	// (register_chrdev_region(&mdev.dev_num, 1, "m-cdev")
	pr_info("Major = %d , Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num)); 
  
  /* 02.1 Creating cdev structure */
  cdev_init(&mdev.m_cdev, &fops);
  /* 02.2 Adding character device to the system*/
  if ((cdev_add(&mdev.m_cdev, mdev.dev_num,ls
   1)) < 0){
    pr_err("Cannot create the struct class for my device\n");
    goto rm_device_numb;
  }
  
  /* 03. Creating strcut class */
  if ((mdev.m_class = class_create("m_class")) == NULL){
    pr_err("Cannot create the struct  class for my device\n");
    goto rm_device_numb;
  }
  
  /* 04. Creating device*/
  if((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL){
    pr_err("Cannot create my device\n");
    goto rm_class;
    } 
    
  /* 05. Creating Physical memory*/
  if ((mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL)) == 0){
    pr_err("Cannot allocate memory in kernel\n");
    goto rm_device;
  }
 
  pr_info("Hello world kernel module\n");
  return 0;
  
  rm_device:
    device_destroy(mdev.m_class, mdev.mdev_num);
  rm_class:
   class_destroy(mdev.m_class);
  rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    
  return -1;
}

// Destructor

static void 
__exit hello_world_exit(void)
{  
  kfree(mdev.kmalloc_ptr);
  device_destroy(mdev.m_class, mdev.dev_num);
  class_destroy(mdev.m_class);
  cdev_del(&mdev.m_cdev);
	unregister_chrdev_region(mdev.dev_num, 1);
  
	pr_info("Goodbye\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);



