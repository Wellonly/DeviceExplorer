/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * Version: SMO_F2 to X2
 * support: wellman@mail.ru; eautomatic@mail.ru
 **************************************************/
#ifndef __SMORECORD_H
#define __SMORECORD_H

#include <assert.h>
#include "compiler.h" //#include "zvvCompiler.h"
#include "zvvDataTypes.h"
#include "GlobalStatusErrorCodes.h"
#include "smodatatypes.h"
#include "smofoldertype.h"
#include "smoheadtypes.h"
#include "smokeystatus.h"
#include "smo_access_enum.h"

#ifndef FolderMethodBitsCount
#define FolderMethodBitsCount 16
#endif

#if CPUBits == 32
#define DefaultAddressBitsCount  32
#else
#define DefaultAddressBitsCount  16
#endif

#ifndef DataAddressBitsCount
#define DataAddressBitsCount     DefaultAddressBitsCount
#endif

#ifndef MethodAddressBitsCount
#define MethodAddressBitsCount   DefaultAddressBitsCount
#endif

#define smo16Size         16
#define smo32Size         32
#define smoPointerSize    2
#define smoPointerType    u16
#define smoSize(ptr)      iif(((SMORecord32_t*)ptr)->HeadByte.IsName24, smo32Size, smo16Size)

#define smo16DataAndName_maxsize (smo16Size - __builtin_offsetof (SMORecord32_t, Data))
#define smo32DataAndName_maxsize (smo32Size - __builtin_offsetof (SMORecord32_t, Data))

//////////// Offsets begin	///////////////////////////
//deprecated, use: offesetof(SMORecord32_t, ...)
// #define smoHeadByteOffset               0
// #define smoPKeyHOffset                  1
// #define smoSecOffset                    2
// #define smoDataPointerOffset            3
// #define smoDP3rdCKeyLOffset             5   /*Object AttributeOffset*/
// #define smoDTypeCKeyHOffset             6   /*DataTypeOffset*/
#define smoNameOffset                      8 //7
//////////// End Offsets //////////////////////////////

#define	smoEndTableMarker               0xFFFF
#define	smoKeyMask                      0xFFF8  // smoKeyMask Mask bits for compressed SMOAddress,so bit3=SmoIsKeyToFlash_bp
#define	smoKeyZeroCheckMask             0xFFF0	// Mask bits for compressed SMOAddress,so bit3=SmoIsKeyToFlash_bp
#define	smoKeyLowByte_bm                0xF8	//
#define smoKeyTag_bm                    0x0007
#define smoGetLoaderKey_bm              0x0008
#define smoKeyFreeSpace_gc              0xFFF0  /* free, not defined or deleted item */

#define AppName_address_gc              0x0000
#define Loader_address_gc               0x0008

//common Smo address range limits... normally it's device specific!!!
//see also: Smo::IntegrityCheck()
#define SmoDeviceName8ParentKey         0x0002
#define SmoDeviceName24ParentKey        0x0006
#define IsRootKey(Key) ((Key) == SmoDeviceName8ParentKey || (Key) == SmoDeviceName24ParentKey)
#define SmoRamKey_address_min           0x0010
#define SmoRamKey_address_max           0xFFE0 /* must be lower then smoKeyFreeSpace_gc*/
#define SmoFlashKey_address_min         0x0048
#define SmoFlashKey_address_max         0xFFE8 /* must be lower then smoKeyFreeSpace_gc*/
// Smart objects defines begin (smo..) ///
//////////////////for HeadByte begin ///
#define	smoIsKeyToFlash_bp              3 //see Smo spec
#define smoIsName24_bp                  2 //see Smo spec
////////////////// end for HeadByte  ///

#define smoKeyToFlash_gm                (1 << smoIsKeyToFlash_bp)
#define IsSmoKeyToFlash(sKey)           ((sKey) & (1 << smoIsKeyToFlash_bp))
#define SmoKeyTag(sKey)                 ((sKey) & smoKeyTag_bm)

#define smoIsName24_bm                  (1 << smoIsName24_bp)

/// Templates for Security byte ...
#define UserGroup_AllGroups_ReadOnly    0x55 // #define OnlyReadForAllGroups			0x55 renamed up
#define UserGroup_AllGroups_RW          0xAA
#define UserGroup_TunersRW_otherRO      0x95
/// end Templates for Security byte

// User Groups begin ...
#define smoTotalGroupsCount    4
enum UserGroups { /*qt public */
    UserGroup_User       = 0,
    UserGroup_PowerUser  = 1,
    UserGroup_Owner      = 2,
    UserGroup_Tuner      = 3,
};

//deprecated #define UserGroup_NetAccess_bm          0x80
#define UserGroup_mask         0x03
// Security codes begin: //////////////////////////////////////
#define SmoAccess_mask         0x03

typedef
#ifndef __cplusplus
enum_packed
#else
enum
#endif
SmoAccess /*{ NoDataAccess = 0, ReadOnly = 1, FullAccess = 2, Invisible = 3 }*/ SmoAccess; // //0=no data access;1=Read only;2=R/W;3=Invisible. //, OneOfVisible = 4
//NOTE: Invisible folder used as link to sibling smos in another space!!!
enum SmoSecurityCodes { /*qt public */
	SecNoDataAccess = 0,
	SecReadOnly     = 1,
	SecReadWrite    = 2,
	SecInvisible    = 3,
};
#define SecNoDataAccess_gc  0x00
#define SecReadOnly_gc      0x01
#define SecReadWrite_gc     0x02
#define SecInvisible_gc     0x03
//// end For Security byte /////////////////

//For smo Controls ...
#define smoControlHelpPrefix_g8_length  2
#define smoControlHelpPrefix_g16_length 3
#define smoControlHelpPrefix_g24_length 4
#define smoControlHelpPrefix_g32_length 5
// end For smo Controls 

/// MOVED TO smodatatypes.h... Smo DataTypes begin ////
//////////////////for DataType byte begin ///
//#define	smoDTMaxLengthString		0x7F
//#define smoDTStringLengthMask           0x7F // #define smoAStringLengthMask_deprecated            0x3F		//max 63 ASCII chars from DTbyte
////deprecated, use smoDataTypeByte_t.BitPositionInByte    #define smoDTBitNumberMask				0x07

//#define smoDTIsString_bp                7
//#define smoDTIsControl_bp               6
//#define smoDT_IsBit_bp                  5
//#define smoDT_IsUnsigned_bp             4
//#define smoDTDataLength_bm              0x0F
//#define smoDTBitPositionInByte_bm       0x07

//#define	smoDTZeroEndedAString_gc        (1<<smoDTIsString_bp) //use SmoDataType::zeString instead!!!

//#define smoDT_IsUnsigned_bm             (1<<smoDT_IsUnsigned_bp)

//#define smoDTypeByLength(length)        (smoDTZeroEndedAString_gc | (length))
//#define smoDType_as_String(length)      (smoDTZeroEndedAString_gc | (length))
//#define smoDType_IsBit(smoDT)           ((((1<<smoDTIsString_bp)|(1<<smoDTIsControl_bp)|(1<<smoDT_IsBit_bp)) & smoDT.all) == (1<<smoDT_IsBit_bp))

//#define smoDType_as_HelpCodesList_g8(CodesCount)       (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g8_length)
//#define smoDType_as_HelpCodesList_g16(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g16_length)
//#define smoDType_as_HelpCodesList_g24(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g24_length)
//#define smoDType_as_HelpCodesList_g32(CodesCount)      (smoDTZeroEndedAString_gc | CodesCount + smoControlHelpPrefix_g32_length)

//deprecated, use:SmoDataType::U32        #define smoDT_U32_gc                    ((1<<smoDT_IsUnsigned_bp) | smoDTSignedDWord_gc)
//deprecated, use:SmoDataType::Pointer16  #define smoDT_smoPointer_gc             (1<<smoDT_IsUnsigned_bp)

///////////////////End for DataType byte ///

#define smoHeaderSize                   8 //7
/// Smo Name length ////////////////
#define smoShortNameMaxLength           8
#define smoFullNameMaxLength            24
#define SmoNameMaxLength(rec_ptr)       iif(((SMORecord32_t*)rec_ptr)->HeadByte.IsName24, smoFullNameMaxLength, smoShortNameMaxLength)
/// Smo Name length ////////////////

#define smoKeyLByteMaskAddress          0xF0	//mask in address

#define smoFreeSpaceBlankChar           0xFF

//#define smoChildValueForDevice          0x0000

//Smo = Smart objects structs begin ///

typedef uint16_t smoKey; //smo Key: maybe with tag!!!
typedef uint16_t smokey; //smo pointer key(also:ptrkey): must be without tag!!!

//typedef enum_packed SmoHeadType /*: byte*/ { /*old: SmoHeadTypes*/ SmoHeadType_Data = 0, /*old: PropertyInData...PropertyInRAM*/    SmoHeadType_DataPointer = 1, /*old: PropertyInCode...PropertyInFLASH*/    SmoHeadType_Folder      = 2, /*old: FoldersOrNetsOrDevice*/    SmoHeadType_Property    = 3  /*Method = 3,*/} SmoHeadType;
typedef
#ifdef __arm__ /* arm only!!! */
enum_packed
#endif
SMOHeadTypes SmoHeadType;

//typedef enum SmoFolderType /*: byte*/ {Normal = 0, Reserved_1, Reserved_2, Reserved_3, Reserved_4, NetDevice = 5, Virtual = 6, NetFolder = 7} SmoFolderType;
typedef
#ifndef __cplusplus
enum
#endif
FolderTypes SmoFolderType;

typedef union_packed smoHeadByte_ {
    u8  all;
    struct_packed {
        uint8_t IsPointerOrData:1;          //old: IsDataToFlash
        uint8_t IsFoldMethod:1;             //old: IsCallObject
        uint8_t IsName24:1;                 //object format: 0=name=8chars(obj=16bytes), 1=name=24chars(obj=32bytes)
        uint8_t IsParentToFlash:1;          //0=RAM(EE),1=FLASH
        uint8_t PKeyL:4;                    //parent address low byte
    };
//    struct_packed { PointerTypes PointerType:2; //DEPRECATED, use SmoHeadType };
    struct_packed {
        SmoHeadType HeadType:2;
    };
} smoHeadByte_t;
static_assert(sizeof(SmoHeadType) == 1, "sizeof(smoHeadByte_t) == 1");

typedef union_packed SmoSecurityByte {	//sec byte smoSecurityByte_
    u8  all;
    struct_packed {
        SmoAccess User:2;      // uint8_t  User:2;
        SmoAccess PowerUser:2; // uint8_t  PowerUser:2;
        SmoAccess Owner:2;     // uint8_t  Owner:2;
        SmoAccess Tuner:2;     // uint8_t  Tuner:2;
    };
} SmoSecurityByte;
static_assert(sizeof(SmoSecurityByte) == 1, "sizeof(SmoSecurityByte) == 1");

typedef union_packed smoDataTypeByte_t { /*smoDataTypeByte_*/
#ifndef __cplusplus
    enum
#endif
    SmoDataType DataType;
    u8 all;
    struct_packed {
        uint8_t  DataLength:4;
        uint8_t  IsUnSigned:1;
        uint8_t  IsBit:1;
        uint8_t  IsControl:1; /* special codes */
        uint8_t  IsString:1;  /* string or data */
    };
    union_packed
    {
        struct_packed
        {
            uint8_t  BitPositionInDWord:5;
            uint8_t  Bit_gc:3; //to find out if SDT==bit compare it to 1!!!
        };
        struct_packed {
            uint8_t  BitPositionInByte:3;
            uint8_t  BitByteOffset:2;
        };
    };
    struct_packed {
        uint8_t  ControlInterface:6;  //see ControlInterfaces_e
        uint8_t  Control_gc:2;        //to find out if SDT==Control compare it to 1!!!
    };
    struct_packed {
        uint8_t  StringLength:7;
    };
} smoDataTypeByte_t;

//TODO: ParentKeyDefinition & smoParentKey_t make the same!!!
#define ParentKeyDefinition	union_packed {  \
    smoKey                ParentKey;    \
    struct_packed {                     \
        smoHeadByte_t     HeadByte;     \
        uint8_t           PKeyH;        \
    };                                  \
};

//TODO: make ChildKeyDefinition as smoParentKey_t!!!
#define ChildKeyDefinition union_packed { \
    smoKey                ChildKey;       \
    struct_packed {                       \
        uint8_t           CKeyL; /*DA3rdCKeyL; DEPRECATED, use: DataAddress3rd; */    \
        uint8_t           CKeyH; /* smoDataTypeByte_t DTypeCKeyH; DEPRECATED, use: DataType; */    \
    };                                    \
    struct_packed {                       \
    uint8_t               FolderType:3; /*qt ChildTag:3;*/    \
        uint8_t           IsChildKeyFlash:1; \
        uint8_t           CKeyLAddrBits:4;   \
        uint8_t           CKeyHAddrByte;  \
    };                                    \
};

//TODO: exclude per bits inits...
typedef struct_packed FolderDataAddresses_
{
    u16 FolderMethodAddress:FolderMethodBitsCount;
    ChildKeyDefinition
} FolderDataAddresses_t;

// #undef DataAddressBitsCount
// #define DataAddressBitsCount 17
//TODO: exclude per bits inits...
typedef union_packed SmoData_t /*Address_u*/
{
    MemAddress_t          MemAddress;
    FolderDataAddresses_t Folder;

    struct_packed { /* data access*/
#if CPUBits == 32
	uint32_t              DataAddress:DataAddressBitsCount;
#if DataAddressBitsCount != 32
    uint32_t              DataParameter:(32-DataAddressBitsCount);
#endif
#else
    uint16_t              DataAddress:DataAddressBitsCount;
	uint16_t              DataParameter;
#endif
    };

    struct_packed { /* method access*/
#if CPUBits == 32
	uint32_t              MethodAddress:MethodAddressBitsCount;
#if DataAddressBitsCount != 32
    uint32_t              MethodParameter:(32-DataAddressBitsCount);
#endif
#else
    uint16_t              MethodAddress:MethodAddressBitsCount;
	uint16_t              MethodParameter;
#endif
    };
//DEPRECATED, use macros to convert to  char      *RamCharPtr; because bit masking must be performed first!
//so after get the addr bits may be used a MemAddress_t
} SmoData_t;

#define SmartRecord_header  ParentKeyDefinition \
            smoSecurityByte_t Security;   \
            smoDataTypeByte_t Type;   \
            SmoData_t         Data; /*MemAddressU32_t*/

typedef struct_packed SmoRecord16_ {		//see Smo spec
    ParentKeyDefinition
    SmoSecurityByte      Security;
    smoDataTypeByte_t    Type;
    SmoData_t            Data;
    char                 RecordName[smoShortNameMaxLength];
} SMORecord16_t;
static_assert(sizeof(SMORecord16_t) == smo16Size, "sizeof(SMORecord16_t) == smo16Size");

typedef struct_packed SMORecord32_ { 	//see Smo spec
    ParentKeyDefinition
    SmoSecurityByte      Security;
    smoDataTypeByte_t    Type;
    SmoData_t            Data;
    char                 RecordName[smoFullNameMaxLength];
} SMORecord32_t;
static_assert(sizeof(SMORecord32_t) == smo32Size, "sizeof(SMORecord32_t) == smo32Size");
static_assert(offsetof(SMORecord32_t, ParentKey)  == 0, "offsetof SMORecord32_t::ParentKey");
static_assert(offsetof(SMORecord32_t, PKeyH)      == 1, "offsetof SMORecord32_t::PKeyH");
static_assert(offsetof(SMORecord32_t, Security)   == 2, "offsetof SMORecord32_t::Security");
static_assert(offsetof(SMORecord32_t, Type)       == 3, "offsetof SMORecord32_t::Type");
static_assert(offsetof(SMORecord32_t, Data)       == 4, "offsetof SMORecord32_t::Data");
static_assert(offsetof(SMORecord32_t, RecordName) == 8, "offsetof SMORecord32_t::RecordName");

//NOTE: sizeof(smoPtr) is cpu depended; sizeof(smoKey) == 2
typedef SMORecord16_t* smoPtr; //before20160315: smoKey; SMORecord16_t* only!!! NOT SMORecord32_t* !!!
#ifdef IsARM
typedef smoPtr smoFPtr; //same for ARM's arch
#else
#  ifdef __flash
typedef SMORecord16_t __flash * smoFPtr; //before20160315: smoKey; NOT SMORecord32_t !!!
#  endif
#endif

typedef struct smoRecordHeader_ {
    ParentKeyDefinition
    SmoSecurityByte        Security;
    smoDataTypeByte_t      Type;
    SmoData_t              Data;
    smokey                 Selfkey;
} smoHeader_t;

typedef union ExecutionFlags_ {
    struct
    {
        u8 userGroup:2;
        u8 isUserLogined:1;
        u8 IsNetAccess:1;
        u8 IsLocalAccess:1;
    };
    u8 all;
} ExecutionFlags_t;

typedef struct smoExecuteEnvironment_ {
    SMORecord32_t    Rec;       //MUST BE first & SMORecord32_t only!!! see the code!
    smokey           Selfkey;   //WARNING: Selfkey & LastMessage & TagDataPtr positions exchanged, so LOADER MAY NOT WORKED!!!
    union { //may be free used by a programmer!!!
        void*        TagDataPtr;
        int          Tag;
        struct
        {
            u8       TagU8L;
            u8       TagU8H;
        };
    };
    g8               LastMessage;  //WARNING: LastMessage&Selfkey positions exchanged, so LOADER MAY NOT WORKED!!!
    ExecutionFlags_t RuntimeFlags; //NOTE: this flags must be refreshed each time the smo executed!!!
} smoExecuteEnvironment_t; //WARNING: LastMessage&Selfkey positions exchanged, so LOADER MAY NOT WORKED!!!

typedef g8 (*smoTaskAddress_t)(void *ParamPtr, smoExecuteEnvironment_t *env, uintp/*uintp*/ TargetDataLength);

//////////////////////////////// smoKey typedefs...
//TODO: move to a SMO_lib.h
typedef union _smoKey_
{ //simple smo key with tag struct...
    smoKey                  Key;
    struct {
        u8                  KeyTagValue:3;
        u8                  IsKeyFlash :1;
    };
} smoKey_t;

typedef union _smoKeyStatus_
{ //simple smo key with tag struct...
    smoKey                  Key;
    enum SmoKeyStatus       KeyStatus:3;
} smoKeyStatus_t;

//unused...
//typedef	union _smoLoginKey_
//{
//    smoKey   Key;
//    struct
//    {
//        u8   UserGroup:2;
//        u8   IsPassAccepted:1;
//    };
//} smoLoginKey_t;

typedef union _smoTaggedKey_
{ //NOTE: this for reference only...
    struct {
        u8   KeyTagBit0:1;
        u8   KeyTagBit1:1;
        u8   KeyTagBit2:1;
        u16  KeyAllAddressBits:13;
    };
    struct {
        u8   KeyTagTwoBits:2;
        u8   IsKeyTagBit2:1;
        u8   KeyAddressLBits:5;
        u8   KeyAddressHBits:8;
    };
    struct {
        u8   KeyTagValue:3;
        u8   IsKeyFlash :1;
    };
    smoKey   Key;
} smoTaggedKey_t;

typedef union _smoParentKey_
{ //simple smo parent key...
    smoKey        Key;
    smoHeadByte_t headByte;
    struct {
        uint8_t IsDataPointer:1;
        uint8_t IsMethodOrData:1;   //see smo
        uint8_t IsName24:1;         //see smo
    };
} smoParentKey_t;

#define smoSelectKey_NotSelected_bm 0x0001
#define smoSelectKey_NotFounded_bm  0x0002
#define SelectTdi_parentKey(key, size) iif((size), (smoKey)(key), (smoKey)(key | smoSelectKey_NotSelected_bm))
typedef union _smoSelectKey_
{
    smoKey      Key;
    struct {
        uint8_t IsNotSelected:1;
    };
} smoSelectKey_t;

// end smoKey typedefs

#endif		//__SMORECORD_H

