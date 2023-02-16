#ifndef SMOCONSTANTS
#define SMOCONSTANTS

namespace SmoNetConstants {
    const byte SyncByte = 0xAA;
//    static const byte SyncLength = 1;
//    static const byte FrameType = 0x80;
//    static const byte FirstPhAddress = 0x01;
//    static const byte OurAddress = 0x00; //host address
//    static const int  MaxFrameLength = 255 + 1; //+1 = Sync byte
//    static const byte MaxDataFieldLength = 246;
//    static const byte MaxDataLength = (byte)(MaxDataFieldLength - 1);
//    static const int smoAccessDenied_return_size = 3; //old:4
//    //net key flags(to slave)...
//    static const ushort ExecuteFlag_bm = 0x0001;
//    static const ushort ExecuteGroupFlag_bm = 0x0002;
//    static const ushort ExecuteGroupWithDataFlag_bm = 0x0001;
//    static const ushort AnswerErrorFlag_bm = 0x0001;
//    static const ushort AnswerFlag_bm = 0x0004;
//    //end net key flags(to slave)

//    //App/loader ...
//    static const ushort AppName_address_gc = 0x0000;
//    static const ushort Loader_address_gc = 0x0008;
//    //end App/loader

//    //net key flags(from slave)...
//    static const ushort smoKeyNotSelected_bm = 0x0001;
//    //end net key flags(from slave)

//    static const ushort smoDTControl_getMenu_gc = 0x0000;

//    public static int SmoNetFrame_key_offset = (int)Marshal.OffsetOf(typeof(SmoNetFrame_t), "key");
//    public static int SmoNetFrame_data_offset = (int)Marshal.OffsetOf(typeof(SmoNetFrame_t), "data");
//    public static int SmoNetFrame_dataLength_offset = (int)Marshal.OffsetOf(typeof(SmoNetFrame_t), "dataLength");

//    public static int SmoNetFrameHeader_FrameTotalLength_offset = (int)Marshal.OffsetOf(typeof(SmoNetFrameHeader_t), "FrameTotalLength");

} //namespace SmoNetConstants

namespace SmoConstants {

//    /// smoKeyMask = 0xFFF8
    const ushort SmoKeyMask = 0xFFF8;
//deprecated, now it is the same with SmoKeyMask:     const ushort smoKeyAddressBitsMask = 0xFFF0;
//    static const byte FolderTypeMask = 0x07;
    const byte IsName24_bm = 0x04;
//    static const byte SmoHeadType_bm = 0x03; //old: SmoType_bm
//    static const byte SecType_bm = 0x03;
//    static const byte IsFolder_gc = 0x02;
//    static const byte IsCall_gc = 0x03;
//    static const byte IsParentToFlash_bm = 0x08;
    const byte IsSMOAddressInCode_bm = 0x08;
//    static const byte IsDataToFlash_bm = 0x01;
//    static const UInt16 EndTableKey = 0xFFFF; //EndTableMarker
//    static const UInt16 NewChild_with_noKey = 0xFFF0;

//    static const int IsString_bm = 0x80;
//    static const int IsControl_bm = 0x40;
//    static const int IsBit_bm = 0x20;
    const byte IsUnSigned_bm = 0x10;
//    static const int NumericDataLength_bm = 0x0F;
//    static const int StringLength_bm = 0x7F;
//    static const int BitPositionInByte_bm = 0x07;
//    static const int BitPositionInU32_bm = 0x1F;

//    //static const int ControlCodeLength_bm = 0x03;
//    static const int ControlInterface_bm = 0x3C;

//    static const int SmoDTMaxString = 127; //63;

//    static const string ShortNameDelimeters = "\0\"~!@#$^&*+/|\\?;<>=:({[¹"; // '"' + "~!@#$^&*+/|\\?;<>=:({[¹"; //\0x27 // ".,-" & space excluded
//    static const string PageNameReplacedChars = "-,."; //will be replaced by "_"

//    static const int IOTryCounter = 2;

//    static const int SmoPointerLength = 2;

//    static const int Name8_maxLength = 8;
//    static const int Name24_maxLength = 24;

    const int Smo16MaxLength = 16;
    const int Smo32MaxLength = 32;
//    static const int smoHeaderSize = 8;

//deprecated, use specific typedefs... see: smoSelectKey_t
//    static const int smoLinkKey_NotSelected_bm = 0x0001;
//    static const int smoLinkKey_RESERVED1_Mode_bm = 0x0002;
//    static const int smoLinkKey_RESERVED2_Mode_bm = 0x0004;

//    static const int smoLoginAccepted_bm = 0x0004;
//    static const int smoPassKey_userGroup_bm = 0x0003;

//    static const int smoTotalGroupsCount          = 4;

//    //static const int CrcByte_offset = 0;
//    static const int PKey_offset = 0;
//    static const int SecByte_offset = 2;
//    static const int Type_offset = 3;
//    static const int Data_offset = 4;
//    static const int CKey_offset = 6;
//    static const int Name_offset = 8;

//    static const int UserGroupMaskShift = 0x00;
//    static const int PowerUserGroupMaskShift = 0x02;
//    static const int OwnerGroupMaskShift = 0x04;
//    static const int TunerGroupMaskShift = 0x06;

    const int SmoControl_Help_code = 0;

} //namespace SmoConstants

#endif // SMOCONSTANTS

