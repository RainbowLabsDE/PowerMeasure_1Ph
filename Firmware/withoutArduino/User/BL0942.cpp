#include "BL0942.h"
#include <string.h>

BL0942::BL0942(SPI &spi) : _spi(spi) {}

// Resets SPI communication
void BL0942::Reset() {
    uint8_t data[6];
    memset(data, 0xFF, sizeof(data));
    _spi.SPISendBytes(data, sizeof(data));
}

// Returns pointer to received data, or nullptr, if CRC wrong
uint8_t* BL0942::sendCommand(frame_t frame) {
    _spi.SPISendReceiveBytes((uint8_t*)&frame, _receivedData, 6);

    // printf("TX: %02X %02X   %02X %02X %02X   %02X\n", frame.command, frame.address, frame.data[0], frame.data[1], frame.data[2], frame.crc);
    // printf("RX: %02X %02X   %02X %02X %02X   %02X\n", 
    //     _receivedData[0], 
    //     _receivedData[1], 
    //     _receivedData[2], 
    //     _receivedData[3], 
    //     _receivedData[4], 
    //     _receivedData[5] 
    // );

    frame_t* rxFrame = (frame_t*)_receivedData;

    // received data doesn't contain cmd/addr, so need to copy it for CRC calculation
    rxFrame->command = frame.command;
    rxFrame->address = frame.address;

    // if CRC correct, return pointer to data section of receive buffer 
    return checkCRC(rxFrame) ? rxFrame->data : nullptr;
}

uint8_t BL0942::calculateChecksum(frame_t* f) {
    // Berechnung der Summe
    uint8_t checksum = (f->command + f->address + f->data[0] + f->data[1] + f->data[2]) & 0xFF;

    // Bitweise Invertierung
    checksum = ~checksum;

    return checksum;
}

bool BL0942::checkCRC(frame_t* frame) {
    return calculateChecksum(frame) == frame->crc;
}

int32_t  BL0942::combineBytes(int8_t byte1, int8_t byte2, int8_t byte3) {
    return (int32_t(byte1) << 16) | (int32_t(byte2) << 8) | int32_t(byte3);
}

uint32_t  BL0942::combineBytes(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    return (uint32_t(byte1) << 16) | (uint32_t(byte2) << 8) | uint32_t(byte3);
}


int32_t BL0942::rawToValue(uint8_t* data, uint8_t significantBits = 24, bool signExtend = false) {
    int32_t val = data[0] << 16 | data[1] << 8 | data[2];
    val &= (1 << significantBits) - 1;  // mask off irrelevant bits

    if (signExtend) {
        // https://stackoverflow.com/a/17719010
        /* generate the sign bit mask. 'b' is the extracted number of bits */
        int mask = 1U << (significantBits - 1);  

        /* Transform a 'b' bits unsigned number 'x' into a signed number 'r' */
        val = (val ^ mask) - mask; 
    }

    return val;
}

int32_t BL0942::readVal(RegAdress addr, uint8_t significantBits = 24, bool signExtend = false) {
    frame_t frame = {
        .command = Read,
        .address = addr,
    };
    uint8_t* result = sendCommand(frame);

    if (result == nullptr) { 
        return INVALID_DATA; 
    }

    return rawToValue(result, significantBits, signExtend);
}


int32_t BL0942::read_I_WAVE() {
    // uint8_t data[3] = {0}; 
    return readVal(I_WAVE, 20, true);
}

int32_t BL0942::read_V_WAVE() {
    return readVal(V_WAVE, 20, true);
}

uint32_t BL0942::read_I_RMS() {
    return readVal(I_RMS);
}

uint32_t BL0942::read_V_RMS() {
    return readVal(V_RMS);
}

uint32_t BL0942::read_I_FAST_RMS() {
    return readVal(I_FAST_RMS);
}

int32_t BL0942::read_WATT() {
    return readVal(WATT, 24, true);
}

uint32_t BL0942::read_CF_CNT() {
    return readVal(CF_CNT);
}

uint16_t BL0942::read_FREQ() {
    int32_t val = readVal(FREQ, 16);
    if (val == INVALID_DATA) {
        return UINT16_MAX;
    }
    return val;
}

// SystemStatus BL0942::read_STATUS()
// {
//     SystemStatus test;
//     return test;
// }
