#include <linux/module.h>     /* Thu vien dung de dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h>	      /* Thu vien nay dung de dinh nghia cac ham allocate major & minor number */
#include <linux/device.h>     /* Thu vien nay dinh nghia cac ham class/create/device_create */
#include <linux/cdev.h>       /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/slab.h>       /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/uaccess.h>    /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/gpio.h>       /* Thu vien nay ho tro gpio */

#define DRIVER_AUTHOR "MinhHung minhhungdenguyn052@gmail.com"
#define DRIVER_DESC "GPIO Kernel module"

#define GPIO_PIN    27

#define LOW         0
#define HIGH        1

#define LED_ON   _IOW('a', 1, int32_t *)
#define LED_OFF  _IOW('a', 0, int32_t *)

#define NPAGES 1

static void __iomem *gpio_base;

struct m_foo_dev{
    int     size;
	dev_t   dev_num; 
    char    *kmalloc_ptr;
  	struct  class *m_class;
	struct  cdev m_cdev;
}mdev;

static int 	    __init gpio_init(void);
static void 	__exit gpio_exit(void);
static long     m_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = 
{
	.owner		= THIS_MODULE,
    .unlocked_ioctl = m_ioctl,
};

static long m_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd){
        case LED_ON:
            pr_info("LED_ON\n");
            gpio_request(GPIO_PIN, "gpio_27");
            gpio_direction_output(GPIO_PIN, LOW);
            gpio_set_value(GPIO_PIN, HIGH);
            break;
        case LED_OFF:
            pr_info("LED_OFF\n");
            gpio_request(GPIO_PIN, "gpio_27");
            gpio_direction_output(GPIO_PIN, LOW);
            gpio_set_value(GPIO_PIN, LOW);
            break;
        default:
            pr_info("default\n");
            break;
    }
    return 0;
}

static int __init gpio_init(void)
{   
    /* 1.1 Dynamic allocating device number (cat /pro/devices) */
    if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "gpio") < 0) {
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }

    pr_info("Major = %d , Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num)); 
    /* 02.1 Creating cdev structure */
    cdev_init(&mdev.m_cdev, &fops);

    /* 02.2 Adding character device to the system*/
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num,1)) < 0){
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    /* 03. Creating strcut class */
    if ((mdev.m_class = class_create("m_class")) == NULL){
        pr_err("Cannot create the struct  class for my device\n");
        goto rm_device_numb;
    }

    /* 04. Creating device*/
    if((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "gpio_device")) == NULL){
        pr_err("Cannot create my device\n");
        goto rm_class;
    } 

    /* 05. Creating Physical memory*/
    if ((mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL)) == 0){
        pr_err("Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    pr_info("GPIO kernel module\n");
    return 0;

    rm_device:
        device_destroy(mdev.m_class, mdev.dev_num);
    rm_class:
        class_destroy(mdev.m_class);
    rm_device_numb:
        unregister_chrdev_region(mdev.dev_num, 1);
    
    return -1;
    
}

static void __exit gpio_exit(void)
{  
    kfree(mdev.kmalloc_ptr);
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    cdev_del(&mdev.m_cdev);
    unregister_chrdev_region(mdev.dev_num, 1);
  
	pr_info("Goodbye\n");
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 