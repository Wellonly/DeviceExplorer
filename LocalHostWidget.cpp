
#include <QGridLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QHostInfo>
#include <QDebug>

#include "treeitem.h"
#include "MainTreeWidget.h"
#include "qhidapi.h"

#include "app_config.h"
#include "LocalHostWidget.h"
#include "smo.h"
#include "deviceinfo.h"
#include "deplugin.h"
#include "tdiporthid.h"
#include "tdiportserial.h"
#include "treelib.h"

LocalHostWidget::LocalHostWidget(MainTreeWidget *mainTree, Smo *smo, const QString &idname)
: DEWidget((QMainWindow*)mainTree->parentWidget(), smo, idname)
{
    mainTreeWidgetPtr = mainTree;

    QGridLayout *gridLayout = zvvlib::GridLayout(this);

    QPushButton *addHostButton = new QPushButton(tr("Add a host..."), this);
    connect(addHostButton, SIGNAL(clicked()), this, SLOT(addHost()));
    gridLayout->addWidget(addHostButton, 0, 0);

    QPushButton *refreshPortsButton = new QPushButton(tr("Refresh ports"), this);
    connect(refreshPortsButton, &QPushButton::clicked, [this](){Smo* smo = smoRef(); iif(smo->isExpanded(), Form_refresh(smo, true), smo->setExpanded(true));});
    gridLayout->addWidget(refreshPortsButton, 1, 0);

    ComPortsPlugin = new DEPlugin("DE","fCOMPort", this);
    HidDevicePlugin= new DEPlugin("DE","fHIDPort", this);

    Smo::SetIcon(smo, "PC");
    smo->setToolTip(0, tr("local host"));
    smo->setExpandedWidget(this);
    smo->setCollapsedWidget(this);
    smo->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
    mainTreeWidgetPtr->getTree()->insertTopLevelItem(0, smo->treeItem());
//    HostDeviceInfo = 0; //new DeviceInfo(smo);

    smo->setExpanded(true); // Why hasn't it e mit event to refresh this???
    Form_refresh(smo, true);
}

LocalHostWidget::~LocalHostWidget()
{
}

void LocalHostWidget::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    if (isFormToBeRefreshed) {
        if (smo->isExpanded()) {
            refreshPorts(smo);
        }
    }
}

//Q_DECLARE_METATYPE(class QSerialPortInfo)
//TODO: friendly name for serial & hid devices!!!
void LocalHostWidget::refreshPorts(Smo* localHostSmo)
{
    int istart = 0;
    QVector<int> foundlist(localHostSmo->childCount(), 0);

    // add serial devices...
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString portname = info.description(); /*+ "(" + info.portName() + ")";*/
        int idx = TreeLib::FindChildIndexOnText<Smo>(localHostSmo, portname, istart);
        if (idx >= 0) {
            if (idx < foundlist.count()) foundlist[idx] = 1;
            if (idx == istart) istart++;
        }
        else
        {
            Smo *portNode = new Smo(localHostSmo, portname); //info.portName()); //TODO: next format: FriendlyName + " (" + info.portName() + ")"
            Smo::SetIcon(portNode, "usb");
            portNode->setToolTip(0, info.systemLocation()); //portNode->setToolTip(0, info.description());
            portNode->setPlugin(ComPortsPlugin);
            localHostSmo->addChild(portNode);
            portNode->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);

            new DeviceInfo(new TDIPortSerial(this, info), portNode);
        }
    }

    // add hid devices...
    auto hidapi = TDIPortHid::hidApi();
    hids = hidapi->enumerate(/*ushort vendorId*/0, /*ushort productId*/0);
    foreach (const QHidDeviceInfo &info, hids) {
        QString hidname = info.productString; // + "(" + info.path + ")";
        int idx = TreeLib::FindChildIndexOnText<Smo>(localHostSmo, hidname, istart);
        if (idx >= 0) {
            if (idx < foundlist.count()) foundlist[idx] = 1;
            if (idx == istart) istart++;
        }
        else
        {
            Smo *hidNode = new Smo(localHostSmo, hidname); //info.portName()); //TODO: next format: FriendlyName + " (" + info.portName() + ")"
            Smo::SetIcon(hidNode, "usb");
            hidNode->setToolTip(0, info.manufacturerString);
            hidNode->setPlugin(HidDevicePlugin);
            localHostSmo->addChild(hidNode);
            hidNode->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);

            new DeviceInfo(new TDIPortHid(this, info), hidNode);
        }
    }

/// remove absent port items!!! or they may be special marked(i.e. absent icon...) ???
    int i = foundlist.count();
    while (--i >= 0) { //start from last index...
        if (foundlist.value(i, -1) == 0) {
            Smo* smo = localHostSmo->takeChild(i);
            if (smo) {
                DeviceInfo* inf = smo->deviceInfo();
                if (inf) {
                    inf->deleteLater();
                }
            }
        }
    }
}

void LocalHostWidget::addHost()
{
    QMessageBox::information(this, "info", "add host(under development...)");
}
