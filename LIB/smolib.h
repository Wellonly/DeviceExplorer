#ifndef SMOLIB_H
#define SMOLIB_H
#include <sys/stat.h>
#include <QFileInfo>

#include "GlobalStatusErrorCodes.h"
#include "smofoldertype.h"

namespace SmoLib {

const char* SelectIconUponFileInfo(const QFileInfo& finfo, enum FolderTypes folder_type = GenericFolder);
bool stat_info_refresh(QFileInfo* finfo, struct stat *sinfo);

QString MainStatusCode2Descript(G8 mainCode);

} //namespace SmoLib
//using namespace SmoLib;

#endif // SMOLIB_H
