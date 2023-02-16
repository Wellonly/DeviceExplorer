/**************************************************
 * Author: Valentin Zotov Copyright 2011
 * support: wellman@mail.ru; eautomatic@mail.ru
 * Title: GlobalStatusErrorCodes.h
 **************************************************/
#ifndef GlobalStatusErrorCodes_h__
#define GlobalStatusErrorCodes_h__
//#ifdef __cplusplus
//extern "C" {
//#endif

#define MainStatus_length                1
#define MainStatus_ExtendedByte_length   2
#define MainStatus_ExtendedWord_length   3
#define MainStatus_ExtendedDWord_length  5

// Acknowledge formats begin ///
#define AckFormat_OnDefault              0
#define AckFormat_Main                   1
#define AckFormat_Extended2              2
#define AckFormat_Extended4              4
// end Acknowledge formats ///

#define StatusCode_without_severity_mask 0x3F
#define Severity_mask                    0xC0

#define Severity_Command_gc              0x00
#define Severity_Info_gc                 0x40
#define Severity_Message_gc              0x40
#define Severity_Warning_gc              0x80
#define Severity_Error_gc                0xC0

/* Special codes:
   msg_EditMode       used by editor: set in before edit event & after edit event!!!
   msg_Silence        used as DO NOT answer! now mutex... an answer will be later or voided!!!
*/

//next used in smo_lib.c->smoDoPacket_as()
#define IsGlobalStatusAsError(value_G8) ((u8)(value_G8) >= 0xC0)
#define IsGlobalStatusAsWarning(value_G8) (((u8)(value_G8) & Severity_mask) == Severity_Warning_gc)
#define IsGlobalStatusAsErrorOrWarning(value_G8) ((u8)(value_G8) & 0x80)

#define IsStatusTextual(value_G8) (!((u8)(value_G8) & StatusCode_without_severity_mask))
#define GetSeverity_groupCode(value_G8)  ((u8)(value_G8) & Severity_mask)

// enum SeverityCode
//  {
// 	Severity_Success       = 0,
// 	Severity_Informational = 1,
// 	Severity_Warning       = 2,
// 	Severity_Error         = 3,
//  };

///Messages Plan:                       message codes: 0x00..0x7F (divided into 2 groups: command codes: 0x01..0x3F & info codes: 0x40..0x7F)
///                                     warning codes: 0x80..0xBF
///                                     error codes:   0xC0..0xFF

///WARNING: "as byte" used by parser!!!!
typedef
enum G8 { //as byte //qt: typedef added!!! but CS does not understand: typedef !!! so there is no solve yet... may be try to typedef this enum in a QT file: e.g.: typedef G8 G8;
// message codes: 0x00..0x7F begin /////////////////////////////////////////////
    msg_Success                     =0x00,
// command codes: 0x01..0x3F begin ///////////////////////////////////////////// 
// Codes 0x01..0x10 used in SMO Controls by free definitions ///////////////////
    msg_UserCode0x01                =0x01,
    msg_UserCode0x10                =0x10,
    msg_StopPlay                    =0x11,
    msg_ExtendedByteCode            =0x12,  //+1byte user code
    msg_ExtendedWordCode            =0x13,  //+2bytes
    msg_ExtendedDWordCode           =0x14,  //+4bytes
    msg_WindowsAPICode              =0x15,  //+4bytes windows Win32 API code, see code definitions for the Win32 API!!!
    msg_Save                        =0x16,
    msg_Delete                      =0x17,
    msg_Read                        =0x18,
    msg_Write                       =0x19,

// info codes:    0x40..0x7F begin /////////////////////////////////////////////
    msg_TextualMessage              =0x40,  //+nbytes may be not zero ended string only if after it no more status codes
    msg_NotReady                    =0x41,
    msg_ZeroString                  =0x42,
    msg_WaitForLineRelease          =0x43,
    msg_NotInOurAddress             =0x44,
    msg_EndOnTimeOut                =0x45,
    msg_EndOnNeg_dV                 =0x46,
    msg_EndOnMaxVoltage             =0x47,
    msg_EndOnMinVoltage             =0x48,
    msg_EndOnMinCurrent             =0x49,
    msg_EndOnDeadLine               =0x4A,
    msg_Engaged                     =0x4B, //static char __flash cMsgEngaged[]=
    msg_NotEngaged                  =0x4C, //static char __flash cMsgNotEngaged[]=
    msg_Idle                        =0x4D,
    msg_ImReady                     =0x4E,
    msg_Executed                    =0x4F,
    msg_OK                          =0x50,
    msg_Load                        =0x51, //msg_LoaderMode
    msg_Blank                       =0x52, //prev. was:    msg_Space						=0x52,
    msg_NewTask                     =0x53,
    msg_Cancel                      =0x54,
    msg_Preliminary                 =0x55, //previous: msg_RunPrecharge
    msg_Pause                       =0x56,
    msg_Transition                  =0x57, //msg_PassToCharge
    msg_EditMode                    =0x58,
    msg_Confirm                     =0x59,
    msg_Activation                  =0x5A,
    msg_ReStart                     =0x5B,
    msg_Stop                        =0x5C,
    msg_Play                        =0x5D,
    msg_Off                         =0x5E,
    msg_ExtendedParameters          =0x5F,
    msg_Forward                     =0x60,
    msg_Backward                    =0x61,
    msg_Silence                     =0x62,	//old: msg_DoNotAnswer WARNING: its code prevents answer to host!!! (bypass answer)
    msg_Minimum                     =0x63,
    msg_Maximum                     =0x64,
    msg_Charge                      =0x65,
    msg_Discharge                   =0x66,
//need or not:    msg_Processing                  =0x65,

//    msg_NewInComingFrame			=0x63,
//    msg_Abort                       =0x59, //msg_ProcessInitAborted
//    msg_Run							=0x4D,
//     msg_RunCharge					=0x4F,
//     msg_RunDischarge				=0x50,
//    msg_ToDo						=0x43,
//    msg_RunFast                     =0x5F,
//    msg_Calibration					=0x02,
//    msg_ArrayAccess                 =0x05,
//    msg_AnswerFrame					=0x21,
//    msg_WaitForSlaveAnswer			=0x22,

// warning codes: 0x80..0xBF begin /////////////////////////////////////////////
    wrn_TextualWarning              =0x80,  //+nbytes may be not zero ended string only if after it no more status codes
    wrn_BadParameters               =0x81,
    wrn_AccessDenied                =0x82,
    wrn_TheEnd                      =0x83,
    wrn_NotFound                    =0x84,
    wrn_NotSelected                 =0x85, //may be removed... use msg_Blank instead!!!
    wrn_NotResponded                =0x86,
    wrn_Busy                        =0x87,
    wrn_VoltageDrop                 =0x88,
    wrn_OverLoad                    =0x89,
    wrn_OverVoltage                 =0x8A,
    wrn_DeadLine                    =0x8B,
    wrn_ShortCircuit                =0x8C,
    wrn_BadCircuit                  =0x8D,
    wrn_InputOverVoltage            =0x8E,
    wrn_LowTemperature              =0x8F,
    wrn_HighTemperature             =0x90,
    wrn_Overflow                    =0x91,
    wrn_LowBattery                  =0x92,
    wrn_Doubled                     =0x93,

//    err_NoTask						=0xCD,	//
//    msg_SignatureWrong              =0x5A,
//    msg_LockingAborted              =0x5B,
//    msg_ErasingAborted              =0x5C,
//    msg_WriteUnSuccess              =0x5D,
//    msg_UnExecutable                =0x5E, //Zero addresses and so on...

// error codes:   0xC0..0xFF ///////////////////////////////////////////////////
    err_TextualError                =0xC0,  //+nbytes may be not zero ended string only if after it no more status codes
    err_Undefined                   =0xC1,  //undefined error
    err_Unsupported                 =0xC2,  //static char __flash cErrUnsupported[]="NoSupportDT";	//to be or not to be: DT - ?
    err_WrongCRC                    =0xC3,  //static char __flash cErrWrongCRC[]=;
    err_ReadOnly                    =0xC4,  //static char __flash cErrReadOnly[]="ReadOnly";
    err_PowerNotGood                =0xC5,
    err_NoEnoughSpace               =0xC6,
    err_Writing                     =0xC7,
//unused yet...   old: err_RetStringTooLong            =0xC8,
    err_Range                       =0xC9,  //err_RangeUndefined
    err_BadIndex                    =0xCA,
    err_DrvWrongNumber              =0xCB,  //static char __flash cErrDrvWrongNumber[]="WrongDrvNum";
    err_Timeout                     =0xCC,  //static char __flash cErrTimeout[]="ByteTimeout";
    err_NotConnected                =0xCD,  //static char __flash cErrNotConnected[]=;
    err_LineError                   =0xCE,
    err_DataLength                  =0xCF,
    err_BufferOverflow              =0xD0,
    err_WrongAddress                =0xD1,
    err_BadCommand                  =0xD2,  //static char __flash cErrBadCommand[]="BadCommand";
    err_IOError                     =0xD3,
    err_IntegrityError              =0xD4,

}

G8;

//#ifdef __cplusplus
//}
//#endif
#endif // GlobalStatusErrorCodes_h__
