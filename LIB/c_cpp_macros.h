#ifndef _C_CPP_MACROS_H
#define _C_CPP_MACROS_H

#define iif(a,b,c) ((a) ? (b) : (c))

//NOTE: note More optimized if only used with values known at compile time.
#define Abs(a)              (((a) <  0 ) ? -(a) : (a))

//NOTE: note More optimized if only used with values known at compile time.
#define Min(a, b)           (((a) < (b)) ?  (a) : (b))

//NOTE: note More optimized if only used with values known at compile time.
#define Max(a, b)           (((a) > (b)) ?  (a) : (b))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*(x)))
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)
#endif

#define IsOffsetInRange(coff, soff, range) ((char*)(coff) >= (char*)(soff) && ((char*)(coff) - (char*)(soff)) < (range))
#define IsIndexInRange(x, minv, maxv) (((x) >= (minv)) && ((x) < (maxv)))

#define between_or_equal(x, minv, maxv) (((x) >= (minv)) && ((x) <= (maxv))) /* include max & min */
#define between_or_min_equal(x, minv, maxv) (((x) >= (minv)) && ((x) < (maxv)))
#define between_not_equal(x, minv, maxv) (((x) > (minv)) && ((x) < (maxv)))

#define hex2char(hex) (iif((hex) > 9, (hex)+0x57, (hex)+0x30)) /* input: 0..15; out: '0'...'f' */
#define hex2CHAR(hex) (iif((hex) > 9, (hex)+0x37, (hex)+0x30)) /* input: 0..15; out: '0'...'F' */
#define char2hex(chr) (iif((chr) >= 0x41, (chr)-0x57, (chr)-0x30)) /* input: '0'...'f'; out: 0..15 */
#define CHAR2hex(chr) (iif((chr) >= 0x41, (chr)-0x37, (chr)-0x30)) /* input: '0'...'F'; out: 0..15 */

#define str2toNumber(s) (s[0] + (s [1] * 0x100))
#define str4toNumber(s) (s[0] + (s [1] * 0x100) + (s [2] * 0x10000) + (s [3] * 0x1000000))

/// multiplicate by shift left...
#define mulby2(x)   ((x) << 1)
#define mulby4(x)   ((x) << 2)
#define mulby8(x)   ((x) << 3)
#define mulby16(x)  ((x) << 4)
#define mulby32(x)  ((x) << 5)
#define mulby64(x)  ((x) << 6)
#define mulby128(x) ((x) << 7)
#define mulby256(x) ((x) << 8)
#define mulby512(x) ((x) << 9)

/// divide by shift right...
#define divby2(x)   ((x) >> 1)
#define divby4(x)   ((x) >> 2)
#define divby8(x)   ((x) >> 3)
#define divby16(x)  ((x) >> 4)
#define divby32(x)  ((x) >> 5)
#define divby64(x)  ((x) >> 6)
#define divby128(x) ((x) >> 7)
#define divby256(x) ((x) >> 8)
#define divby512(x) ((x) >> 9)

#define bitsCLEAR(bits, value) (bits &= ~(value)) /* clear bits and returns new value */
#define bitsSET(bits, value)   (bits |= (value))  /* set bits and returns new value */

#endif // _C_CPP_MACROS_H

