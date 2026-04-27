#include <linux/module.h>   /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */     

#define GPIO_NUMBER_27      27

#define LOW                 0
#define HIGH                1

#define DRIVER_AUTHOR ""
#define DRIVER_DESC   "gpio subsystem"

/* Constructor */
static int __init gpio_init(void)
{
    gpio_request(GPIO_NUMBER_27, "gpio_27");
    gpio_direction_output(GPIO_NUMBER_27, LOW);

    gpio_set_value(GPIO_NUMBER_27, HIGH);

    pr_info("GPIO27 set to HIGH, GPIO27 status: %d!\n", gpio_get_value(GPIO_NUMBER_27));
    return 0;
}

/* Destructor */
static void __exit gpio_exit(void)
{
    gpio_set_value(GPIO_NUMBER_27, LOW);

    gpio_free(GPIO_NUMBER_27);
    
    pr_info("Good bye my fen !!!\n");
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 