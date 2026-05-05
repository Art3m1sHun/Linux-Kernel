#include <linux/module.h>   /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */   
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>

#define LOW                 0
#define HIGH                1

#define DRIVER_AUTHOR "minhhungdenguyn052@gmail.com"
#define DRIVER_DESC   "gpio subsystem"

struct gpio_desc *gpio_27;

static  const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-based", },
    { /* sentinel */ }
};

static int mgpio_driver_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    gpio_27 = gpiod_get(dev, "led27", GPIOD_OUT_LOW);
    gpiod_set_value (gpio_27, HIGH);

    pr_info ("%s - %d\n", __func__, __LINE__);
    return 0;
}

static int mgpio_driver_remove(struct platform_device *pdev){
    gpiod_set_Value(gpio_27, LOW);
    gpiod_put(gpio_27);
    
    pr_info ("%s - %d\n", __func__, __LINE__);
    return 0;
}

static struct platform_driver mgpio = {
    .probe = mgpio_driver_probe,
    .remove = mgpio_driver_remove,
    .driver = {
        .name = "descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .onwer = THIS_MODULE,
    },
};

module_platform_driver(mgpio);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 