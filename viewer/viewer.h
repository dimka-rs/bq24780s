#ifndef _VIEWER_H_
#define _VIEWER_H_

#define NUM_OF_REGS 13

uint16_t LastData[NUM_OF_REGS];
uint16_t CurrData[NUM_OF_REGS];

uint16_t ExampleData[] = {
    0xE108,
    0xC210,
    0x0384,
    0x1A40,
    0x4A54,
    0x8120,
    0x0000,
    0x0000,
    0x1080,
    0x1800,
    0x1000,
    0x0040,
    0x0030 };

uint8_t Regs[] = {
    0x12,
    0x3B,
    0x38,
    0x37,
    0x3C,
    0x3D,
    0x3A,
    0x14,
    0x15,
    0x39,
    0x3F,
    0xFE,
    0xFF };

uint8_t RegNames[][18] = {
    "ChargeOption0",
    "ChargeOption1",
    "ChargeOption2",
    "ChargeOption3",
    "ProchotOption0",
    "ProchotOption1",
    "ProchotStatus",
    "ChargeCurrent",
    "ChargeVoltage",
    "DischargeCurrent",
    "InputCurrent",
    "ManufacturerID",
    "DeviceID" };

/* example dump
0x12: E108
0x3B: C210
0x38: 0384
0x37: 1A40
0x3C: 4A54
0x3D: 8120
0x3A: 0000
0x14: 0000
0x15: 1080
0x39: 1800
0x3F: 1000
0xFE: 0040
0xFF: 0030
*/

#endif
