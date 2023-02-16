/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * support: wellman@mail.ru; eautomatic@mail.ru
 **************************************************/
#ifndef Crc_h__
#define Crc_h__
#ifdef __cplusplus
extern "C" {
#endif
//#include "zvvCompiler.h"
#include <stdbool.h>

#define CRC16_INIT       0xFFFF
#define CRC16_Polinom    0x1021
#define CRC16BytesLength 2 //bytes


unsigned short CRC16_calcByte(unsigned short CrcValue, char DByte);
unsigned short CRC16_calc(unsigned short crc, char *message, int length);
bool CRC16_verify(unsigned short crc, char *message, int length); /* CRC16_verify() length must include crc bytes!!! //bol removed from Qt !!!*/
void CRC16_append(unsigned short crc, char *message, int length);
// end Prototypes ///

#ifdef __cplusplus
}
#endif
#endif // Crc_h__
