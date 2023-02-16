/*
* CRC8_lib.c
* Created: 2011
* for IAR...GCC
* Author: Valentin Zotov, Copyright 2011,2012
* support: wellman@mail.ru; eautomatic@mail.ru
*/

#include "CRC8_lib.h"

uint8_t CRC8Byte(uint8_t crc, uint8_t data, uint8_t polinom)
{
	CRC8(crc, data, polinom);
	return crc;
}

// CRC8CalcSMO() smoAddress, smoRealLength or smo total length
u8 CRC8CalcSMO(void *smoAddress, intp smoRealLength) {
	char * smoAddr = (char *)smoAddress + smoRealLength;
	while (--smoRealLength > 0 && *--smoAddr == 0) {}
	return CRC8Calc_String((char *)smoAddress+1, smoRealLength, smoCRC8InitionalByte, smoCRC8Polinom);
}

u8 CRC8Calc_String(char *sAddress, uint8_t Count, uint8_t Crc, uint8_t Polinom) {
	for (u8 Cou = 0; Cou < Count; Cou++)
	{
		CRC8(Crc, *sAddress++, Polinom);
	}
	return Crc;
}

u8 CRC8Calc_String_P(char __flash *sAddress, uint8_t Count, uint8_t Crc, uint8_t Polinom) {
	for (u8 Cou = 0; Cou < Count; Cou++)
	{
		CRC8(Crc, *sAddress++, Polinom);
	}
	return Crc;
}
