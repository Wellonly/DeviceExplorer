#ifndef SMODATATYPEDEFS_H
#define SMODATATYPEDEFS_H
#include "zvvDataTypes.h"
#include "SmoRecord.h"

typedef struct {
    u8  ButtonValue;
} ControlButton_t;

typedef struct {
    smokey  key;
} ControlSelectItem_t; //0x41, /*datasize: 2; SelectTdi() is a handler for this */

typedef struct_packed { //struct_packed must be for result size of 3(without it's 4)
    smokey   Loginkey;
    union {
        u8   UserGroup;
        char UserPassStr[1]; //why: flexible array member in union forbidden???
    };
} ControlLogin_t; // 0x43,/*datasize: answer:3, request:1+ */

typedef struct {
    union {
        u8   UserGroup;
        char UserPassStr[1]; //why: flexible array member in union forbidden???
    };
} ControlPassLine_t; // 0x44, /*datasize: answer:1, request:1+ */

typedef struct { //TODO:it
} ControlIPv4_t; //= 0x47, /*instead: IPv4Address  = 0x07   */

#endif // SMODATATYPEDEFS_H
