/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * support: wellman@mail.ru; eautomatic@mail.ru
 **************************************************/
//#ifdef ZvvIncludeCRC

#include "CRC.h"
#include "CRC8_lib.h"
#include "CRC8_lib.c"

//XModem:
unsigned short CRC16_calcByte(unsigned short Crc, char DByte) {	//TODO: as define
	Crc = Crc ^ ((uint16_t) DByte << 8);
	for (uint8_t j=0; j<8; j++) {
		if (Crc & 0x8000)
			Crc = (Crc << 1) ^ CRC16_Polinom;
		else
			Crc <<= 1;
	}
	return Crc;
}

unsigned short CRC16_calc(unsigned short crc, char *message, int length)
{
	for(unsigned short i = 0; i < length; i++)
	{
		crc = CRC16_calcByte(crc, *message++); // message[i]
	}
	return crc;
}

/* CRC16_verify() length must include crc bytes!!!*/
bool CRC16_verify(unsigned short crc, char *message, int length) //bol removed from Qt
{	// Returns true if the last two bytes in a message is the crc of the preceding bytes.
	unsigned short expected = CRC16_calc(crc, message, length -= 2);
#ifdef AlineRequired
	char *crcPtr = message + length;
	return expected == (u16)((*crcPtr) | (((u16)(*(crcPtr+1)))<<8));
#else
	return expected == *((unsigned short *)(message + length)); //[0]
#endif
}

void CRC16_append(unsigned short crc, char *message, int length)
{
	crc = CRC16_calc(crc, message, length); //, CRC16_INIT
#ifdef AlineRequired
	char *crcPtr = message + length;
	*crcPtr = (u8)crc;
	*(crcPtr+1) = (u8)(crc>>8);
#else
	*((unsigned short *)(message + length)) = crc;
#endif
}

//#endif //#ifdef ZvvIncludeCRC

/*
.include	"..\Lib.asm\CRC16RAMStr.asm"	///// Code & time optimized CRC16(Xmodem) ******************************

*/

/*
For ref. See avr-libc-user-manual.pdf
23.28.2.4 static __inline__ uint16_t _crc_xmodem_update ( uint16_t __crc, uint8_t __data )
Optimized CRC-XMODEM calculation.
Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
Initial value: 0x0 (in FoxPro sys(2007) and I use 0xFFFF)
This is the CRC used by the Xmodem-CRC protocol.
The following is the equivalent functionality written in C.
uint16_t crc_xmodem_update (uint16_t crc, uint8_t data) {
	int i;
	crc = crc ^ ((uint16_t)data << 8);
	for (i=0; i<8; i++) {
		if (crc & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
	}
return crc;
}
*/

