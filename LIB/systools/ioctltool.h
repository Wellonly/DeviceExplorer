#ifndef IOCTLTOOL_H
#define IOCTLTOOL_H

#ifdef __cplusplus
extern "C" {
#endif

int ioctl_by_name(const char* filename, int openflags, unsigned long request, void* data);


#ifdef __cplusplus
}
#endif

#endif // IOCTLTOOL_H
