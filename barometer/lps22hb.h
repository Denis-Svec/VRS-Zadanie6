#ifndef LPS22HB_H_
#define LPS22HB_H_

#include "main.h"
#include "i2c.h"


#define     LPS22HB_DEVICE_ADDRESS_0                0xB8
#define     LPS22HB_DEVICE_ADDRESS_1                0xBA

#define     LPS22HB_WHO_AM_I_VALUE                    0xB1
#define     LPS22HB_WHO_AM_I_ADDRES                    0x0F

#define     LPS22HB_ADDRESS_CTRL1                    0x10

#define     LPS22HB_ADDRESS_PRESS                    0x28
#define     LPS22HB_ADDRESS_TEMP                    0x2B

uint8_t lps22hb_init(void);
float lps22hb_get_pressure();
uint8_t lps22hb_read_byte(uint8_t reg_addr);
void lps22hb_write_byte(uint8_t reg_addr, uint8_t value);
float lps22hb_get_temp();


#endif /* LPS22HB_H_ */
