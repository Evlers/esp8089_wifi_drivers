#ifndef _LINUX_ESP8266EXS_H
#define _LINUX_ESP8266EXS_H

#include <linux/spinlock.h>
#include <linux/spi/spi.h>
#include <linux/platform_device.h>

int esp8089_probe_dt(struct device *dev);
int esp8089_get_dt_data(struct spi_device *spi);
void esp8089_set_reset_gpio(int numss);
void esp8089_set_interrupt_gpio(int numss);
struct esp8089_info
{
    unsigned int debug;
    int reset_gpio;
    int interrupt_gpio;
};







#endif
