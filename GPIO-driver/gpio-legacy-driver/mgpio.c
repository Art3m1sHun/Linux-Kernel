#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

static void __iomem *gpio_base;

static int __init gpio_driver_init(void)
{
        uint32_t reg;

    //01. Map GPIO reg into kernel virtual address space
    gpio_base = ioremap(BCM2835_GPIO_BASE_ADDR, 0x28);
    if(!gpio_base){
        pr_err("Failed to ioremap GPIO\n");
        return -ENOMEM;
    }
    
    //02. Configure GPIO27 as output
    reg = ioread32(gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10)*4);
    reg &= ~(7 << ((GPIO_NUMBER_27 % 10) *3)); //clear current function
    reg |= (1 << ((GPIO_NUMBER_27 % 10)* 3)); //set as output
    iowrite32(reg, gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10)*4) 
    return 0;

    //03. Set GPIO27 to HIGH
    iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_SET_0_OFFSET);
    pr_info("GPIO 27 SET HIGH\n");
}

static void __exit gpio_driver_exit(void)
{
    //01. Set GPIO27 to LOW
    iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_CLR_0_OFFSET);
    pr_info("GPIO 27 SET LOW\n");

    //02. Unmap GPIO memory region
    if (gpio_base) iounmap(gpio_base);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_LICENSE("DevLinux");
MODULE_DESCRIPTION("Legacy GPIO Driver for Raspberry Pi Zero 2w");
