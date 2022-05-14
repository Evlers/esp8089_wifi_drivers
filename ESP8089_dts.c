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
#include "esp_debug.h"

struct esp8089_info esp8089_dts_info;
void esp8089_set_interrupt_gpio(int numss);
void esp8089_set_reset_gpio(int numss);

int esp8089_probe_dt(struct device *dev)
{
    int ret;
    struct device_node *node = dev->of_node;

    if (!node)
    {
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: Not found device tree data.");
		return -EINVAL;
	}

    ret = of_property_read_u32(node, "reset", &esp8089_dts_info.reset_gpio);//获取reset的GPIO
    ret = of_property_read_u32(node, "interrupt", &esp8089_dts_info.interrupt_gpio); //获取interrupt的GPIO
    if(ret != 0)
    {
        esp_dbg(ESP_DBG_ERROR, "esp8089_spi: read reset interrupt gpio error");
        return ret;
    }

    ret = of_property_read_u32(node,"debug", &esp8089_dts_info.debug); 
    if(ret != 0) {
        esp_dbg(ESP_DBG_ERROR, "esp8089_spi:read debug level erorr");
        esp8089_dts_info.debug = ESP_DBG_ERROR;
        esp_dbg(ESP_DBG_ERROR, "Set debug level to default value.");
    }

    esp_dbg(ESP_DBG_ERROR, "esp8089_spi: succeed get all info from device tree.");
    esp_msg_level = esp8089_dts_info.debug | ESP_DBG_ERROR;//强制打开错误日志
    esp8089_set_reset_gpio (esp8089_dts_info.reset_gpio);
    esp8089_set_interrupt_gpio (esp8089_dts_info.interrupt_gpio);
    esp_dbg(ESP_DBG_ERROR, "esp8089_spi: succeed Cover esp_interrupt and esp_reset_gpio.");
    esp_dbg(ESP_DBG_ERROR, "esp8089_spi: interrupt_gpio: %d, reset_gpio : %d", esp8089_dts_info.interrupt_gpio, esp8089_dts_info.reset_gpio);
    return ret;
}


int esp8089_get_dt_data(struct spi_device *spi)
{
    struct device *dev;
    int ret;

    dev = &spi->dev;
    
    ret = esp8089_probe_dt(dev);

    return ret;
}
