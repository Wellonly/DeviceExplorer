
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QAction>
#include <QTreeWidgetItem>
#include <QDateTime>
#include <QLineEdit>
#include <QVariant>

#include <QMap>
#include <QMetaType>
#include <QMetaObject>

#include "zvvlib.h"
#include "LogTree.h"
#include "dewidget.h"
#include "MainWindow.h"
#include "deconfig.h"

#include "fhidport.h"
#include "app_config.h"
#include "tdiporthid.h"

static void LogWarningFail(const char* func_name) {
    Log::addWarning(QString(func_name).append(" fail"));
}

fHIDPort::fHIDPort(QMainWindow *parent, Smo *smo, const QString &idname)
: DEWidget(parent, smo, idname)
{
    using namespace std;
    setMinimumSize(315,150);

    gridLayout = zvvlib::GridLayout(this);

    PortSettings_update(smo);

    QWidget* cmds = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(cmds);
    buttonLayout->addStretch();
    const int cmdrow = sizeof(controlsArray) / sizeof(PortSettings);
    gridLayout->addWidget(cmds, cmdrow, 0, 1, 2);

    buttonTry = new QPushButton(tr("Connect"), this);
    connect(buttonTry, SIGNAL(clicked()), this, SLOT(okSlot()));
    buttonLayout->addWidget(buttonTry);

    if (smo->isExpanded()) TryConnect(smo);
}

void fHIDPort::okSlot()
{
    if (!m_smo->isExpanded()) {
        qDeleteAll(m_smo->takeChildren());
        m_smo->setExpanded(true);
    }
    TryConnect(m_smo);
}

void fHIDPort::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    if (m_smo != smo) PortSettings_update(smo);
    if (isFormToBeRefreshed) TryConnect(smo);
}

void fHIDPort::PortSettings_update(Smo* smo)
{
    m_smo = smo;
    TDIPortHid* port = qobject_cast<TDIPortHid*>(smo->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortHid*>();
    if (!port) return LogWarningFail(__func__);
    const QHidDeviceInfo* dinfo = port->deviceInfo();
    const int arrLen = sizeof(controlsArray) / sizeof(PortSettings);
    for (int i = 0; i < arrLen; ++i) {
        auto item = gridLayout->itemAtPosition(i, 1);
        if (item) {
            QLineEdit* line = qobject_cast<QLineEdit*>(item->widget());
            line->setText(controlsArray[i].data(dinfo).toString());
        }
        else
        { //init...
            QLabel *label = new QLabel((controlsArray[i].name), this);
            gridLayout->addWidget(label, i, 0);
            QLineEdit* line = new QLineEdit(controlsArray[i].data(dinfo).toString(), this);
            line->setReadOnly(true);
            gridLayout->addWidget(line, i, 1);
        }
    }
}

void fHIDPort::ReadFirstGate(Smo *smo)
{
//    TDIPortHid *smo_port = smo->deviceInfo()->HWPort(); //smo->Tag.value<TDIPortHid*>();
    Smo *portFirstDeviceSmo = iif(smo->childCount() > 0, smo->child(0), 0);
//    TDIPortHid *dev_port = iif(portFirstDeviceSmo && !portFirstDeviceSmo->deviceInfo().isNull(), qobject_cast<TDIPortHid*>(portFirstDeviceSmo->deviceInfo()->HWPort()), 0);
//    if (!dev_port || smo_port->getDeviceId() != dev_port->getDeviceId()) {
    if (!portFirstDeviceSmo || !portFirstDeviceSmo->deviceInfo()) {
        if (portFirstDeviceSmo) qDeleteAll(smo->takeChildren());
        portFirstDeviceSmo = new Smo(smo,"...");
        smo->deviceInfo()->setDeviceSmo(portFirstDeviceSmo);
    }
    if (Smo::Read(portFirstDeviceSmo)) {
        Smo::SetIcon(portFirstDeviceSmo, "device");
        portFirstDeviceSmo->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
        portFirstDeviceSmo->deviceInfo()->setDeviceName(portFirstDeviceSmo->ShortName());
    }
    else {
        portFirstDeviceSmo->deviceInfo()->setDeviceName(QString());
    }
}

bool fHIDPort::PortSetupAndOpen(TDIPortHid *port)
{
    if (port->open()) {
        MainWindow* mainw = (MainWindow*)parentWidget();
        DEConfig* cfg = mainw->getAppConf();
        port->setTimeout(cfg->getPortTimeout());
        port->rootDeviceInfo()->portScanInit();
        return true;
    }
    return false;
}

void fHIDPort::TryConnect(Smo *smo)
{
    TDIPortHid *port = qobject_cast<TDIPortHid*>(smo->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortHid*>();
    if (!port) return LogWarningFail(__func__);
//    if (port->isOpen()) port->close(); //zvv: close the port in any way!!!
    if (smo->isExpanded()) { // expanded: open the port else close the port...
        if (PortSetupAndOpen(port)) {
            ReadFirstGate(smo);
        }
        else {
            QString errstr = port->errorString();
            Smo::MessageInfo(smo, tr("Open error: %1").arg(errstr));
            Log::addError(tr("Device port error: %1").arg(errstr));
        }
    }
    else
    { // TODO: the port may be closed if all child nodes collapsed...
//        Smo::MessageInfo(smo, "port closed");
    }
//    if (smo->childCount() == 0) {
//        Log::addWarning(tr("no children"));
//        Smo::MessageInfo(smo, "undefined error");
//    }
}

