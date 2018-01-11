#ifndef _BQ24780S_H_
#define _BQ24780S_H_

/* device slave address */
#define BQ24780S_SLAVE_ADDRESS      0x12

/* registers */
#define BQ24780S_CHARGE_OPTION_0    0x12
#define BQ24780S_CHARGE_OPTION_1    0x3b
#define BQ24780S_CHARGE_OPTION_2    0x38
#define BQ24780S_CHARGE_OPTION_3    0x37
#define BQ24780S_PROCHOT_OPTION_0   0x3c
#define BQ24780S_PROCHOT_OPTION_1   0x3d
#define BQ24780S_PROCHOT_STATUS     0x3a
#define BQ24780S_CHARGE_CURRENT     0x14
#define BQ24780S_CHARGE_VOLTAGE     0x15
#define BQ24780S_DISCHARGE_CURRENT  0x39
#define BQ24780S_INPUT_CURRENT      0x3f
#define BQ24780S_MANUFACTURER_ID    0xfe
#define BQ24780S_DEVICE_ID          0xff

void bq24780s_init(I2C_HandleTypeDef * init_hi2c, UART_HandleTypeDef * init_huart);
void bq24780s_write_word(uint16_t MemAddress, uint8_t * RegValue);
void bq24780s_read_word(uint16_t MemAddress, uint8_t * RegValue);
void bq24780s_print_reg(uint16_t MemAddress);
void bq24780s_dump_regs();

#endif
