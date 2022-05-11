obj-$(CONFIG_ESP8089)        +=  esp8089_spi.o
esp8089_spi-y                 +=  esp_debug.o sdio_sif_esp.o spi_sif_esp.o esp_io.o \
                                    esp_file.o esp_main.o esp_sip.o esp_ext.o esp_ctrl.o \
                                    esp_mac80211.o esp_debug.o esp_utils.o esp_pm.o testmode.o \
                                    ESP8089_dts.o 

# 遗留代码
ccflags-y += -DDEBUG -DSIP_DEBUG -DDEBUG_FS
ccflags-y += -DRX_SENDUP_SYNC -DTESETS
ccflags-y += -DFAST_TX_STATUS 
ccflags-y += -DSIF_DSR_WAR 
ccflags-y += -DKERNEL_IV_WAR
ccflags-y += -DHAS_INIT_DATA
ccflags-y += -DP2P_CONCURRENT 
ccflags-y += -DHAS_FW
ccflags-y += -DESP_ACK_INTERRUPT
ccflags-y += -DESP_USE_SPI
ccflags-y += -DREGISTER_SPI_BOARD_INFO
ifdef ANDROID
ccflags-y += -DANDROID
endif
ifdef P2P_CONCURRENT
ccflags-y += -DP2P_CONCURRENT
endif
ifdef TEST_MODE
ccflags-y += -DTEST_MODE
endif
