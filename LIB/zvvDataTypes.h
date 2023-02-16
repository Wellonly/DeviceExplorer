/*
* zvvDataTypes.h
* Created: 2014
* for IAR...GCC
* Copyright: Valentin Zotov
* support: wellman@mail.ru; eautomatic@mail.ru
* changed list: 
* 20140919 - added / removed / changed
*/

#ifndef zvvDataTypes_h__
#define zvvDataTypes_h__

#include "compiler.h" //#include "zvvCompiler.h"

#ifndef __flash
#define __flash
#endif

typedef intp          g8;

typedef int8_t        bol; //before 20160131:char; #define bol     char	//instead bool
typedef uint8_t       byte; //byte: unsigned
typedef int8_t        sbyte;
typedef uint8_t       u8; //#define u8      uint8_t
typedef int8_t        s8;      //#define s8      S8		//signed char
typedef uint16_t      u16; //#define u16     uint16_t
typedef int16_t       s16;
typedef uint32_t      u32; //#define u32     uint32_t
typedef int32_t       s32;
typedef uint64_t      u64; //#define u32     uint32_t
typedef int64_t       s64;
typedef float         f32;  //!< 32-bit floating-point number.
typedef double        f64;  //!< 64-bit floating-point number.
typedef unsigned char uchar; //qt #define uchar	unsigned char
typedef unsigned int  uint; //qt #define uint unsigned int
typedef uint16_t      ushort;
typedef unsigned long ulong;

typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint16_t UInt16; //typedef unsigned short UInt16;
typedef int16_t  Int16;

typedef uint32_t UInt32;
typedef int32_t  Int32;

typedef uint64_t UInt64;
typedef int64_t  Int64;

typedef void (*Task_t)( void *taskDataPtr);
typedef void (*TaskVoid_t)( void );

typedef union_packed MemAddressU16 
{
    struct_packed
    {
            u8       L;
            u8       H;
    };
    u16          U16LH;
} MemAddressU16;

typedef union_packed MemAddress_t
{
    //deprecated, be simpler ;) !!!
//#if CPUBits == 32 || CPUBits == 64 	/* by this we have got byte/word oriented access to the pointer!!! */
//        struct_packed   { MemAddressU16 WordL; MemAddressU16  WordH; };
//        int32_t        Int32Address;
//        uint32_t       UInt32Address;
//#else
//	struct       { u8 L; u8 H; };
//	u16          U16LH;
//#endif
//#if CPUBits == 64
//        int32_t        IntLH;
//        uint32_t       UintLH;
//#else

#ifdef QT
    uint32_t     Uint32LH;
    int32_t      Int32LH;
#else
    uint         UintLH;         //change: U16LH to UintLH; u16>uint
    int          IntLH;
    u16          U16LH;
    u8           AddressByte[4];
    u8           *RamBytePtr;
    char         *RamCharPtr;
    char __flash *FlashCharPtr;  //WARNING: NOT _MEMATTR
#endif
} MemAddress_t;

typedef struct_packed _MemAddressLHWords
{
	 MemAddress_t WordL;
	 MemAddress_t WordH;
} MemAddressLHWords_t;

typedef union_packed MemAddressU32_t
{
#if CPUBits == 32 || CPUBits == 64
        MemAddress_t MemAddress; // MemAddress -> MemAddressLH
#else
	MemAddressLHWords_t MemAddress;
#endif
	int32_t   Int32Address;
	uint32_t  UInt32Address;
} MemAddressU32_t; //old: MemFullAddress_t;

typedef struct FifoBufferU8_
{
	u8                FifoHead;
	u8                FifoTail;
	u8                FifoData[];
} FifoBufferU8_t; //old: FifiBuffer_u8_ FifiBuffer_u8_t

typedef struct FlashWriter_ //PageWriter_
{
	MemAddressU32_t   FlashAddressB;
	char              Data[];
} FlashWriter_t;

// typedef struct DataOffsetedU32_ { //old: DataOffseted_t Used by loader!!!
// 	uint32_t   dataOffset;
// 	char       Data[];
// } DataOffsetedU32_t;

typedef struct SizedU32Data_
{
	uint32_t          dataSize;
	char              Data[];
} SizedU32Data_t;

typedef struct SizedU16Data_
{
	uint16_t          dataSize;
	char              Data[];
} SizedU16Data_t;

typedef struct SizedU8Data_
{
	uint8_t           dataSize;
	char              Data[];
} SizedU8Data_t;

typedef struct SmoCompositeData_
{
	uint8_t           MembersCount;
	char              MemberType[]; // MembersTypesList then its data
} SmoCompositeData_t;

typedef struct CompositeData_
{
	uint8_t           MembersCount;
	char              MemberType[]; // MembersTypesList then its data
} CompositeData_t;


// typedef struct AddressedData_
// {
// 	MemFullAddress_t  AddressU32;
// 	char              Data[];
// } AddressedData_t;

// typedef struct SizedData_
// {
// 	U32               DataSize;
// 	char              Data[];
// } SizedData_t;

// typedef struct SizedAddressedData_
// {
// 	U32               DataSize;
// 	MemFullAddress_t  DataAddressU32;
// 	char              Data[];
// } SizedAddressedData_t;
// 
// typedef struct ProtectedSizedAddressedData_
// {
// 	U32               CRC32;
// 	U32               DataSize;
// 	MemFullAddress_t  DataAddressU32;
// 	char              Data[];
// } ProtectedSizedAddressedData_t;

// cs compatibles section    ....
//#define public
// end cs compatibles section....
#endif // zvvDataTypes_h__
