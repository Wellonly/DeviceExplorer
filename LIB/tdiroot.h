#ifndef TDIROOT_H
#define TDIROOT_H
#include <stdint.h>

typedef union TDIInfo {
    struct {
        uint8_t SmoBusVersion;
        uint8_t SmoVersion;
        uint16_t TDICharset;
        uint32_t TdiCRC;
    };
    uint16_t TDIVersion;
    uint64_t all;
}TDIInfo_t;

#endif // TDIROOT_H

