
//Denis Svec

#include "lps25hb.h"

uint8_t address_bar = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{

	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_bar, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_bar, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_bar, 1);
}

float lps25hb_get_pressure()
{
	uint32_t press;
    uint8_t press_Out_L, press_Out_H, press_Out_XL;
	uint8_t data[3];


	lps25hb_readArray(data, LPS25HB_ADDRESS_PRESS, 3);

	press_Out_XL = data[0];
	press_Out_L = data[1];
    press_Out_H = data[2];

	press = (uint32_t)press_Out_L << 8 | press_Out_XL;
	press = (uint32_t)press_Out_H << 16 | press;

	return (press/4096.0);
}

float lps25hb_get_temp()
{
	int16_t temperature;
	uint8_t data[2];

	lps25hb_readArray(data, LPS25HB_ADDRESS_TEMP, 2);

	temperature = ((int16_t)((data[1] << 8) | data[0]));

    return (42.5 + (temperature/480));
}

uint8_t lps25hb_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else
	{
		address_bar = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			return status;
		}
	}

	uint8_t ctrl1 = 0b11000000;
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}

