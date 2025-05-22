#ifndef SPI_H
#define SPI_H

#include <cstdint>
#include <ch32v00x_spi.h>
#include <ch32v00x_rcc.h>



/* SPI Communication Mode Selection */
// #define SPI_MODE   HOST_MODE

#define SPI_CS_PIN                  GPIO_Pin_3
#define SPI_CS_PORT                 GPIOC

class SPI {
public:
    enum SPI_MODE {
        MASTER_MODE = SPI_Mode_Master,
        SLAVE_MODE = SPI_Mode_Slave
    };
    SPI();


    void begin(SPI_MODE mode,GPIO_TypeDef * csPort,uint16_t csPin);
    // void slaveBegin();
    // void masterBegin();
    void SPISendBytes(uint8_t *sendData, uint32_t length);
    void SPIReceiveBytes(uint8_t *getData, uint32_t length);
    void SPISendReceiveBytes(uint8_t *sendData, uint8_t *getData, uint32_t length);



private:
   GPIO_TypeDef* _csPort;
   uint16_t _csPin;
   SPI_MODE _mode;
};

#endif // SPI_H
