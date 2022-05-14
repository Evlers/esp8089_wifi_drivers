# Linux ESP8089 Drivers

使用在Linux内核编译的ESP8266/ESP8089驱动模块

## 硬件

如果使用ESP8089，则不需要对芯片进行任何更改。如果使用在ESP8266，则SPI Flash必须先摘除。
注意：目前未将CS脚持续拉低，会造成模块进入休眠无法通讯或通讯延迟较高，请将CS脚接GND保持唤醒或者使用普通IO进行控制。

#### F1C200S(SPI1)与模块的引脚定义:

| F1C200S      | ESP8266 / ESP8089      | Function         |
| ------------ | ---------------------- | ---------------- |
| PE4          | CHIP_EN                | esp\_reset\_gpio |
| PE7          | GPIO10 / SDIO\_DATA\_3 | esp\_cs\_pin     |
| PE10         | GPIO7 / SDIO\_DATA\_0  | MISO             |
| PE8          | GPIO11 / SDIO\_CMD     | MOSI             |
| PE9          | GPIO6 / SDIO\_CLK      | SCLK             |
| PE5          | GPIO8 / SDIO\_DATA\_1  | esp\_interrupt   |
| 3.3V         | GPIO15 / MTDO          | boot select      |
| 3.3V         | GPIO0                  | boot select      |
| GND          | GPIO2                  | boot select      |

建议在每个I/O引脚上添加电阻 (33 ~ 330 Ohm).

## 设备树配置

Start with a fresh install of MangoPi-Tiny-R3.

#### 第一步: 添加SPI1引脚定义

suniv-f1c100s.dtsi:
```
spi1_pins: spi1-pins {
                pins = "PE7", "PE8", "PE9", "PE10";
                function = "spi1";
                bias-pull-up;
            };
```

```
spi1: spi@1c06000 {
            compatible = "allwinner,suniv-f1c100s-spi",
                     "allwinner,sun8i-h3-spi";
            reg = <0x01c06000 0x1000>;
            interrupts = <11>;
            clocks = <&ccu CLK_BUS_SPI1>, <&ccu CLK_BUS_SPI1>;
            clock-names = "ahb", "mod";
            resets = <&ccu RST_BUS_SPI1>;
            status = "disabled";
            pinctrl-names = "default";
            pinctrl-0 = <&spi1_pins>;
            #address-cells = <1>;
            #size-cells = <0>;
        };
```


#### 第二步: 配置设备驱动节点

devicetree.dts:
```
&spi1 {
	status = "okay";
	
	wifi@0 {
		compatible = "espressif,esp8089";
		spi-cpol;
		spi-cpha;
		reg = <0>;
		spi-max-frequency = <30000000>;

		reset= <132>;//PE4 128 + 4 = 132
		interrupt= <133>;//PE5 128 + 5 = 133
		debug= <0>;
		
		status = "okay";
	};
};
```

#### 第三步：下拉代码到内核

- 把该仓库下拉到内核源码/driver/staging/下
- 修改内核源码/driver/staging/下的Kconfig文件，添加：
`source "drivers/staging/esp8089/Kconfig"`
- 修改内核源码/driver/staging/下的Makefile
`obj-$(CONFIG_ESP8089)           += esp8089/`
- 接着返回内核源码所在目录，输入：
`make menuconfig   #（buildroot请输入 make linux-menuconfig）`
- 然后选中ESP8089，里面选中SPI编译即可:
```
[Device Drivers]
        ->[Staging drivers]
                ->[<M>   Extend for NetWork Using ESP_8266EX/8089] 
                       ->[<M>   Compile SPI-Mode-ESP_8266EX/8089 module in kernel]
```

#### 第四步：重新编译内核
`./rebuild-kernel.sh`

## How it works

The ESP8266 is basically just a rehashed ESP8089, which is a wireless chip 
commonly used in many Unix-based devices. The ESP8089 is designed to load its 
firmware over SPI / SDIO when the device in question boots, whereas the ESP8266 
is intended to load firmware off of the integrated SPI flash component. The 
pins used for this operation, though, are exposed on most varieties of the 
chip. These pins can be therefore utilized to load any custom firmware onto an 
ESP8266; in fact, this is what the eagle\_fw\#.h files are. The SPI 
flash must first be removed to do this.

Upon boot of the host device, the ESP chip is power cycled using the CHIP\_EN 
pin \(held low\) and subsequently set to load code over SPI via GPIO15 
\(held high\). During this time, GPIO0 must be held high and GPIO2 must be held 
low to select the correct boot mode.

## References

[https://github.com/notabucketofspam/ESP8089-SPI](https://github.com/notabucketofspam/ESP8089-SPI)

[https://pinout.xyz/pinout/spi](https://pinout.xyz/pinout/spi)

[https://hackaday.io/project/8678-rpi-wifi](https://hackaday.io/project/8678-rpi-wifi)

[https://github.com/al177/esp8089](https://github.com/al177/esp8089)

[https://github.com/george-hopkins/esp8089-spi](https://github.com/george-hopkins/esp8089-spi)

[https://github.com/linux-rockchip/linux-rockchip/tree/mirror/rk3188-rbox-kk/drivers/net/wireless/esp8089](https://github.com/linux-rockchip/linux-rockchip/tree/mirror/rk3188-rbox-kk/drivers/net/wireless/esp8089)

[https://static.abstore.pl/design/accounts/soyter/img/dokumentacje/esp8089-driver-release-desc_v1-9-2_english.pdf](https://static.abstore.pl/design/accounts/soyter/img/dokumentacje/esp8089-driver-release-desc_v1-9-2_english.pdf)

[https://www.espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf)

[https://www.espressif.com/sites/default/files/documentation/esp8266_hardware_design_guidelines_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp8266_hardware_design_guidelines_en.pdf)

[http://gamma.spb.ru/images/pdf/esp8089_datasheet_en.pdf](http://gamma.spb.ru/images/pdf/esp8089_datasheet_en.pdf)

[https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FE%252FEspressif_FAQ_EN.pdf](https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FE%252FEspressif_FAQ_EN.pdf)

[https://www.signal.com.tr/pdf/cat/8n-esp8266_spi_reference_en_v1.0.pdf](https://www.signal.com.tr/pdf/cat/8n-esp8266_spi_reference_en_v1.0.pdf)

[https://www.espressif.com/sites/default/files/documentation/ESP8266_Pin_List_0.xls](https://www.espressif.com/sites/default/files/documentation/ESP8266_Pin_List_0.xls)

