#include <linux/module.h>
#include <linux/of.h>                                                           
#include <linux/of_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>

struct pinctrl          *pinctrl;
struct pinctrl_state    *pins_default;
int 					retval;
// platform_device 		*lpdev;

static int gpio_probe(struct platform_device *pdev)
{
	printk("gpio_probe\r\n");

	// lpdev = pdev;

	pinctrl 		= devm_pinctrl_get(&(pdev->dev));
	if(IS_ERR(pinctrl))
	{
		printk("devm_pinctrl_get ERR\r\n");
		return PTR_ERR(pinctrl);
	}
		
	pins_default 	= pinctrl_lookup_state(pinctrl, PINCTRL_STATE_DEFAULT);

	if(IS_ERR(pins_default))
	{
		printk("pinctrl_lookup_state ERR\r\n");
		return PTR_ERR(pins_default);
	}
		
	retval 			= pinctrl_select_state(pinctrl, pins_default);
	if(retval)
	{
		dev_err(&(pdev->dev), "err pinctrl_select_state\r\n");
	}

	return 0;
}

static int gpio_remove(struct platform_device *pdev)
{
	printk("gpio_remove\r\n");
	devm_pinctrl_put(pinctrl);

	return 0;
}

static const struct of_device_id gpio_of_matches[] = {
	{.compatible = "company,led-gpio",},
//	{.compatible = "raspberrypi-firmware"},
	{}
};

MODULE_DEVICE_TABLE(of, gpio_of_matches);

static struct platform_driver gpio_driver = 
{
    .driver = {
        	.name	= "gpio_driver",
		.of_match_table = of_match_ptr(gpio_of_matches),
		.owner=THIS_MODULE,
    },
    .probe        = gpio_probe,
    .remove	  = gpio_remove,
};

static int __init gpio_module_init(void)
{
	int ret;
	//printk("Init GPIO_MODULE\r\n");
	ret =  platform_driver_register(&gpio_driver);
	printk("Init GPIO_MODULE[%d]\r\n",ret);
 	return ret;
}

static void __exit gpio_module_exit(void)
{
	int ret;
//	printk("exit GPIO_MODULE\r\n");
	platform_driver_unregister(&gpio_driver);
	printk("Exit GPIO_MODULE\r\n");
}

MODULE_AUTHOR("Jaeson  <djs6421@gmail.com>");
MODULE_DESCRIPTION("gpio driver");
MODULE_LICENSE("GPL");

module_init(gpio_module_init);
module_exit(gpio_module_exit);
