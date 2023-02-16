#ifndef LOCALHOSTWIDGET_H
#define LOCALHOSTWIDGET_H

#include <QTreeWidget>
#include <QSerialPortInfo>

#include "dewidget.h"

class Smo;
class DEPlugin;
class DeviceInfo;
class MainTreeWidget;
class QHidDeviceInfo;

class LocalHostWidget : public DEWidget
{
    Q_OBJECT

public:
    LocalHostWidget(MainTreeWidget *parent = 0, Smo* smo = 0, const QString& idname = QString("DE.LocalHost"));
    ~LocalHostWidget();

signals:

public slots:

private slots:
    void addHost();
    void refreshPorts(Smo *localHostSmo);

private:
    MainTreeWidget *mainTreeWidgetPtr;

    QList<QHidDeviceInfo> hids;

    DEPlugin *ComPortsPlugin;
    DEPlugin *HidDevicePlugin;
//    DeviceInfo *HostDeviceInfo;

    void Form_refresh(Smo *smo = 0, bool isFormToBeRefreshed = false) override;

};

#endif // LOCALHOSTWIDGET_H
