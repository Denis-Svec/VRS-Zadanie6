
//Denis Svec

#include "hts221.h"

uint8_t address_hyg = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_hyg, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_hyg, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_hyg, 1);
}

int hts221_get_humid()
{
	int16_t H0_T0_out, H1_T0_out, H_T_out, H0_rh, H1_rh;

	uint8_t buffer[2];
	float humid = 0;

	hts221_readArray(buffer, 0x30, 2);
	H0_rh = buffer[0]>>1;
	H1_rh = buffer[1]>>1;

	hts221_readArray(buffer, 0x36, 2);
	H0_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	hts221_readArray(buffer, 0x3A, 2);
	H1_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	hts221_readArray(buffer, 0x28, 2);

	H_T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	humid = ((((int32_t)(H_T_out - H0_T0_out)) * ((int32_t)(H1_rh - H0_rh)*10))/(H1_T0_out - H0_T0_out) + H0_rh*10);

		if(humid>1000)
		{
			humid = 1000;
		}

	return (humid * 0.1);
}

float hts221_get_temp()
{
	uint8_t buffer[4], tmp_r;
	float value;
	int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16, T0_degC, T1_degC;


	hts221_readArray(buffer, 0x32, 2);

	tmp_r = hts221_read_byte(0x35);

	T0_degC_x8_u16 = (((uint16_t)(tmp_r & 0x03)) << 8) | ((uint16_t)buffer[0]);
	T1_degC_x8_u16 = (((uint16_t)(tmp_r & 0x0C)) << 6) | ((uint16_t)buffer[1]);

	T0_degC = T0_degC_x8_u16>>3;
	T1_degC = T1_degC_x8_u16>>3;

	hts221_readArray(buffer, 0x3C, 4);
	T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];

	hts221_readArray(buffer, 0x2A, 2);
	T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	value = (float)((((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC)*10)) /(T1_out - T0_out) + T0_degC*10)*0.10000f;

		return value;
}

uint8_t hts221_init(void)
{

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}



	uint8_t ctrl1 = 0b10000111;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}

