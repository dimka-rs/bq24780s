#include "stm32f0xx_hal.h"
#include "bq24780s.h"


I2C_HandleTypeDef * hi2c;
UART_HandleTypeDef * huart;

void bq24780s_read_word(uint16_t MemAddress,  uint8_t * pData)
{
    uint16_t DevAddress = BQ24780S_SLAVE_ADDRESS;
    uint16_t MemAddSize = 1;
    uint16_t Size = 2;
    uint32_t Timeout = 100;

    HAL_I2C_Mem_Read(
        hi2c,
        DevAddress,
        MemAddress,
        MemAddSize,
        pData,
        Size,
        Timeout);

}

void bq24780s_write_word(uint16_t MemAddress,  uint8_t * pData)
{
    uint16_t DevAddress = BQ24780S_SLAVE_ADDRESS;
    uint16_t MemAddSize = 1;
    uint16_t Size = 2;
    uint32_t Timeout = 100;

    HAL_I2C_Mem_Write (
        hi2c,
        DevAddress,
        MemAddress,
        MemAddSize,
        pData,
        Size,
        Timeout);
}

void bq24780s_init(I2C_HandleTypeDef * init_hi2c, UART_HandleTypeDef * init_huart)
{
    hi2c = init_hi2c;
    huart = init_huart;
    HAL_UART_Transmit(huart, (uint8_t *) "bq24780s init\n\r\0", 16, 100);
}

void bytetostr(uint8_t byte, uint8_t * str)
{
    uint8_t upper = byte / 16;
    uint8_t lower = byte % 16;
    if (upper > 9) {
        str[0] = upper + 0x37;
    } else {
        str[0] = upper + 0x30;
    }

    if (lower > 9) {
        str[1] = lower + 0x37;
    } else {
        str[1] = lower + 0x30;
    }
}

void bq24780s_print_reg(uint16_t MemAddress)
{
    uint8_t regval[2];
    bq24780s_read_word(MemAddress, regval);
    uint8_t regprint[13];
    regprint[0]='0';
    regprint[1]='x';
    bytetostr(MemAddress, &regprint[2]);
    regprint[4]='=';
    regprint[5]='0';
    regprint[6]='x';
    bytetostr(regval[1], &regprint[7]);
    bytetostr(regval[0], &regprint[9]);
    regprint[11]='\r';
    regprint[12]='\n';

    HAL_UART_Transmit(huart, regprint, 13, 100);
}

void bq24780s_dump_regs()
{
    uint8_t regs[]={
        BQ24780S_CHARGE_OPTION_0,
        BQ24780S_CHARGE_OPTION_1,
        BQ24780S_CHARGE_OPTION_2,
        BQ24780S_CHARGE_OPTION_3,
        BQ24780S_PROCHOT_OPTION_0,
        BQ24780S_PROCHOT_OPTION_1,
        BQ24780S_PROCHOT_STATUS,
        BQ24780S_CHARGE_CURRENT,
        BQ24780S_CHARGE_VOLTAGE,
        BQ24780S_DISCHARGE_CURRENT,
        BQ24780S_INPUT_CURRENT,
        BQ24780S_MANUFACTURER_ID,
        BQ24780S_DEVICE_ID,
        0 };
    uint8_t * preg = regs;
    uint8_t newline[] = { '\r', '\n'};
    HAL_UART_Transmit(huart, newline, 2, 100);

    while(*preg != 0){
        bq24780s_print_reg(*preg);
        preg++;
    }
}


