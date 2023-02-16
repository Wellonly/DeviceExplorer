
//#include <QComboBox>
#include <QString>

//#include "SerialPort.h"
#include "LogTree.h"
#include "zvvlib.h"
#include "zvvdebug.h" //#include <QDebug>

#include "smolib.h" //NOTE: smolib.h MUST BE BEFORE g8enum.h
#include "g8enum.h" //NOTE: smolib.h MUST BE BEFORE g8enum.h

namespace SmoLib {

const char *SelectIconUponFileInfo(const QFileInfo &finfo, enum FolderTypes folder_type)
{
    const char* cImage; //QString cImage;
    if (finfo.isSymLink()) {
        cImage = "control"; /* "link" */
    }
    else if (finfo.isDir())
    {
        if (finfo.isHidden()) {
            cImage = "group0";
        }
        else if ((folder_type == LinkToDevice) || finfo.isRoot()) {
            cImage = "device";
        }
        else {
            cImage = "folder";
        }
    }
    else if (finfo.isFile())
    { // file
        if (finfo.isExecutable()) {
            cImage = "blocks";
        }
        else {
            cImage = "data";
        }
    }
    else {
        cImage = "warning";
    }
    return cImage;
}

bool stat_info_refresh(QFileInfo *finfo, struct stat *sinfo)
{
    const char* fname = finfo->absoluteFilePath().toUtf8().constData();
    if (stat(fname, sinfo)) {
        sinfo->st_dev = 0;
        return false;
    }
    return true;
}

QString MainStatusCode2Descript(G8 mainCode)
{
    if (mainCode == G8::msg_ZeroString) return QString("");
    auto retv = G8Enum::G8MetaEnum.valueToKey((int)mainCode);
    if (retv != 0) return QString::fromLatin1(retv);
    return QString("undefined G8 code: 0x%1").arg((int)mainCode, 0, 16);
}


//void ShowSmoInComboBox(QComboBox* combox, SMO* smo)
//{
//    QVariant svar = QVariant::fromValue(smo);
//    int cind = combox->findData(QVariant::fromValue(svar));
//    combox->blockSignals(true);
//    if (cind >= 0) {
//        combox->setCurrentIndex(cind);
//    }
//    else {
//        combox->addItem(smo->icon(0), SMO::ShortName(smo), svar);
//    }
//    combox->setCurrentText(selectedItemText); //SMO::TargetMemoryToString(smo)
//    combox->blockSignals(false);
//}

} //namespace SmoLib
