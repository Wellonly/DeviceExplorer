#ifndef FHIDPORT_H
#define FHIDPORT_H

#include <QDockWidget>
#include <QMainWindow>
#include <QPushButton>

#include "smo.h"
#include "tdiporthid.h"
#include "dewidget.h"


typedef QVariant (*devinfo)(const QHidDeviceInfo* dinf);

class fHIDPort : public DEWidget
{
    Q_OBJECT
public:
    fHIDPort(QMainWindow *parent, Smo *smo, const QString& idname = QString("DE.fHIDPort"));

private slots:
    void okSlot();

public slots:

private:
    QPushButton *buttonTry;
    Smo *m_smo;
    QGridLayout *gridLayout;

    void Form_refresh(Smo *smo, bool isFormToBeRefreshed) override;

    void TryConnect(Smo *smo);
    void PortSettings_update(Smo* smo);
    bool PortSetupAndOpen(TDIPortHid *port);
    void ReadFirstGate(Smo* smo);

    const struct PortSettings {
        const char *name;
        devinfo data;
    } controlsArray [8] = {
        { "Device path:",         [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->path);}},
        { "Vendor ID:",           [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->vendorId);}},
        { "Product ID:",          [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->productId);}},
        { "Serial number:",       [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->serialNumber);}},
        { "Release number:",      [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->releaseNumber);}},
        { "Manufacturer string:", [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->manufacturerString);}},
        { "Product string:",      [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->productString);}},
        { "USB interface:",       [](const QHidDeviceInfo* dinf){ return QVariant::fromValue(dinf->interfaceNumber);}},
    };
};

#endif // FHIDPORT_H
