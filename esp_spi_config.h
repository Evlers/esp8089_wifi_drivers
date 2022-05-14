#ifndef _ESP_SPI_CONFIG_H_
#define _ESP_SPI_CONFIG_H_

#define MHz (1000000)

/* https://www.signal.com.tr/pdf/cat/8n-esp8266_spi_reference_en_v1.0.pdf */

//#define SPI_FREQ (10000000)
//#define SPI_FREQ (20000000)                               //  1. 22.5Mhz     2. 45Mhz
#define SPI_FREQ (30000000)                                 //  1. 22.5Mhz     2. 45Mhz

//Below are for spi HZ 22.5M
#if (SPI_FREQ == 30000000)

#define CMD_RESP_SIZE                   (10)//(50)              //Common respon wait time
#define DATA_RESP_SIZE_W                (142+45)//(1024*13)     //   (1024*16)  //(398+400) // (1024*10)    //Only for Write bytes function, data write response.  max:(361+109) 
#define DATA_RESP_SIZE_R                (231+75)//(340+102)     //(231+75)//(340+102)   //Only for Read bytes function, data write response    max:(340+102) 

#define BLOCK_W_DATA_RESP_SIZE_EACH     (10)                    //For each data write resp size, in block write 
#define BLOCK_W_DATA_RESP_SIZE_FINAL    (152)// (142+52)        //For final data write resp size, in block write ,max: 119

#define BLOCK_R_DATA_RESP_SIZE_1ST      (265)//(231+75)         //For each data read resp size, in block read  ,max: 134
#define BLOCK_R_DATA_RESP_SIZE_EACH     (10)//(20)              //For each data read resp size, in block read 

#elif(SPI_FREQ == 20000000)

#define CMD_RESP_SIZE                   (10)            //Common respon wait time
#define DATA_RESP_SIZE_W                (103+40)        //Only for Write bytes function, data write response.  max: 103
#define DATA_RESP_SIZE_R                (118+40)        //Only for Read bytes function, data write response  max: 118
//w: oxFF : 218 clock.     oxFE : 214 clock.  

#define BLOCK_W_DATA_RESP_SIZE_EACH     (20)            //For each data write resp size, in block write 
#define BLOCK_W_DATA_RESP_SIZE_FINAL    (112+40)        //For final data write resp size, in block write ,max :112

#define BLOCK_R_DATA_RESP_SIZE_1ST      (123+40)        //For each data read resp size, in block read ,max: 123
#define BLOCK_R_DATA_RESP_SIZE_EACH     (20)            //For each data read resp size, in block read 
//0xE5 ~0xFF  30us totoal 

#elif (SPI_FREQ == 10000000)

#define CMD_RESP_SIZE 10
#define DATA_RESP_SIZE_W 99
#define DATA_RESP_SIZE_R 10

#define BLOCK_W_DATA_RESP_SIZE_EACH  30
#define BLOCK_W_DATA_RESP_SIZE_FINAL 152

#define BLOCK_R_DATA_RESP_SIZE_1ST   61
#define BLOCK_R_DATA_RESP_SIZE_EACH  30

#else /* Per 1*MHz */

#define CMD_RESP_SIZE                 (0*(SPI_FREQ/1000000)+10)
#define DATA_RESP_SIZE_W              (4.4*(SPI_FREQ/1000000)+55)
#define DATA_RESP_SIZE_R              (14.8*(SPI_FREQ/1000000)-138)

#define BLOCK_W_DATA_RESP_SIZE_EACH   (-1*(SPI_FREQ/1000000)+40)
#define BLOCK_W_DATA_RESP_SIZE_FINAL  (0*(SPI_FREQ/1000000)+152)

#define BLOCK_R_DATA_RESP_SIZE_1ST    (10.2*(SPI_FREQ/1000000)-41)
#define BLOCK_R_DATA_RESP_SIZE_EACH   (-1*(SPI_FREQ/1000000)+40)

#endif

/*
x per 10*MHz

CMD_RESP_SIZE                 0x+10
DATA_RESP_SIZE_W              44x+55
DATA_RESP_SIZE_R              148x-138
                              
BLOCK_W_DATA_RESP_SIZE_EACH   -10x+40
BLOCK_W_DATA_RESP_SIZE_FINAL  0x+152

BLOCK_R_DATA_RESP_SIZE_1ST    102x-41
BLOCK_R_DATA_RESP_SIZE_EACH   -10x+40
*/

#endif // !_ESP_SPI_CONFIG_H_
