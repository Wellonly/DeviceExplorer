#ifndef _SMODATATYPES_H
#define _SMODATATYPES_H

#define SizeAutoRange   0x04
#define Size4bytesRange 0x06
#define Size8bytesRange 0x09

#define SmoDTBits_rangeStart 0x20
#define SmoDTBits_rangeEnd   0x3F

#define SmoDTControls_rangeStart 0x40
#define SmoDTControls_rangeEnd   0x7E


/// MOVED FROM SmoRecord.h... Smo DataTypes begin ////
//////////////////for DataType byte begin ///
#define	smoDTMaxLengthString		0x7F
#define smoDTStringLengthMask           0x7F // #define smoAStringLengthMask_deprecated            0x3F		//max 63 ASCII chars from DTbyte
//deprecated, use smoDataTypeByte_t.BitPositionInByte    #define smoDTBitNumberMask				0x07

#define smoDTIsString_bp                7
#define smoDTIsControl_bp               6
#define smoDT_IsBit_bp                  5
#define smoDT_IsUnsigned_bp             4
#define smoDTDataLength_bm              0x0F
#define smoDTBitPositionInByte_bm       0x07

#define	smoDTZeroEndedAString_gc        (1<<smoDTIsString_bp) //use SmoDataType::zeString instead!!!

#define smoDT_IsUnsigned_bm             (1<<smoDT_IsUnsigned_bp)

#define smoDTypeByLength(length)        (smoDTZeroEndedAString_gc | (length))
#define smoDType_as_String(length)      (smoDTZeroEndedAString_gc | (length))
#define smoDType_IsBit(smoDT)           ((((1<<smoDTIsString_bp)|(1<<smoDTIsControl_bp)|(1<<smoDT_IsBit_bp)) & smoDT.all) == (1<<smoDT_IsBit_bp))

#define smoDType_as_HelpCodesList_g8(CodesCount)       (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g8_length)
#define smoDType_as_HelpCodesList_g16(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g16_length)
#define smoDType_as_HelpCodesList_g24(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g24_length)
#define smoDType_as_HelpCodesList_g32(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g32_length)

#define smoDTSize_isFixed(sz) ((sz) <= 8)
//deprecated, use:SmoDataType::U32        #define smoDT_U32_gc                    ((1<<smoDT_IsUnsigned_bp) | smoDTSignedDWord_gc)
//deprecated, use:SmoDataType::Pointer16  #define smoDT_smoPointer_gc             (1<<smoDT_IsUnsigned_bp)

///////////////////End for DataType byte /////////////////////////////////////////

//NOTE: SmoDataType is a uint8_t !!!
 /* ideally: __cplusplus, but it's only actual in compile time(not editor time) */  /*uint8_t*/

//#define SmoDataType_no_wrap
//#if defined(QT) && !defined(SmoDataType_no_wrap) /* QObject wrapper for moc! */
//#include <QObject>
//class SMODataTypes : public QObject
//{
//    Q_OBJECT
//public:
//    SMODataTypes() {} //NOTE: SMOFlags() {} need for metasystem!!!
//    SMODataTypes(const SMODataTypes& other) : QObject(other.parent()) {}
//#endif //defined(QT) && !defined(SmoDataType_no_wrap)

enum SmoDataType
#ifdef __cplusplus /*__x86_64*/
        : unsigned char
#endif
{ /*see also "smodatatype_calc.c"*/
/* Size Auto Range...   *********************************************************/
    Undefined  = 0x00, /*datasize:0; or dynamically defined!!! */
    Void       = 0x10, /*datasize:0; old: SmoPointer==2bytes; now SmoPointer: Pointer16 */

    S8 = 0x01,
    U8 = 0x11,

    S16 = 0x02,
    U16 = 0x12,

    RGB = 0x03,
    U24 = 0x13,

    S32 = 0x04,
    U32 = 0x14,

/* Size 4bytes Range... *********************************************************/
    Float32   = 0x05,
    Pointer32 = 0x15, /* the pointer of a pointer32 */

    DateBSD32  = 0x06, /* or QDate() */
    RealTime32 = 0x16, /* 1 == 30.517578125 uS; max~= 36 hours */

/* Size 8bytes Range... *********************************************************/
    S64 = 0x07,
    U64 = 0x17,

    Float64    = 0x08,
    DateTime64 = 0x18,

    Decimal64  = 0x09,
    Pointer64  = 0x19,  /* the pointer of a long pointer64 */
/* end Size 8bytes Range ********************************************************/

    Pointer16    = 0x0A, /*the pointer of short pointer(2bytes) used also for SmoPointer!!! */
    U128         = 0x1A, /* a data of 16bytes length */

    SizedU8Data  = 0x0B, /*datasize:1..256(0xFF+1)*/
    SizedU32Data = 0x1B, /*datasize:4..(0xFFFFFFFF+4); also used as a file!!! */ // CompositeData = 0x09, // SmoCompositeData = 0x19,

/* big data ... */
    SzData128    = 0x0C, /* data: with size: up to 128 bytes with zero-ended compression! */
    SzData256    = 0x1C, /* data: with size: up to 256 bytes with zero-ended compression! */

    SzData512    = 0x0D, /* data: with size: up to 512 bytes with zero-ended compression! */
    SzData1024   = 0x1D, /* data: with size: up to 1024 bytes with zero-ended compression! */

    SzData2048   = 0x0E, /* data: with size: up to 2048 bytes with zero-ended compression! */
    SzData4096   = 0x1E, /* data: with size: up to 4096 bytes with zero-ended compression! */

    SzData8192   = 0x0F, /* may be unused(RESERVED) on 20170312 */
    SzData16384  = 0x1F, /* may be unused(RESERVED) on 20170312 */

//NOTE: Control's group has higher priority and moved up here(optimized for combobox in the builder)
//NOTE: The Controls typedefs defined in smodatatypedefs.h
    ControlButton     = 0x40,
    ControlSelectItem = 0x41, /*datasize: 2; SelectTdi() is a handler for this */
    ControlLogin      = 0x43, /*datasize: answer:3, request:1+ */
    ControlPassLine   = 0x44, /*datasize: answer:1, request:1+ */
    ControlIPv4       = 0x47, /*instead: IPv4Address  = 0x07   */

    SiblingKey        = 0x7E, /* Links folder sibling items in different sectors */
    Enumerator        = 0x7F, /* old: Constant or ConstantData */

//bits...
    Bit0 = 0x20,
    Bit1 = 0x21,
    Bit2 = 0x22,
    Bit3 = 0x23,
    Bit4 = 0x24,
    Bit5 = 0x25,
    Bit6 = 0x26,
    Bit7 = 0x27,

    Bit8 = 0x28,
    Bit9 = 0x29,
    Bit10 = 0x2A,
    Bit11 = 0x2B,
    Bit12 = 0x2C,
    Bit13 = 0x2D,
    Bit14 = 0x2E,
    Bit15 = 0x2F,

    Bit16 = 0x30,
    Bit17 = 0x31,
    Bit18 = 0x32,
    Bit19 = 0x33,
    Bit20 = 0x34,
    Bit21 = 0x35,
    Bit22 = 0x36,
    Bit23 = 0x37,

    Bit24 = 0x38,
    Bit25 = 0x39,
    Bit26 = 0x3A,
    Bit27 = 0x3B,
    Bit28 = 0x3C,
    Bit29 = 0x3D,
    Bit30 = 0x3E,
    Bit31 = 0x3F,
//end bits

    ZeString = 0x80, /*zero ended string with max size software depended!!! */
/// next: sized data(string)...
    SzData1 = 0x81, /*sized data(string), max size 1byte with zero-ended compression! */
    SzData2 = 0x82, /*sized data(string), max size 2byte with zero-ended compression!, and so on */
    SzData3 = 0x83, /* and so on... */
    SzData4 = 0x84,
    SzData5 = 0x85,
    SzData6 = 0x86,
    SzData7 = 0x87,
    SzData8 = 0x88,
    SzData9 = 0x89,
    SzData10 = 0x8A,
    SzData11 = 0x8B,
    SzData12 = 0x8C,
    SzData13 = 0x8D,
    SzData14 = 0x8E,
    SzData15 = 0x8F,
    SzData16 = 0x90,
    SzData17 = 0x91,
    SzData18 = 0x92,
    SzData19 = 0x93,
    SzData20 = 0x94,
    SzData21 = 0x95,
    SzData22 = 0x96,
    SzData23 = 0x97,
    SzData24 = 0x98,
    SzData25 = 0x99,
    SzData26 = 0x9A,
    SzData27 = 0x9B,
    SzData28 = 0x9C,
    SzData29 = 0x9D,
    SzData30 = 0x9E,
    SzData31 = 0x9F,
    SzData32 = 0xA0,
    SzData33 = 0xA1,
    SzData34 = 0xA2,
    SzData35 = 0xA3,
    SzData36 = 0xA4,
    SzData37 = 0xA5,
    SzData38 = 0xA6,
    SzData39 = 0xA7,
    SzData40 = 0xA8,
    SzData41 = 0xA9,
    SzData42 = 0xAA,
    SzData43 = 0xAB,
    SzData44 = 0xAC,
    SzData45 = 0xAD,
    SzData46 = 0xAE,
    SzData47 = 0xAF,
    SzData48 = 0xB0,
    SzData49 = 0xB1,
    SzData50 = 0xB2,
    SzData51 = 0xB3,
    SzData52 = 0xB4,
    SzData53 = 0xB5,
    SzData54 = 0xB6,
    SzData55 = 0xB7,
    SzData56 = 0xB8,
    SzData57 = 0xB9,
    SzData58 = 0xBA,
    SzData59 = 0xBB,
    SzData60 = 0xBC,
    SzData61 = 0xBD,
    SzData62 = 0xBE,
    SzData63 = 0xBF,
    SzData64 = 0xC0,
    SzData65 = 0xC1,
    SzData66 = 0xC2,
    SzData67 = 0xC3,
    SzData68 = 0xC4,
    SzData69 = 0xC5,
    SzData70 = 0xC6,
    SzData71 = 0xC7,
    SzData72 = 0xC8,
    SzData73 = 0xC9,
    SzData74 = 0xCA,
    SzData75 = 0xCB,
    SzData76 = 0xCC,
    SzData77 = 0xCD,
    SzData78 = 0xCE,
    SzData79 = 0xCF,
    SzData80 = 0xD0,
    SzData81 = 0xD1,
    SzData82 = 0xD2,
    SzData83 = 0xD3,
    SzData84 = 0xD4,
    SzData85 = 0xD5,
    SzData86 = 0xD6,
    SzData87 = 0xD7,
    SzData88 = 0xD8,
    SzData89 = 0xD9,
    SzData90 = 0xDA,
    SzData91 = 0xDB,
    SzData92 = 0xDC,
    SzData93 = 0xDD,
    SzData94 = 0xDE,
    SzData95 = 0xDF,
    SzData96 = 0xE0,
    SzData97 = 0xE1,
    SzData98 = 0xE2,
    SzData99 = 0xE3,
    SzData100 = 0xE4,
    SzData101 = 0xE5,
    SzData102 = 0xE6,
    SzData103 = 0xE7,
    SzData104 = 0xE8,
    SzData105 = 0xE9,
    SzData106 = 0xEA,
    SzData107 = 0xEB,
    SzData108 = 0xEC,
    SzData109 = 0xED,
    SzData110 = 0xEE,
    SzData111 = 0xEF,
    SzData112 = 0xF0,
    SzData113 = 0xF1,
    SzData114 = 0xF2,
    SzData115 = 0xF3,
    SzData116 = 0xF4,
    SzData117 = 0xF5,
    SzData118 = 0xF6,
    SzData119 = 0xF7,
    SzData120 = 0xF8,
    SzData121 = 0xF9,
    SzData122 = 0xFA,
    SzData123 = 0xFB,
    SzData124 = 0xFC,
    SzData125 = 0xFD,
    SzData126 = 0xFE,
    SzData127 = 0xFF,
};

//#if defined(QT) && !defined(SmoDataType_no_wrap) /* QObject wrapper!!! */
//    Q_ENUM(SmoDataType)
////    Q_DECLARE_FLAGS(DataTypes, SmoDataType)
////    Q_FLAG(DataTypes)
//};
//Q_DECLARE_METATYPE(SMODataTypes)
////Q_DECLARE_METATYPE(SMODataTypes*)
////Q_DECLARE_METATYPE(const SMODataTypes*)
//typedef SMODataTypes::SmoDataType SmoDataType;
//#endif // defined(QT) && !defined(SmoDataType_no_wrap)

#endif // _SMODATATYPES_H

