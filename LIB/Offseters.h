/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * support: wellman@mail.ru; eautomatic@mail.ru
 **************************************************/

#ifndef Offseters_h__
#define Offseters_h__

#include "compiler.h"
#include "zvvDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//201703 moved up... #include "compiler.h"
//201703 moved up... #include "zvvDataTypes.h"

#define OpenKey1LValue	0xC7
#define OpenKey1HValue	0xC7
#define OpenKey2LValue	0xB1
#define OpenKey2HValue	0xB1

#define OpenKeyDefaultValue	    0xC7

#define OFFSETER_ON_ZERO 0xFE

//old: Offseter
#define OffsetHandler(Dest, Offstr) { \
	u8 NewOffseter = Offstr;          \
	Dest = iif(NewOffseter, NewOffseter, NewOffseter + 1); \
}

#define Offseter_global_init() Offseter_global_set(OpenKeyDefaultValue)

// Prototypes begin //
unsigned char DeOffset(unsigned char Data, unsigned char flyOffsetr, unsigned char OffsetCounter);
unsigned char InOffset(unsigned char Data, unsigned char flyOffsetr, unsigned char OffsetCounter);
//old,use: InOffsetData() (WARNING: 3rd param logic changed!!!) void InOffsetString(char *DataStr, uintp StartCharPosiotion, uintp FullStrLength, unsigned char Offseter);
void InOffsetData(char *DataStr, uintp nStartIndex, uintp countb);
void DeOffsetData(char *DataStr, uintp nStartIndex, uintp countb);
void Offseter_global_set(char NewGlobalOffseter);
// end Prototypes ////

static inline char InOffset_notZero(unsigned char Data, unsigned char Offsetr, unsigned char OffsetCounter) {
	unsigned char offs = InOffset(Data, Offsetr, OffsetCounter);
	return iif(offs, offs, OFFSETER_ON_ZERO);
}

#ifdef __cplusplus
}
#endif
#endif // Offseters_h__
