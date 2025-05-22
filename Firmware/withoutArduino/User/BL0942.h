#ifndef BL0942_H
#define BL0942_H

#include "spi.h"
#include <vector>


class BL0942
{
public:
    BL0942(SPI &spi);
    

    struct SystemStatus
    {
        uint8_t CF_REVP_F;  // This bit indicates the direction of the last energy Pulse CF - 0: active forward; 1: active reverse
        uint8_t CREEP_F;    // This bit indicates whether the BL0942 is in active power no-load status - 0: not active power no-load state; 1: active power no-load state
        uint8_t I_ZX_LTH_F; // This bit indicates the current signal is below zero crossing current detection threshold
        uint8_t V_ZX_LTH_F; // This bit indicates the current signal is below zero crossing current detection threshold
    };

    int32_t read_I_WAVE();      // Current waveform data, signed
    int32_t read_V_WAVE();      // Voltage waveform data, signed
    uint32_t read_I_RMS();      // Current RMS, unsigned
    uint32_t read_V_RMS();      // Voltage RMS, unsigned
    uint32_t read_I_FAST_RMS(); // Current fast RMS, unsigned
    int32_t read_WATT();        // Active power, signed
    uint32_t read_CF_CNT();     // Active energy pulse counter, unsigned
    uint16_t read_FREQ();       // Line voltage frequency
    // SystemStatus read_STATUS(); // System Status



    void Reset();

    static const int32_t INVALID_DATA = INT32_MAX;

private:
    SPI &_spi;
    enum IniByte : uint8_t
    {
        Read = 0x58,
        Write = 0xa8
    };

    enum RegAdress : uint8_t
    { /********** Read-only register ************/
      I_WAVE = 0x01,
      V_WAVE = 0x02,
      I_RMS = 0x03,
      V_RMS = 0x04,
      I_FAST_RMS = 0x05,
      WATT = 0x06,
      CF_CNT = 0x07,
      FREQ = 0x08,
      STATUS = 0x09,
      /**********User operation register ************/
      I_RMSOS = 0x12,
      WA_CREEP = 0x14,
      I_FAST_RMS_TH = 0x15,
      I_FAST_RMS_CYC = 0x16,
      FREQ_CYC = 0x17,
      OT_FUNX = 0x18,
      MODE = 0x19,
      GAIN_CR = 0x1A,
      SOFT_RESET = 0x1C,
      USR_WRPROT = 0x1D
    };

    typedef struct {
        IniByte command;
        RegAdress address;
        uint8_t data[3];
        uint8_t crc;
    } frame_t;
    static_assert(sizeof(frame_t) == 6, "Wrong struct packing size, should be 6 bytes in size");

    uint8_t *sendCommand(frame_t frame);
    uint8_t calculateChecksum(frame_t* frame);
    bool checkCRC(frame_t* frame);
    uint32_t combineBytes(uint8_t byte1, uint8_t byte2, uint8_t byte3);
    int32_t combineBytes(int8_t byte1, int8_t byte2, int8_t byte3);
    int32_t rawToValue(uint8_t* data, uint8_t significantBits, bool signExtend);
    int32_t readVal(RegAdress addr, uint8_t significantBits, bool signExtend);


    uint8_t _receivedData[6] = {0};

};

#endif // BL0942_H
