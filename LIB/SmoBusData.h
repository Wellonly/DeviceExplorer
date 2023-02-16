/*
* SmoBusData.h
* Created: 2015
* for IAR...GCC
* Copyright: Valentin Zotov
* support: wellman@mail.ru; eautomatic@mail.ru
*/
//SmoNetConstants must be defined here!!!

#ifndef SmoBusData_h__
#define SmoBusData_h__
#include "app_config.h"
#include "compiler.h"
#include "NetFrameTypes.h"
#include "SmoRecord.h"

#define SmoBus_dataLength_maxb 244 /* maxb = 255 - SmoBusHeaderLength(8) - crc(2) - reserved(1) */

#ifndef MaxStringLengthByApp
#define MaxStringLengthByApp   SmoBus_dataLength_maxb
#endif
static_assert(!(MaxStringLengthByApp % CPUAlign), "MaxStringLengthByApp must be multiple for CPUAlign!!!");

// DEPRECATED, as SyncField used only for RF!!!
//#ifndef SmoBusFrame_SyncField_type
//#define SmoBusFrame_SyncField_type u8 /* u8 or s8 only!!! */
//#endif
//#ifndef SmoBusFrame_SyncField_length
//#if CPUAlign == 4
//#define SmoBusFrame_SyncField_length 3
//#else
//#define SmoBusFrame_SyncField_length 3
//#endif
//#endif
//#define SmoBusFrame_syncByte_offset    (SmoBusFrame_SyncField_length -1)

//#define SmoBus_SyncField_value         0xAA //old: SmoBus_SyncByte
//#define SmoBus_SyncField_4byte_value   0xAAAAAAAA

#define SmoBus_MasterAddress           0x00
#define SmoBus_FirstPhAddress          0x01
#define SmoBus_FirstSlaveAddress       0x01
//DEPRECATED, use: a Master(or a Slave Address) address... #define SmoBus_OurAddress              0x00

#define smobusMessageCodeAnswer_gc     0

#define smoKeyNotSelected_bm           0x0001

//Smart parameters for the device begin ///
#define OurAddressLength               1
#define MACHeaderLength                offsetof(SmoBusFrame_t, Packet) //20170530: Packet offset changed, see:SmoBusFrame_t
#define SmoBusCRCLength                2
#define SmoBusHeaderLength             offsetof(SmoBusFrame_t, Packet.Data)
#define SmoBusMinAnswerLength          (offsetof(SmoBusFrame_t, Packet.Data) -1) /*-1: exnclude KeyAddressH */
#define SmoBusMinRequestLength         offsetof(SmoBusFrame_t, Packet.DataLength)
#define SmoBusMaxFrameLength           254
#define smoAccessDenied_return_size    3

// Acknowledge formats begin ///
//TODO: support another types of ack formats(selecting on by the length)
// end Acknowledge formats ///

//net key flags...
//see also: PackKeyFlags_t
enum SmoBusKeyFlags {
//request info codes...
    SmoBusKey_readRecord        = 0,
    SmoBusKey_call              = 1,
    SmoBusKey_callGroup         = 2,
    SmoBusKey_callGroupWithData = 3,
//answer codes...
    SmoBusKey_retRecord   = 4,
    SmoBusKey_retData     = 5,
    SmoBusKey_retDevice   = 6,
    SmoBusKey_retStatus   = 7,
};
#define SmoBus_KeyFlags_mask           0x0007

#define ExecuteFlag_bm                 0x0001
#define ExecuteGroupFlag_bm            0x0002
#define ExecuteGroupWithDataFlag_bm    0x0001
#define AnswerFlag_bm                  0x0004
//end net key flags(to slave)

enum SmoBusRootCommands
{ // Commands upon length value (see: TDIRootExtended())...
  SmoBusRoot_get_tdi_info = 0,
  SmoBusRoot_get_id       = 1,
  SmoBusRoot_get_sec_data = 2,
  /* ... */
  SmoBusRoot_set_sec_data = 255,
};

typedef	union PackKeyFlags_t
{
    u8 all;
    struct_packed { /*for enum SmoBusKeyFlags PackType:3; Qt assign 8bytes without packed!!! */ // u8 PackType:3;
        enum SmoBusKeyFlags PackType:3;
    };
    struct
    {
        u8 IsExecuteOrRead:1;
        u8 IsGroupCall:1;
        u8 IsAnswerOrRequest:1;
        u8 IsKeyFlash:1;
    };
    struct
    { //see ExecuteModes_e in smo_lib.h
        u8 ExecuteMode:2;
    };
    struct
    {
        u8 IsGroupCallWithData:1;
    };
} PackKeyFlags_t;

typedef union smoPackKey_t /*union_packed may be removed?:20160328 packed added by Qt 20150507*/
{
    smoKey Key; //old:Smoptr
    struct
    {
        union {
            PackKeyFlags_t PackFlags;
            u8 KeyAddressL;
        };
        u8 KeyAddressH;
    };
} smoPackKey_t;

static_assert(sizeof(smoPackKey_t) == 2, "sizeof(smoPackKey_t) != 2 !!!");

typedef struct_packed smoPacket_t {
  u8           packID;     //Offseter(FramesCounter)
  u8           DataLength;
  smoPackKey_t packKey;
  char         Data[MaxStringLengthByApp];
} smoPacket_t;

typedef struct SmoBusFrame_t
{
//20170602: DEPRECATED, used only for RF: SmoBusFrame_SyncField_type SyncField[SmoBusFrame_SyncField_length];
//20170602: DEPRECATED, used only for RF: u8           FramesCounter; //old: FCounter

  u8           FrameType;
  u8           DestAddress;
  u8           SourceAddress; //NOTE: since 20170530: SourceAddress before TotalLength!!!
  u8           TotalLength;   //NOTE: since 20170530: TotalLength after SourceAddress !!!

  smoPacket_t  Packet; ////NOTE: since 20170530: u8 Offseter; moved to Packet!
  u32          TimeStamp; //WARNING: the Crc will be overwritten by TimeStamp value!!! to save the 2bytes!!!
} SmoBusFrame_t;

enum SmoBusStates /* and offsets if syncField==1byte!!! */ {
//	SmoBusState_syncField     = 0,
	SmoBusState_frametype     = 0,
	SmoBusState_destAddress   = 1,
	SmoBusState_sourceAddress = 2,
	SmoBusState_totalLength   = 3,
	SmoBusState_Packet        = 4, /* same as packID */
	SmoBusState_PacketLength  = 5,
};

#endif // SmoBusData_h__
