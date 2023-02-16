#ifndef FCOMPORT_H
#define FCOMPORT_H

#include <QFile>
#include <QComboBox>
#include <QDockWidget>
#include <QJsonObject>
#include <QMainWindow>
#include <QPushButton>
#include <QSerialPort>
#include <QMap>
#include <QMetaType>
#include <QMap>

#include "smo.h"
#include "jconfig.h"
#include "tdiportserial.h"
#include "dewidget.h"

#ifndef SERIAL_SETTINGS_FILENAME
#define SERIAL_SETTINGS_FILENAME "COMPorts.ini"
#endif

#define DevicePropertyName "DevN" /*"DeviceName"*/

class fCOMPort : public DEWidget
{
    Q_OBJECT
public:
    fCOMPort(QMainWindow *parent, Smo *smo, const QString& idname = QString("DE.fCOMPort"));

private slots:
    void okSlot();
    void saveSlot();
    void SettingsChangedSlot(Smo *smo, const char* propertyName, const QString &newValue);

public slots:

private:
    QComboBox *comboDevice;
    QComboBox *comboBoudRate;
    QComboBox *comboDataBits;
    QComboBox *comboStops;
    QComboBox *comboFlowControl;
    QComboBox *comboParity;
    QComboBox *comboTimeOut;

    QPushButton *buttonTry;
    QPushButton *buttonSave;

//    Smo *m_smo;

    void Form_refresh(Smo *smo, bool isFormToBeRefreshed) override;

    void TryConnect(Smo *smo);
    void SerialPortSettings_update(Smo* smo);
    void GateSettings_update(Smo* smo, const QString &GateName);
    bool PortSettings_save(const QString& deviceShortName, bool isForceUpdate);
    bool PortSetupAndOpen(TDIPortSerial *port);
    void Combos_update(Smo* smo);
    bool ReadFirstGate(Smo* smo);

    const char * const ConfigFName = SERIAL_SETTINGS_FILENAME;
    JConfig *config;

    const struct PortSettings {
            const char *name;
            QComboBox **combo;
            const EMap<> comboList;
            const char *propertyName;
    } controlsArray [7] = {
        { "Device:",       &comboDevice,      EMap<>(),          DevicePropertyName},
        { "Baud rate:",    &comboBoudRate,    TDIPortSerial::BaudRateMap,   "BaudRate"},
        { "Data bits:",    &comboDataBits,    TDIPortSerial::DataBitsMap,   "DataBits"},
        { "Stop bits:",    &comboStops,       TDIPortSerial::StopBitsMap,   "StopBits"},
        { "Flow control:", &comboFlowControl, TDIPortSerial::FlowControlMap,"FlowControl"},
        { "Parity:",       &comboParity,      TDIPortSerial::ParityMap,     "Parity"},
        { "Timeout,mS:",   &comboTimeOut,     TDIPortSerial::TimeoutMap,    "Timeout"}
    };

};

#endif // FCOMPORT_H
