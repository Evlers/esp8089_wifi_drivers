// SPDX-License-Identifier: GPL-2.0+
/*
 *   Test file
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/of.h>

#include "ESP8089_dts.h"
#include "esp_sif.h"
struct esp8089_info *esp8089_dts_info;

int esp8089_probe_dt(struct device *dev) 
{
    struct device_node *node = dev->of_node;
    int ret;
    if (!node) {
		dev_err(dev, "Missing DT data");
		return -EINVAL;
	}
    ret = of_property_read_u32(node,"reset",&esp8089_dts_info->reset_gpio);//获取reset的GPIO
    ret =of_property_read_u32(node,"interrupt",&esp8089_dts_info->interrupt_gpio); //获取interrupt的GPIO
    if(ret != 0) {
        dev_err(dev,"%s:read reset interrupt gpio error",__func__);
        return ret;
    } 

    ret = of_property_read_u32(node,"debug",&esp8089_dts_info->debug); 
    if(ret != 0) {
        dev_err(dev,"%s:read debug level erorr",__func__);
        esp8089_dts_info->debug=0;
        dev_info(dev,"Set debug level to default value 0");
    }

    dev_info(dev,"succeed get all info from dt!");
    esp8089_set_reset_gpio (esp8089_dts_info->reset_gpio);
    esp8089_set_interrupt_gpio (esp8089_dts_info->interrupt_gpio);
    dev_info(dev,"succeed Cover esp_interrupt esp_reset_gpio !");
    dev_info(dev,"interrupt_gpio: %d, reset_gpio : %d",esp8089_dts_info->interrupt_gpio,esp8089_dts_info->reset_gpio);
    return ret;
}


int esp8089_get_dt_data(struct spi_device *spi)                  
{                                                                          
    struct device *dev;
    int ret;
    dev = &spi->dev;
    esp8089_dts_info =  devm_kzalloc(dev, sizeof(*esp8089_dts_info), GFP_KERNEL);
    if (!esp8089_dts_info)
		return -ENOMEM;
    ret = esp8089_probe_dt(dev);
    if(ret != 0)
        dev_info(dev," esp8089_probe_dt error");
    dev_info(dev,"Finish got dt data");
    return 0;
} 

