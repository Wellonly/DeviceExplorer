#ifndef ZVVCOMPILER_H
#define ZVVCOMPILER_H
// ZVVCOMPILER for QT
#include <stdint-gcc.h>
#include <stddef.h>
#include <stdbool.h>
#include "c_cpp_macros.h"

#define Packed       __attribute__ ((packed))

#define struct_packed struct __attribute__ ((packed))
#define union_packed  union  __attribute__ ((packed))
#define enum_packed   enum   __attribute__ ((packed))

/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __EXTERN_C_BEGIN extern "C" {
# define __EXTERN_C_END   }
#else
# define __EXTERN_C_BEGIN
# define __EXTERN_C_END
#endif


//#include <QString>
//#include <QDateTime>
//class QString;
//class QDateTime;
//typedef QString string;
//typedef QDateTime DateTime;

//#define DateTime QDateTime

#define QT 1 /* this file only for QT */

#define CPUBits            32 /* 64 */
#define CPUAlign           1  /*(CPUBits / 8)*/

//deprecated, use intp... #define cpuInt             int /*int64_t*/
//deprecated, use uintp... #define uintp            unsigned int /*uint64_t*/
typedef int intp;
typedef unsigned int uintp;

#define SizeOfInt   sizeof(int) /*___int_size_t_h*/

//find out why this not working: #define null NULL

#define nullstr QString()

#define __flash

#define FLASH(x) static x

#define Offsetof offsetof

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

#ifdef QT_DEBUG
# define Assert(expr) \
  {\
    if (!(expr)) while (true);\
  }
#else
#  define Assert(expr) ((void) 0)
#endif

#define ALIGN(x) __attribute__((__aligned__(x)))

#include "zvvDataTypes.h" //must be here because it uses these definitions!!!
#endif // ZVVCOMPILER_H

