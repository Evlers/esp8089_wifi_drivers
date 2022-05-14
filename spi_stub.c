#include "esp_sif.h"
#include "esp_debug.h"
#include "ESP8089_dts.h"
#include "linux/interrupt.h"
#include "linux/spi/spi.h"
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/* *** *** Board info *** *** */
#ifdef REGISTER_SPI_BOARD_INFO

#define MAX_SPEED_HZ (10*MHz)

static struct spi_board_info esp_board_spi_devices[] = {
	{
		.modalias = "espressif,esp8089",
		.max_speed_hz = MAX_SPEED_HZ,
		.bus_num = 1,
		.chip_select = 0,
		.mode = SPI_MODE_3,
	},
};

void sif_platform_register_board_info(void)
{
  //spi_register_board_info(esp_board_spi_devices, ARRAY_SIZE(esp_board_spi_devices));
}

struct spi_device* sif_platform_new_device(void)
{
	struct device *dev;
	char str [32];
	static struct spi_master *master;
	static struct spi_device *spi;

	master = spi_busnum_to_master(esp_board_spi_devices[0].bus_num);
	if(!master)
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: FAILED to find master\n");

	snprintf(str, sizeof(str), "%s.%u", dev_name(&master->dev), esp_board_spi_devices[0].chip_select);
	dev = bus_find_device_by_name(&spi_bus_type, NULL, str);
	if(dev)
	{
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: Find a dev using spi, deleting!\n");
		device_del(dev);
	}
	else
	{
		esp_dbg(ESP_SHOW, "esp8089_spi: No dev using spi!\n");
	}

	spi = spi_new_device( master, esp_board_spi_devices );
	if(!spi)
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: FAILED to create slave\n");
	if(spi_setup(spi))
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: FAILED to setup slave\n");
	
	return spi;
}
#endif

/* 
HSPI:
  GPIO12  HMISO
  GPIO13  HMOSI
  GPIO14  HSCLK
  GPIO15  HCS

SPI:
  GPIO6   SCLK
  GPIO7   MISO
  GPIO8   MOSI
  GPIO11  CS

SDIO:
  GPIO6   SDCLK
  GPIO7   SDD0
  GPIO8   SDD1
  GPIO9   SDD2
  GPIO10  SDD3
  GPIO11  SDCMD
*/

// static int esp_cs_pin = 135;//PE7 128 + 7 = 135
static int esp_interrupt = 133;//PE5 128 + 5 = 133
static int esp_reset_gpio = 132;//PE4 128 + 4 = 132

void esp8089_set_interrupt_gpio(int numss) 
{
	esp_interrupt=numss;
}

void esp8089_set_reset_gpio(int numss) 
{
    esp_reset_gpio=numss;
}

int sif_platform_irq_init(void)
{
  	int ret;

	esp_dbg(ESP_SHOW, "esp8089_spi: %s enter\n", __func__);

	if ( (ret = gpio_request(esp_interrupt, "esp_interrupt")) != 0)
	{
		esp_dbg(ESP_DBG_ERROR, "esp8089_spi: request gpio error\n");
		return ret;
	}
	gpio_direction_input(esp_interrupt);

  	sif_platform_irq_clear();
	sif_platform_irq_mask(1);

  	udelay(1);

	return 0;
}

void sif_platform_irq_deinit(void)
{
	gpio_free(esp_interrupt);
}

int sif_platform_get_irq_no(void)
{
	return gpio_to_irq(esp_interrupt);
} 

int sif_platform_is_irq_occur(void)
{
	return 1;
}

void sif_platform_irq_clear(void)
{

}

void sif_platform_irq_mask(int mask)
{
	if (mask)
		disable_irq_nosync(sif_platform_get_irq_no());
	else
		enable_irq(sif_platform_get_irq_no());
}

void sif_platform_target_speed(int high_speed)
{

}

#ifdef ESP_ACK_INTERRUPT
void sif_platform_ack_interrupt(struct esp_pub *epub)
{
	sif_platform_irq_clear();
}
#endif

/* *** *** Platform power *** *** */

void sif_platform_reset_target(void)
{
	gpio_request(esp_reset_gpio, "esp_reset_gpio");
	gpio_direction_output(esp_reset_gpio, 1);
	mdelay(1000);
	gpio_direction_output(esp_reset_gpio, 0);
	mdelay(200);
	gpio_direction_output(esp_reset_gpio, 1);
	mdelay(200);
	gpio_free(esp_reset_gpio);
}

void sif_platform_target_poweroff(void)
{
	gpio_direction_output(esp_reset_gpio, 0);
}

void sif_platform_target_poweron(void)
{
	gpio_request(esp_reset_gpio, "esp_reset_gpio");
	mdelay(200);
	gpio_direction_output(esp_reset_gpio, 1);
	mdelay(1000);
	gpio_direction_output(esp_reset_gpio, 0);
	mdelay(200);
	gpio_direction_output(esp_reset_gpio, 1);
	mdelay(200);
	gpio_free(esp_reset_gpio);
}
