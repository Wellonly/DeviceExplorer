

#include "deplugin.h"

//DEPlugin::DEPlugin(const QString& fName, QObject *parent) : QObject(parent)
//{
//    pluginFName = fName;
//}

DEPlugin::DEPlugin(const QString& appNameSpace, const QString& appFormName, QObject* parent) : QObject(parent)
{
    AppLink = appNameSpace + "." + appFormName;
}

DEPlugin::~DEPlugin()
{

}

