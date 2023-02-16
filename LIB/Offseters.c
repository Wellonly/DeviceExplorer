/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * support: wellman@mail.ru; eautomatic@mail.ru
 **************************************************/

#include "app_config.h"
#include "Offseters.h"

#ifndef FlyOffseter
#define FlyOffseter(length) (offset_rnd += length) /*TODO: optimize FlyOffseter for a true random generate */
#endif

//	TableDeOffset(RTable) open key: 0x4FB17B89
FLASH(unsigned char TableDeOffset[256]) =
{
	0x7B, 0x8F, 0x22, 0xD6, 0xC9, 0x3D, 0x90, 0x64, 0xAE, 0x5A, 0xF7, 0x03, 0x1C, 0xE8, 0x45, 0xB1, 
	0x60, 0x94, 0x39, 0xCD, 0xD2, 0x26, 0x8B, 0x7F, 0xB5, 0x41, 0xEC, 0x18, 0x07, 0xF3, 0x5E, 0xAA, 
	0x4D, 0xB9, 0x14, 0xE0, 0xFF, 0x0B, 0xA6, 0x52, 0x98, 0x6C, 0xC1, 0x35, 0x2A, 0xDE, 0x73, 0x87, 
	0x56, 0xA2, 0x0F, 0xFB, 0xE4, 0x10, 0xBD, 0x49, 0x83, 0x77, 0xDA, 0x2E, 0x31, 0xC5, 0x68, 0x9C, 
	0x17, 0xE3, 0x4E, 0xBA, 0xA5, 0x51, 0xFC, 0x08, 0xC2, 0x36, 0x9B, 0x6F, 0x70, 0x84, 0x29, 0xDD, 
	0x0C, 0xF8, 0x55, 0xA1, 0xBE, 0x4A, 0xE7, 0x13, 0xD9, 0x2D, 0x80, 0x74, 0x6B, 0x9F, 0x32, 0xC6, 
	0x21, 0xD5, 0x78, 0x8C, 0x93, 0x67, 0xCA, 0x3E, 0xF4, 0x00, 0xAD, 0x59, 0x46, 0xB2, 0x1F, 0xEB, 
	0x3A, 0xCE, 0x63, 0x97, 0x88, 0x7C, 0xD1, 0x25, 0xEF, 0x1B, 0xB6, 0x42, 0x5D, 0xA9, 0x04, 0xF0, 
	0xA3, 0x57, 0xFA, 0x0E, 0x11, 0xE5, 0x48, 0xBC, 0x76, 0x82, 0x2F, 0xDB, 0xC4, 0x30, 0x9D, 0x69, 
	0xB8, 0x4C, 0xE1, 0x15, 0x0A, 0xFE, 0x53, 0xA7, 0x6D, 0x99, 0x34, 0xC0, 0xDF, 0x2B, 0x86, 0x72, 
	0x95, 0x61, 0xCC, 0x38, 0x27, 0xD3, 0x7E, 0x8A, 0x40, 0xB4, 0x19, 0xED, 0xF2, 0x06, 0xAB, 0x5F, 
	0x8E, 0x7A, 0xD7, 0x23, 0x3C, 0xC8, 0x65, 0x91, 0x5B, 0xAF, 0x02, 0xF6, 0xE9, 0x1D, 0xB0, 0x44, 
	0xCF, 0x3B, 0x96, 0x62, 0x7D, 0x89, 0x24, 0xD0, 0x1A, 0xEE, 0x43, 0xB7, 0xA8, 0x5C, 0xF1, 0x05, 
	0xD4, 0x20, 0x8D, 0x79, 0x66, 0x92, 0x3F, 0xCB, 0x01, 0xF5, 0x58, 0xAC, 0xB3, 0x47, 0xEA, 0x1E, 
	0xF9, 0x0D, 0xA0, 0x54, 0x4B, 0xBF, 0x12, 0xE6, 0x2C, 0xD8, 0x75, 0x81, 0x9E, 0x6A, 0xC7, 0x33, 
	0xE2, 0x16, 0xBB, 0x4F, 0x50, 0xA4, 0x09, 0xFD, 0x37, 0xC3, 0x6E, 0x9A, 0x85, 0x71, 0xDC, 0x28
};

// TableInOffset(STable) open key:	0xBFB17B58
FLASH(unsigned char TableInOffset[256]) =
{
	0x69, 0xD8, 0xBA, 0x0B, 0x7E, 0xCF, 0xAD, 0x1C, 0x47, 0xF6, 0x94, 0x25, 0x50, 0xE1, 0x83, 0x32, 
	0x35, 0x84, 0xE6, 0x57, 0x22, 0x93, 0xF1, 0x40, 0x1B, 0xAA, 0xC8, 0x79, 0x0C, 0xBD, 0xDF, 0x6E, 
	0xD1, 0x60, 0x02, 0xB3, 0xC6, 0x77, 0x15, 0xA4, 0xFF, 0x4E, 0x2C, 0x9D, 0xE8, 0x59, 0x3B, 0x8A, 
	0x8D, 0x3C, 0x5E, 0xEF, 0x9A, 0x2B, 0x49, 0xF8, 0xA3, 0x12, 0x70, 0xC1, 0xB4, 0x05, 0x67, 0xD6, 
	0xA8, 0x19, 0x7B, 0xCA, 0xBF, 0x0E, 0x6C, 0xDD, 0x86, 0x37, 0x55, 0xE4, 0x91, 0x20, 0x42, 0xF3, 
	0xF4, 0x45, 0x27, 0x96, 0xE3, 0x52, 0x30, 0x81, 0xDA, 0x6B, 0x09, 0xB8, 0xCD, 0x7C, 0x1E, 0xAF, 
	0x10, 0xA1, 0xC3, 0x72, 0x07, 0xB6, 0xD4, 0x65, 0x3E, 0x8F, 0xED, 0x5C, 0x29, 0x98, 0xFA, 0x4B, 
	0x4C, 0xFD, 0x9F, 0x2E, 0x5B, 0xEA, 0x88, 0x39, 0x62, 0xD3, 0xB1, 0x00, 0x75, 0xC4, 0xA6, 0x17, 
	0x5A, 0xEB, 0x89, 0x38, 0x4D, 0xFC, 0x9E, 0x2F, 0x74, 0xC5, 0xA7, 0x16, 0x63, 0xD2, 0xB0, 0x01, 
	0x06, 0xB7, 0xD5, 0x64, 0x11, 0xA0, 0xC2, 0x73, 0x28, 0x99, 0xFB, 0x4A, 0x3F, 0x8E, 0xEC, 0x5D, 
	0xE2, 0x53, 0x31, 0x80, 0xF5, 0x44, 0x26, 0x97, 0xCC, 0x7D, 0x1F, 0xAE, 0xDB, 0x6A, 0x08, 0xB9, 
	0xBE, 0x0F, 0x6D, 0xDC, 0xA9, 0x18, 0x7A, 0xCB, 0x90, 0x21, 0x43, 0xF2, 0x87, 0x36, 0x54, 0xE5, 
	0x9B, 0x2A, 0x48, 0xF9, 0x8C, 0x3D, 0x5F, 0xEE, 0xB5, 0x04, 0x66, 0xD7, 0xA2, 0x13, 0x71, 0xC0, 
	0xC7, 0x76, 0x14, 0xA5, 0xD0, 0x61, 0x03, 0xB2, 0xE9, 0x58, 0x3A, 0x8B, 0xFE, 0x4F, 0x2D, 0x9C, 
	0x23, 0x92, 0xF0, 0x41, 0x34, 0x85, 0xE7, 0x56, 0x0D, 0xBC, 0xDE, 0x6F, 0x1A, 0xAB, 0xC9, 0x78, 
	0x7F, 0xCE, 0xAC, 0x1D, 0x68, 0xD9, 0xBB, 0x0A, 0x51, 0xE0, 0x82, 0x33, 0x46, 0xF7, 0x95, 0x24
};

// TableDeOffset(RTable) open key: 0x4FB17B89
// TableInOffset(STable) open key: 0xBFB17B58
// FLASH(unsigned char OpenKey1L) = OpenKey1LValue;
// FLASH(unsigned char OpenKey1H) = OpenKey1HValue;
// FLASH(unsigned char OpenKey2L) = OpenKey2LValue;
// FLASH(unsigned char OpenKey2H) = OpenKey2HValue;
// 
// static char OpenKey1RL = OpenKey1LValue; // the var. may be changed on the fly!!!
// static char OpenKey1RH = OpenKey1HValue; // the var. may be changed on the fly!!!

static u8 offset_rnd; //do not init, it's random generated!
static unsigned char GlobalOffseter = 0; //0xC7; //on default == 0xC7

unsigned char DeOffset(unsigned char Data,  unsigned char flyOffsetr, unsigned char OffsetCounter) {
    if (GlobalOffseter/*flyOffsetr*/) {
        return TableDeOffset[Data] - GlobalOffseter - flyOffsetr - OffsetCounter;
	}
	return Data;
}

unsigned char InOffset(unsigned char Data,  unsigned char flyOffsetr, unsigned char OffsetCounter) {
    if (GlobalOffseter/*flyOffsetr*/) {
        return TableInOffset[(Data + GlobalOffseter + flyOffsetr + OffsetCounter) & 0xFF];
	}
	return Data;
}

//void InOffsetString(char *DataStr, uintp StartCharPosition, uintp FullStrLength, unsigned char Offseter)
//{
//	char Tm;
//	if (Offseter) {
//		for (; StartCharPosition < FullStrLength; StartCharPosition++)
//		{ //TODO: optimize it
//			Tm=DataStr[StartCharPosition];
//			DataStr[StartCharPosition] = TableInOffset[(Tm + GlobalOffseter + Offseter + StartCharPosition) & 0xFF];
//		}
//	}
//}

void InOffsetData(char *DataStr, uintp nStartIndex, uintp countb)
{
    if (GlobalOffseter/*flyOffseter*/) {
        char Tm;
        unsigned char flyOffseter = FlyOffseter(countb);
        uintp EndCharPosition = nStartIndex + countb;
        for (; nStartIndex < EndCharPosition; nStartIndex++)
        { //TODO: optimize it
            Tm=DataStr[nStartIndex];
            DataStr[nStartIndex] = TableInOffset[(Tm + GlobalOffseter + flyOffseter + nStartIndex) & 0xFF];
        }
    }
}

void DeOffsetData(char *DataStr, uintp nStartIndex, uintp countb)
{
    if (GlobalOffseter/*flyOffseter*/) {
        unsigned char flyOffseter = FlyOffseter(countb);
        uintp OffsetIndex = nStartIndex;
        uintp EndCharPosition = nStartIndex + countb;
        for (uintp Cou = nStartIndex; Cou < EndCharPosition; Cou++)
        {
            unsigned char data = TableDeOffset[(unsigned char)DataStr[Cou]]; // COMPort.rsTables.TableDeOffset[DataStr[Cou]];
            data -= GlobalOffseter;
            data -= flyOffseter;
            data -= (unsigned char)(OffsetIndex++);
            DataStr[Cou] = data;
        }
    }
}


void Offseter_global_set(char NewGlobalOffseter) {
	GlobalOffseter = NewGlobalOffseter;
}
