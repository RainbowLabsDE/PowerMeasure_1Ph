#include "spi.h"

SPI::SPI()
{
}

void SPI::begin(SPI_MODE mode, GPIO_TypeDef * csPort, uint16_t csPin)
{
    _csPort = csPort;
    _csPin = csPin;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = csPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(csPort, &GPIO_InitStructure);

    if (mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    else
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = mode; // SPI Mode set

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // TODO Bauthrate setzbar über Varibale
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

void SPI::SPISendBytes(uint8_t *sendData, uint32_t length)
{
    GPIO_ResetBits(_csPort, _csPin);
    uint32_t loop = 0;
    uint8_t tmp = 0;
    for (loop = 0; loop < length; loop++)
    {
        // Send SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
            ; // wait while flag is zero or TX buffer not empty
        SPI_I2S_SendData(SPI1, sendData[loop]);

        // Receive SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
            ; // wait while flag is zero or RX buffer is empty
        tmp = SPI_I2S_ReceiveData(SPI1);
    }
    GPIO_SetBits(_csPort, _csPin);
}

void SPI::SPIReceiveBytes(uint8_t *getData, uint32_t length)
{
    GPIO_ResetBits(_csPort, _csPin);
    uint32_t loop = 0;
    for (loop = 0; loop < length; loop++)
    {
        // Send SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
            ; // wait while flag is zero or TX buffer not empty
        SPI_I2S_SendData(SPI1, 0x00);

        // Receive SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
            ; // wait while flag is zero or RX buffer is empty
        getData[loop] = SPI_I2S_ReceiveData(SPI1);
    }
    GPIO_SetBits(_csPort, _csPin);
}

void SPI::SPISendReceiveBytes(uint8_t *sendData, uint8_t *getData, uint32_t length)
{
    GPIO_ResetBits(_csPort, _csPin);
    uint32_t loop = 0;
    for (loop = 0; loop < length; loop++)
    {
        // Send SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
            ; // wait while flag is zero or TX buffer not empty
        SPI_I2S_SendData(SPI1, sendData[loop]);

        // Receive SPI Byte
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
            ; // wait while flag is zero or RX buffer is empty
        getData[loop] = SPI_I2S_ReceiveData(SPI1);
    }
    GPIO_SetBits(_csPort, _csPin);
}