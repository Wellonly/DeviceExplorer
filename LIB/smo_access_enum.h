#ifndef SMO_ACCESS_ENUM_H
#define SMO_ACCESS_ENUM_H

#ifdef WINNT /*win32*/
enum __attribute__ ((packed))
#else
enum
#endif
SmoAccess {
    NoDataAccess = 0,
    ReadOnly     = 1,
    FullAccess   = 2,
    Invisible    = 3
};
#endif // SMO_ACCESS_ENUM_H
