// usage:
//    sets = new DESettings(parent, "filename");
// then read/update/save its properties!!!

#include "desettings.h"
#include "LogTree.h"

#include <QMessageBox>


DESettings::DESettings(MainWindow *parent = NULL, const char *configFName = NULL) : Config(parent, configFName)
{
    Parent = parent;
    Read(configmap);
    QVariant dest = QVariant::fromValue(this);
    ReadObjectFromFolder(&configmap, &dest, QString::fromLocal8Bit(SettingsJsonName));
}

DESettings::~DESettings()
{
    //save(QVariant::fromValue(this), SettingsJsonName);
}

