/****************************************************************************
**
** Copyright (C) 2014 Well and/or its subsidiary(-ies).
**
**
****************************************************************************/
//20170304 :added new feature: hid devices!!!

// on weekend to see: TODO, find out, Debug, mem, ???

/* to do/see next:
    Smo's info as ToolTip!!!
    Smo's int tracking!!!
*/

#include "MainWindow.h"
#include "zvvlib.h"

#include <QApplication>
#include <QMessageBox>
#include <qdebug.h>

/* QML... */
#include <qqml.h>
#include "fbitem.h"
//#include "smo.h"

using namespace std;

//call syntax: DE.exe [layout_settings_file.dat]
int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

//qml register...
    qmlRegisterType<FbItem>("QuickWidgetExample", 1, 0, "FbItem");

//DEPRECATED, use lib... help for The Qt Creator C++ code model...
//    qmlRegisterUncreatableType<SMO, 1>("SMO", 1, 0, "SMO", "SMO info");
//    qmlRegisterUncreatableType<Smo, 1>("SMO", 1,0, "__SMO__private", "SMO private, uncreatable");
//    qmlRegisterUncreatableType<DeviceInfo, 1>("SMO", 1, 0, "__Device__private", "Device info private, uncreatable");
//end qml register

    MainWindow mainWin(zvvlib::AppArguments(argc, argv));

    int retCode = -1;
    try {
        mainWin.show();
        retCode = app.exec();
    } catch (exception &e) {
        qDebug() << "main exception: " << e.what();
    } catch (...) {
        qDebug() << "main undefined exception!!!";
    }
    return retCode;
}
