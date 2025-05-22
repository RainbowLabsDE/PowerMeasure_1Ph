#include "BL0942.h"

BL0942::BL0942(SPI &spi) : _spi(spi) {}

// uint8_t * BL0942::sendCommand(IniByte command, RegAdress adresse, const uint8_t data[3])
// {
//     if (command == Write)
//     {
//        uint8_t datas[] = {command, adresse, data[0], data[1], data[2], calculateChecksum({command, adresse, data[0], data[1], data[2]})};
//     }
//     else
//     {
//         uint8_t datas[] = {command, adresse};
//     }
    
    
    
//     uint8_t recDatas[5] = {};

//     _spi.SPISendReceiveBytes(datas, recDatas, sizeof(datas));

//     return checkCRC(recDatas) ? recDatas : nullptr;
// }

uint8_t* BL0942::sendCommand(IniByte command, RegAdress address, const uint8_t data[3]) 
{
    std::vector<uint8_t> commandData = {(uint8_t)command, (uint8_t)address};

    if (command == Write) 
    {
        commandData.insert(commandData.end(), data, data + 3);
        commandData.push_back(calculateChecksum(commandData.data()));
    }

    uint8_t receivedData[5] = {};
    _spi.SPISendReceiveBytes(commandData.data(), receivedData, commandData.size());

    return checkCRC(receivedData) ? receivedData : nullptr;
}

uint8_t BL0942::calculateChecksum(uint8_t *data)
{
    // Berechnung der Summe
    uint8_t checksum = (data[0] + data[1] + data[2] + data[3] + data[4]) & 0xFF;

    // Bitweise Invertierung
    checksum = ~checksum;

    return checksum;
}

bool BL0942::checkCRC(uint8_t *data)
{
    return calculateChecksum(data) == data[5];
}

int32_t  BL0942::combineBytes(int8_t byte1, int8_t byte2, int8_t byte3) {
    return (int32_t(byte1) << 16) | (int32_t(byte2) << 8) | int32_t(byte3);
}

uint32_t  BL0942::combineBytes(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    return (uint32_t(byte1) << 16) | (uint32_t(byte2) << 8) | uint32_t(byte3);
}

int32_t BL0942::read_I_WAVE() {
    uint8_t data[3] = {0}; 
    uint8_t* rec = sendCommand(Read, I_WAVE, data);

    if (rec == nullptr) { 
        return 0; 
    }

    return combineBytes(rec[2], rec[3], rec[4]); 
}

int32_t BL0942::read_V_WAVE()
{
    return 0;
}
uint32_t BL0942::read_I_RMS()
{
    return 0;
}
uint32_t BL0942::read_V_RMS()
{
    return 0;
}
uint32_t BL0942::read_I_FAST_RMS()
{
    return 0;
}
int32_t BL0942::read_WATT()
{
    return 0;
}
uint32_t BL0942::read_CF_CNT()
{
    return 0;
}
uint16_t BL0942::read_FREQ()
{
    return 0;
}
// SystemStatus BL0942::read_STATUS()
// {
//     SystemStatus test;
//     return test;
// }
