#ifndef SMO_INFO_H
#define SMO_INFO_H

#include <QObject>
#include <QVariant>

#include "SmoRecord.h"

/// \brief The SMO class contains the Smo class related info!!!
class SMO : public QObject
{
    Q_OBJECT

public:
    SMO() {} //NOTE: SMOFlags() {} need for metasystem!!!
    SMO(const SMO& other) : QObject(other.parent()) {}

    static const Qt::ConnectionType Connection_unique_auto;
    static const Qt::ConnectionType Connection_unique_queued;

    Q_INVOKABLE QVariant tstFunc(QObject* smoFolder);

    enum WriteStatus {
        WriteOk = 0,
        WriteBadValue = 1,
        WriteIOError = 2,
    };
    Q_ENUM(WriteStatus)

    enum Flags {
//        _AllClear       = 0,      /* old: Clear */
        FlagIOk        = (1<<0), /* Ok equals: isCallSuccessful */
        FlagIOData         = (1<<1),
        FlagIOStatus       = (1<<2),
          FlagIOError        = (1<<3),
        FlagIOMessage  = (1<<4),
          FlagIntegrityError = (1<<5),
        FlagEndTable       = (1<<6),
        FlagInvisible      = (1<<7),

        FlagFolder             = (1<<8),
        FlagAllChildrenRead    = (1<<9),
        FlagHasInvisibleChild  = (1<<10),
          FlagRefreshedWithError = (1<<11),

//        Changed        = (1<<1),
//        IsRefreshInProcess   = (1<<8),
//        NotAnswer      = (1<<6),
//        CRCError       = (1<<5),
//        NoDevice       = 2,
    };
    Q_DECLARE_FLAGS(SMOFlags, Flags)
    Q_ENUM(Flags)
    Q_FLAG(SMOFlags)

    enum HeadType {
        TypeConstData = SmoHeadType_Data,
        TypeData      = SmoHeadType_DataPointer,
        TypeFolder    = SmoHeadType_Folder,
        TypeProperty  = SmoHeadType_Property
    };
    Q_ENUM(HeadType)

    enum FolderType {
        FolderGeneric      = FolderTypes::GenericFolder,
        Folder1            = FolderTypes::FolderType1,
        Folder2            = FolderTypes::FolderType2,
        Folder3            = FolderTypes::FolderType3,
        Folder4            = FolderTypes::FolderType4,
        FolderLink         = FolderTypes::LinkToFolder,
        FolderLinkToDevice = FolderTypes::LinkToDevice,
        FolderNet          = FolderTypes::NetFolder
    };
    Q_ENUM(FolderType)

    enum DataType {
        DtUndefined = Undefined,
        DtVoid = Void,

        DtS8 = S8,
        DtU8 = U8,

        DtS16 = S16,
        DtU16 = U16,

        DtRGB = RGB,
        DtU24 = U24,

        DtS32 = S32,
        DtU32 = U32,

        DtFloat32   = Float32,
        DtPointer32 = Pointer32,

        DtDateBSD32  = DateBSD32, /* or QDate() */
        DtRealTime32 = RealTime32, /* 1 == 30.517578125 uS; max~= 36 hours */

        DtS64 = S64,
        DtU64 = U64,

        DtFloat64    = Float64,
        DtDateTime64 = DateTime64,

        DtDecimal64  = Decimal64,
        DtPointer64  = Pointer64,

        DtPointer16    = Pointer16,
        DtU128         = U128,

        DtSizedU8Data  = SizedU8Data, /*datasize:1..256(0xFF+1)*/
        DtSizedU32Data = SizedU32Data, /*datasize:4..(0xFFFFFFFF+4); also used as a file!!! */

        DtSzData128    = SzData128, /* data: with size: up to 128 bytes with zero-ended compression! */
        DtSzData256    = SzData256, /* data: with size: up to 256 bytes with zero-ended compression! */

        DtSzData512    = SzData512, /* data: with size: up to 512 bytes with zero-ended compression! */
        DtSzData1024   = SzData1024, /* data: with size: up to 1024 bytes with zero-ended compression! */

        DtSzData2048   = SzData2048, /* data: with size: up to 2048 bytes with zero-ended compression! */
        DtSzData4096   = SzData4096, /* data: with size: up to 4096 bytes with zero-ended compression! */

        DtSzData8192   = SzData8192, /* may be unused(RESERVED) on 20170312 */
        DtSzData16384  = SzData16384, /* may be unused(RESERVED) on 20170312 */

//NOTE: Control's group has higher priority and moved up here(optimized for combobox in the builder)
//NOTE: The Controls typedefs defined in smoDataTypedefs.h
        DtControlButton     = ControlButton,
        DtControlSelectItem = ControlSelectItem,
        DtControlLogin      = ControlLogin,      /*datasize: answer:3, request:1+ */
        DtControlPassLine   = ControlPassLine,   /*datasize: answer:1, request:1+ */
        DtControlIPv4       = ControlIPv4,       /*instead: IPv4Address  = 0x07   */

        DtSiblingKey        = SiblingKey, /* Links folder sibling items in different sectors */
        DtEnumerator        = Enumerator,

//bits...
        DtBit0 = Bit0,
        DtBit1 = Bit1,
        DtBit2 = Bit2,
        DtBit3 = Bit3,
        DtBit4 = Bit4,
        DtBit5 = Bit5,
        DtBit6 = Bit6,
        DtBit7 = Bit7,

        DtBit8 = Bit8,
        DtBit9 = Bit9,
        DtBit10 = Bit10,
        DtBit11 = Bit11,
        DtBit12 = Bit12,
        DtBit13 = Bit13,
        DtBit14 = Bit14,
        DtBit15 = Bit15,

        DtBit16 = Bit16,
        DtBit17 = Bit17,
        DtBit18 = Bit18,
        DtBit19 = Bit19,
        DtBit20 = Bit20,
        DtBit21 = Bit21,
        DtBit22 = Bit22,
        DtBit23 = Bit23,

        DtBit24 = Bit24,
        DtBit25 = Bit25,
        DtBit26 = Bit26,
        DtBit27 = Bit27,
        DtBit28 = Bit28,
        DtBit29 = Bit29,
        DtBit30 = Bit30,
        DtBit31 = Bit31,

        DtZeString = ZeString, /*zero ended string with max size software depended!!! */
/// next: sized data(string)...
        DtSized1 = SzData1, /*sized data(string), max size 1byte with zero-ended compression! */
        DtSized2 = SzData2, /*sized data(string), max size 2byte with zero-ended compression!, and so on */
        DtSized3 = SzData3, /* and so on... */
        DtSized4 = SzData4,
        DtSized5 = SzData5,
        DtSized6 = SzData6,
        DtSized7 = SzData7,
        DtSized8 = SzData8,
        DtSized9 = SzData9,
        DtSized10 = SzData10,
        DtSized11 = SzData11,
        DtSized12 = SzData12,
        DtSized13 = SzData13,
        DtSized14 = SzData14,
        DtSized15 = SzData15,
        DtSized16 = SzData16,
        DtSized17 = SzData17,
        DtSized18 = SzData18,
        DtSized19 = SzData19,
        DtSized20 = SzData20,
        DtSized21 = SzData21,
        DtSized22 = SzData22,
        DtSized23 = SzData23,
        DtSized24 = SzData24,
        DtSized25 = SzData25,
        DtSized26 = SzData26,
        DtSized27 = SzData27,
        DtSized28 = SzData28,
        DtSized29 = SzData29,
        DtSized30 = SzData30,
        DtSized31 = SzData31,
        DtSized32 = SzData32,
        DtSized33 = SzData33,
        DtSized34 = SzData34,
        DtSized35 = SzData35,
        DtSized36 = SzData36,
        DtSized37 = SzData37,
        DtSized38 = SzData38,
        DtSized39 = SzData39,
        DtSized40 = SzData40,
        DtSized41 = SzData41,
        DtSized42 = SzData42,
        DtSized43 = SzData43,
        DtSized44 = SzData44,
        DtSized45 = SzData45,
        DtSized46 = SzData46,
        DtSized47 = SzData47,
        DtSized48 = SzData48,
        DtSized49 = SzData49,
        DtSized50 = SzData50,
        DtSized51 = SzData51,
        DtSized52 = SzData52,
        DtSized53 = SzData53,
        DtSized54 = SzData54,
        DtSized55 = SzData55,
        DtSized56 = SzData56,
        DtSized57 = SzData57,
        DtSized58 = SzData58,
        DtSized59 = SzData59,
        DtSized60 = SzData60,
        DtSized61 = SzData61,
        DtSized62 = SzData62,
        DtSized63 = SzData63,
        DtSized64 = SzData64,
        DtSized65 = SzData65,
        DtSized66 = SzData66,
        DtSized67 = SzData67,
        DtSized68 = SzData68,
        DtSized69 = SzData69,
        DtSized70 = SzData70,
        DtSized71 = SzData71,
        DtSized72 = SzData72,
        DtSized73 = SzData73,
        DtSized74 = SzData74,
        DtSized75 = SzData75,
        DtSized76 = SzData76,
        DtSized77 = SzData77,
        DtSized78 = SzData78,
        DtSized79 = SzData79,
        DtSized80 = SzData80,
        DtSized81 = SzData81,
        DtSized82 = SzData82,
        DtSized83 = SzData83,
        DtSized84 = SzData84,
        DtSized85 = SzData85,
        DtSized86 = SzData86,
        DtSized87 = SzData87,
        DtSized88 = SzData88,
        DtSized89 = SzData89,
        DtSized90 = SzData90,
        DtSized91 = SzData91,
        DtSized92 = SzData92,
        DtSized93 = SzData93,
        DtSized94 = SzData94,
        DtSized95 = SzData95,
        DtSized96 = SzData96,
        DtSized97 = SzData97,
        DtSized98 = SzData98,
        DtSized99 = SzData99,
        DtSized100 = SzData100,
        DtSized101 = SzData101,
        DtSized102 = SzData102,
        DtSized103 = SzData103,
        DtSized104 = SzData104,
        DtSized105 = SzData105,
        DtSized106 = SzData106,
        DtSized107 = SzData107,
        DtSized108 = SzData108,
        DtSized109 = SzData109,
        DtSized110 = SzData110,
        DtSized111 = SzData111,
        DtSized112 = SzData112,
        DtSized113 = SzData113,
        DtSized114 = SzData114,
        DtSized115 = SzData115,
        DtSized116 = SzData116,
        DtSized117 = SzData117,
        DtSized118 = SzData118,
        DtSized119 = SzData119,
        DtSized120 = SzData120,
        DtSized121 = SzData121,
        DtSized122 = SzData122,
        DtSized123 = SzData123,
        DtSized124 = SzData124,
        DtSized125 = SzData125,
        DtSized126 = SzData126,
        DtSized127 = SzData127,
    };
    Q_ENUM(DataType)

};
Q_DECLARE_METATYPE(SMO)

//class SMOTypes : public QObject
//{
//    Q_OBJECT
//public:
//    SMOTypes() {} //NOTE: SMOFlags() {} need for metasystem!!!
//    SMOTypes(const SMOTypes& other) : QObject(other.parent()) {}

//    enum SmoType {
//        ConstData = SmoHeadType_Data,
//        Data      = SmoHeadType_DataPointer,
//        Folder    = SmoHeadType_Folder,
//        Property  = SmoHeadType_Property
//    };
//    Q_ENUM(SmoType)

////    Q_DECLARE_FLAGS(SmoTypeFlags, SmoType)
////    Q_FLAG(SmoTypeFlags)
//};
//Q_DECLARE_METATYPE(SMOTypes)

//class SMOFolderTypes : public QObject
//{
//    Q_OBJECT
//public:
//    SMOFolderTypes() {} //NOTE: SMOFlags() {} need for metasystem!!!
//    SMOFolderTypes(const SMOFolderTypes& other) : QObject(other.parent()) {}

//    enum FolderType {
//        Generic      = FolderTypes::GenericFolder,
//        Folder1      = FolderTypes::FolderType1,
//        Folder2      = FolderTypes::FolderType2,
//        Folder3      = FolderTypes::FolderType3,
//        Folder4      = FolderTypes::FolderType4,
//        LinkedFolder = FolderTypes::LinkToFolder,
//        LinkedDevice = FolderTypes::LinkToDevice,
//        Net          = FolderTypes::NetFolder
//    };
//    Q_ENUM(FolderType)
//};
//Q_DECLARE_METATYPE(SMOFolderTypes)

//Find out what this does not work!!!
//class SMODataTypes : public QObject
//{
//    Q_OBJECT
//public:
//    SMODataTypes() {} //NOTE: SMOFlags() {} need for metasystem!!!
//    SMODataTypes(const SMODataTypes& other) : QObject(other.parent()) {}
//#undef _SMODATATYPES_ENUM_H
//#include "smodatatypes.h"
//    Q_DECLARE_FLAGS(DataTypeFlags, SmoDataType)
//    Q_ENUM(SmoDataType)
//    Q_FLAG(DataTypeFlags)
//};
//Q_DECLARE_METATYPE(SMODataTypes)
//Q_DECLARE_METATYPE(SMODataTypes*)
//Q_DECLARE_METATYPE(const SMODataTypes*)

//namespace SMOEnums {
//    Q_NAMESPACE
////#undef _SMODATATYPES_ENUM_H
////#include "smodatatypes_enum.h"
//    Q_ENUM_NS(SmoDataType)
//}
//  qmlRegisterUncreatableMetaObject(MyNamespace::staticMetaObject, "io.qt", 1, 0, "MyNamespace", "Access to enums & flags only");
//Q_DECLARE_METATYPE(SmoDataType)
//Q_DECLARE_METATYPE(SmoDataType*)
//Q_DECLARE_METATYPE(const SmoDataType*)

typedef struct _LoginAndGroup
{
    smokey   Loginkey;
    u8       LoginGroup;
} LoginAndGroup_t;

typedef enum SmoUITypes
{ /*see: Smo::UIType() */
    uiUndefined  = 0,
    uiPageHeader = 1,
    uiTextLine   = 2,
    uiCheckBox   = 3,
    uiDataTime   = 4,
    uiNumeric    = 5,
    uiMessage    = 6,
//never occurs, see UIType()...   uiUnsupportedControl = 7,
    uiSelectItem = SmoDataType::ControlSelectItem,
    uiLogin      = SmoDataType::ControlLogin,
    uiPassLine   = SmoDataType::ControlPassLine,
}SmoUITypes;

enum PathOption {
    path_less = 0,
    path_relative = 1,
    path_absolute = 2,
};

typedef SMO::Flags SmoFlags;

#endif // SMO_INFO_H
