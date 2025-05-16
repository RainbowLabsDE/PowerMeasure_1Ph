#include "Arduino.h"

const int StateLed0 = PC4;
const int StateLed1 = PC3;
const int StateLed2 = PC2;

const int ButtonPin = PD0;

const int RePin = PD2;
const int dePin = PD3;

const int OpOut = PD4;


#define MODBUS_SERIAL Serial
#define MODBUS_BAUD 38400
#define MODBUS_CONFIG SERIAL_8N1
static int MODBUS_UNIT_ID =  1;
