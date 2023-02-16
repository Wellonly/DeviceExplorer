/*
* CRC8_lib.h
* Created: 2011
* for IAR...GCC
* Author: Valentin Zotov, Copyright 2011, 2012
* support: wellman@mail.ru; eautomatic@mail.ru
*/

#ifndef CRC8_lib_h__
#define CRC8_lib_h__
#include "zvvDataTypes.h"

#define CRC8Polinom                     0x07
#define smoCRC8InitionalByte            0x80
#define smoCRC8Polinom                  0x43


#define CRC8(crc, data, polinom) {       \
	crc ^= data;                         \
	for (u8 i = 0; i < 8; i++)           \
	{                                    \
		if (crc & 0x01)                  \
		crc = (crc >> 1) ^ polinom;      \
		else                             \
		crc >>= 1;                       \
	}                                    \
}

// DEPRECATED: Returns true if the last byte in a message is the crc of the preceding bytes.
// Returns true if the next byte(CRC) after the message equals the calculated value.
//WARNING: since 2012 the length MUST passes without CRC byte!!!
#define CRC8Verify(message, length, Crc, Polinom) (*(message + length) == CRC8Calc_String(message, length, Crc, Polinom))

//WARNING: see note above!!!
#define CRC8Verify_P(message, length, Crc, Polinom) (*(message + length) == CRC8Calc_String_P(message, length, Crc, Polinom))

// Prototypes begin ///
u8 CRC8Byte(uint8_t crc, uint8_t data, uint8_t Polinom);
u8 CRC8Calc_String(char *sAddress, uint8_t Count, uint8_t Crc, uint8_t Polinom);
u8 CRC8Calc_String_P(char __flash *sAddress, uint8_t Count, uint8_t Crc, uint8_t Polinom);
u8 CRC8CalcSMO(void *smoAddress, intp smoRealLength);
#endif // CRC8_lib_h__
