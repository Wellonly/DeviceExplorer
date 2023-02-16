
#ifdef linux
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <unistd.h>

#include "ioctltool.h"

/// \brief ioctl_by_name
/// \param filename      ex:"/dev/sda"
/// \param openflags     O_RDONLY or O_RDWR
/// \param request       ioctl's
/// \param data          in/out
/// \return ioctl        ret value
int ioctl_by_name(const char* filename, int openflags, unsigned long request, void* data) {
  int fd = open(filename, openflags);
  if (fd==-1) {
    return -1;
  }
  int result = ioctl(fd, request, data);
  close(fd);
  return result;
}
#endif
