
//Denis Svec

#include "lps22hb.h"

uint8_t address_bar = LPS22HB_DEVICE_ADDRESS_0;

uint8_t lps22hb_read_byte(uint8_t reg_addr)
{

	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_bar, 0));
}


void lps22hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_bar, 0);
}


void lps22hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_bar, 1);
}

float lps22hb_get_pressure()
{

	uint32_t press;
	uint8_t data[3];
    uint8_t press_Out_L, press_Out_H, press_Out_XL;

	lps22hb_readArray(data, LPS22HB_ADDRESS_PRESS, 3);

	press_Out_XL = data[0];
	press_Out_L = data[1];
    press_Out_H = data[2];

    press = (uint32_t)press_Out_L << 8 | (uint32_t)press_Out_H << 16 | press_Out_XL;

	return (press/4096.0);
}

float lps22hb_get_temp()
{
	int16_t temperature;
	uint8_t data[2];

	lps22hb_readArray(data, LPS22HB_ADDRESS_TEMP, 2);

	temperature = ((int16_t)((data[1] << 8) | (data[0] & 0xff)));

    return (temperature/100);
}

uint8_t lps22hb_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps22hb_read_byte(LPS22HB_WHO_AM_I_ADDRES);

	if(val == LPS22HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else
	{
		address_bar = LPS22HB_DEVICE_ADDRESS_1;
		val = lps22hb_read_byte(LPS22HB_WHO_AM_I_ADDRES);
		if(val == LPS22HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			return status;
		}
	}

	uint8_t ctrl1 = 0b01001010;
	lps22hb_write_byte(LPS22HB_ADDRESS_CTRL1, ctrl1);

	return status;
}

