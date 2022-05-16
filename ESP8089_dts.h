#ifndef _LINUX_ESP8266EXS_H
#define _LINUX_ESP8266EXS_H

#include <linux/spinlock.h>
#include <linux/spi/spi.h>
#include <linux/platform_device.h>

struct esp8089_info
{
    unsigned int debug;
    int cs_gpio;
    int reset_gpio;
    int interrupt_gpio;
};

int esp8089_probe_dt(struct device *dev);
int esp8089_get_dt_data(struct spi_device *spi);

#endif
