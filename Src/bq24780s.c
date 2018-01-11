#include "stm32f0xx_hal.h"
#include "bq24780s.h"


I2C_HandleTypeDef * hi2c;
UART_HandleTypeDef * huart; 

uint8_t bq24780s_read_word(uint8_t regnum,  uint8_t * pData)
{
    uint16_t DevAddress = BQ24780S_SLAVE_ADDRESS;
    uint16_t MemAddress = BQ24780S_MANUFACTURER_ID;
    uint16_t MemAddSize = 1;
    uint16_t Size = 2;
    uint32_t Timeout = 100;
    HAL_StatusTypeDef rc;

    rc = HAL_I2C_Mem_Read(
            hi2c,
            DevAddress,
            MemAddress,
            MemAddSize,
            pData,
            Size,
            Timeout);

   if (rc == HAL_OK) {
       return 0;
   } else {
       return 1;
   }
}

void bq24780s_init(I2C_HandleTypeDef * init_hi2c, UART_HandleTypeDef * init_huart)
{
    hi2c = init_hi2c;
    huart = init_huart;
    HAL_UART_Transmit(huart, "bq24780s init\n\r\0", 16, 100);
    uint8_t rcval[2];
    uint8_t rc = bq24780s_read_word(BQ24780S_DEVICE_ID, rcval);
    rc += 0x30; //printable digit
    //rcval[0] += 0x20;
    rcval[1] += 0x30;
    HAL_UART_Transmit(huart, &rc, 1, 100);
    HAL_UART_Transmit(huart, rcval, 2, 100);
}

