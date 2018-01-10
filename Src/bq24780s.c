#include "stm32f0xx_hal.h"
#include "bq24780s.h"


SMBUS_HandleTypeDef * hsmbus;
UART_HandleTypeDef * huart; 

void bq24780s_init(SMBUS_HandleTypeDef * init_hsmbus, UART_HandleTypeDef * init_huart)
{
    hsmbus = init_hsmbus;
    huart = init_huart;
    HAL_UART_Transmit(huart, "bq24780s init\n\r\0", 16, 100);
}

uint8_t bq24780s_read_word(uint8_t * pData)
{
    uint16_t DevAddress = BQ24780S_SLAVE_ADDRESS;
    uint16_t Size = 2;
    uint32_t XferOptions = SMBUS_FIRST_AND_LAST_FRAME_NO_PEC;

    HAL_StatusTypeDef rc;
    rc = HAL_SMBUS_Master_Receive_IT (
        hsmbus, 
        DevAddress, 
        pData, 
        Size, 
        XferOptions );
    if (rc == HAL_OK) {
        return 0;
    } else {
        return 1;
    }
}

void HAL_SMBUS_MasterRxCpltCallback (SMBUS_HandleTypeDef *hsmbus)
{

}
