
#include "fcomport.h"
#include "zvvlib.h"
#include "zvvdebug.h"
#include "LogTree.h"
#include "dewidget.h"

#include <QGridLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QAction>
#include <QTreeWidgetItem>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>

#include <QMetaObject>

#include "app_config.h"

#ifndef DelayBeforeFirstFrame_mS
#define DelayBeforeFirstFrame_mS 200
#endif

static void LogWarningFail(const char* func_name) {
    Log::addWarning(QString(func_name).append(" fail"));
}

fCOMPort::fCOMPort(QMainWindow *parent, Smo *smo, const QString& idname)
: DEWidget(parent, smo, idname)
{
    using namespace std;
    setMinimumSize(315,150);

    QGridLayout *gridLayout = zvvlib::GridLayout(this);

    const int arrLen = sizeof(controlsArray) / sizeof(PortSettings);
    int i;
    for (i = 0; i < arrLen; ++i) {
        QLabel *label = new QLabel((controlsArray[i].name), this);
        gridLayout->addWidget(label, i, 0);
        QComboBox *cbox = *controlsArray[i].combo = new QComboBox(this);
        cbox->setEditable(cbox == comboBoudRate);
        foreach (auto item, controlsArray[i].comboList.keys()) {
            cbox->addItem(item);
        }
        gridLayout->addWidget(cbox, i, 1);
        connect(cbox, &QComboBox::currentTextChanged, [this, i](QString newValue)->void {this->SettingsChangedSlot(this->smoRef(), controlsArray[i].propertyName, newValue); });
    }

    QWidget* cmds = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(cmds);
    buttonLayout->addStretch();
    gridLayout->addWidget(cmds, i, 0, 1, 2);

    buttonTry = new QPushButton(tr("Connect"), this);
    connect(buttonTry, SIGNAL(clicked()), this, SLOT(okSlot()));
    buttonLayout->addWidget(buttonTry);
    buttonSave = new QPushButton(tr("Save"), this);
    connect(buttonSave , SIGNAL(clicked()), this, SLOT(saveSlot()));
    buttonLayout->addWidget(buttonSave );

    config = new JConfig(this, ConfigFName); //NOTE: if config file is not found then it will be created!!!

    SerialPortSettings_update(smo);
    if (smo->isExpanded()) TryConnect(smo);
}

void fCOMPort::okSlot()
{
    if (smoRef()->isExpanded()) {
        TryConnect(smoRef());
    }
    else {
        smoRef()->setExpanded(true); //TryConnect() will be called from Form_refresh() on expanded event!!!
    }
}

void fCOMPort::saveSlot()
{
    buttonSave->setEnabled(!PortSettings_save(comboDevice->currentText(), true));
}

void fCOMPort::SettingsChangedSlot(Smo *smo, const char *propertyName, const QString& newValue)
{
    //Log::Add(tr("fCOMPort::ControlsChangedSlot(Smo %1, %2, %3) %4").arg(smo->Text()).arg(propertyName).arg(newValue).arg(QDateTime::currentDateTime().toString()));
    TDIPortSerial* port = qobject_cast<TDIPortSerial*>(smo->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortSerial*>();
    if (!port) return LogWarningFail(__func__);
    if (QString(propertyName).indexOf(DevicePropertyName) >= 0)
    { //refresh gate settings...
        GateSettings_update(smo, newValue);
        this->buttonSave->setEnabled(false);
    }
    else {
        QVariant oldValue = port->property(propertyName);
        if (oldValue.type() == QVariant::Int) {
            port->setProperty(propertyName, newValue.toInt());
        }
        else {
            port->setProperty(propertyName, newValue);
        }
        this->buttonSave->setEnabled(true);
    }
}

void fCOMPort::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    if (smoRef() != smo) SerialPortSettings_update(smo);
    if (isFormToBeRefreshed) TryConnect(smo);
}

void fCOMPort::Combos_update(Smo* smo)
{
    TDIPortSerial* port = qobject_cast<TDIPortSerial*>(smo->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortSerial*>();
    if (!port) return LogWarningFail(__func__);
    const int arrLen = sizeof(controlsArray) / sizeof(PortSettings);
    for (int i = 1; i < arrLen; ++i) { //skip gate name combo @ zero index!!!
        QComboBox *cbox = *controlsArray[i].combo;
        QVariant propValue = port->property(controlsArray[i].propertyName);
        if (propValue.type() == QVariant::Int) {
            cbox->setCurrentText(QString::number(propValue.toInt()));
        }
        else {
            cbox->setCurrentText(propValue.toString());
        }
    }
    buttonSave->setEnabled(false);
}

void fCOMPort::SerialPortSettings_update(Smo* smo)
{
    setSmoRef(smo); //    smoRef() = smo;
    QString friendlyName = smo->toolTip(/*0*/);
    comboDevice->setToolTip(friendlyName);

    QString GateName;

    QVariantMap configmap;
    config->readToDynamicProperties(configmap);
    QVariantMap gates(configmap["Gates"].value<QVariantMap>());

    if (gates.contains(friendlyName)) {
        GateName = gates[friendlyName].value<QString>();
        if (configmap.contains(GateName)) {
            QVariantMap deviceSetsMap = configmap[GateName].value<QVariantMap>();
            if (config->mapToObjectDynamicProperties(&deviceSetsMap, smo->deviceInfo()->HWPort())) { //smo->Tag.value<QObject*>()
                qDebug() << "Config successfully read back for: " << friendlyName << "; Gate: " << GateName;
            }
        }
    }

    foreach (auto devname, configmap.keys()) {
        if (devname != "Gates" && comboDevice->findText(devname) == -1) {
            comboDevice->addItem(devname);
        }
    }
    comboDevice->setCurrentText(GateName);

    Combos_update(smo);

}

void fCOMPort::GateSettings_update(Smo* smo, const QString& GateName)
{
    QVariantMap configmap;
    config->readToDynamicProperties(configmap);
    QVariantMap deviceSetsMap = configmap[GateName].value<QVariantMap>();
    config->mapToObjectDynamicProperties(&deviceSetsMap, smo->deviceInfo()->HWPort()); //smo->Tag.value<QObject*>()
    Combos_update(smo);
}

bool fCOMPort::ReadFirstGate(Smo *smo)
{
//    TDIPortSerial *smo_port = smo->deviceInfo()->HWPort(); //smo->Tag.value<TDIPortSerial*>();
    Smo *portFirstDeviceSmo = iif(smo->childCount() > 0, smo->child(0), 0);
//    TDIPortSerial *dev_port = iif(portFirstDeviceSmo && !portFirstDeviceSmo->deviceInfo().isNull(), qobject_cast<TDIPortSerial*>(portFirstDeviceSmo->deviceInfo()->HWPort()), 0);
//    if (!dev_port || smo_port->name() != dev_port->name()) {
    if (!portFirstDeviceSmo || !portFirstDeviceSmo->deviceInfo()) {
        if (portFirstDeviceSmo) qDeleteAll(smo->takeChildren());
        portFirstDeviceSmo = new Smo(smo,"...");
        smo->deviceInfo()->setDeviceSmo(portFirstDeviceSmo);
        zvvlib::Delay_mS(DelayBeforeFirstFrame_mS); //TODO: make this value as a setting! wait for hardware (e.g. power stable for LionGate) before first frame!!!
    }
    if (Smo::Read(portFirstDeviceSmo)) {
        Smo::SetIcon(portFirstDeviceSmo, "device");
        portFirstDeviceSmo->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
        portFirstDeviceSmo->deviceInfo()->setDeviceName(portFirstDeviceSmo->ShortName());
        return true;
    }
    portFirstDeviceSmo->deviceInfo()->setDeviceName(QString());
    return false;
}

bool fCOMPort::PortSettings_save(const QString& deviceShortName, bool isForceUpdate)
{
    if (deviceShortName.isEmpty()) {
        Log::addWarning(tr("Config has not saved: device name is empty!"));
        return false;
    }
    if (isForceUpdate || comboDevice->currentText() != deviceShortName) {
        QVariantMap gates;
        config->readToDynamicProperties(gates, "Gates");
        QString gate = smoRef()->toolTip(/*0*/);
        gates[gate] = deviceShortName;
        bool IsOk = config->save(gates, "Gates");

        TDIPortSerial* port = qobject_cast<TDIPortSerial*>(smoRef()->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortSerial*>();
        QVariant qv = QVariant::fromValue<TDIPortSerial*>(port);
        IsOk = IsOk && config->save(qv, deviceShortName);
        if (!IsOk)
        {
            auto last_ex = config->lastException();
            Log::addError(tr("Config: %1").arg(iif(last_ex, last_ex->eMessage, "undefined error")));
        }
        return IsOk;
    }
    return true;
}

bool fCOMPort::PortSetupAndOpen(TDIPortSerial *serial)
{
    bool ok = serial->Settings_engage();
    ok = ok && serial->open();
    if (ok) {
        serial->rootDeviceInfo()->portScanInit();
    }
    return ok;
}

void fCOMPort::TryConnect(Smo *smo)
{
    TDIPortSerial *port = qobject_cast<TDIPortSerial*>(smo->deviceInfo()->HWPort()); //smo->Tag.value<TDIPortSerial*>();
    if (!port) return LogWarningFail(__func__);
    if (port->isOpen()) port->close(); //zvv: close the port in any way!!!
    if (smo->isExpanded()) { // expanded: open the port else close the port...
        debug_func("serial port");
        if (PortSetupAndOpen(port)) {
            if (ReadFirstGate(smo)) {
                QString gateName = smo->child(0)->text(0);
                buttonSave->setEnabled(!PortSettings_save(gateName, false));

                if (comboDevice->findText(gateName) == -1) comboDevice->addItem(gateName);
                comboDevice->setCurrentText(gateName);
            }
        }
        else {
            QString errstr = port->errorString();
            Smo::MessageInfo(smo, tr("Open error: %1").arg(errstr));
            Log::addError(tr("Serial port error1: %1").arg(errstr));
        }
    }
    else
    { // TODO: the port may be closed if all child nodes collapsed...
//        Smo::MessageInfo(smo, "port closed");
    }
}

