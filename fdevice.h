#ifndef FDEVICE_H
#define FDEVICE_H

#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>

#include "smo.h"
#include "dewidget.h"

class fDevice : public DEWidget
{
    Q_OBJECT
public:
   explicit fDevice(QMainWindow *parent, Smo *smo, const QString& idname = QString("DE.fDevice"));

signals:

public slots:

private slots:
    void buttonMAC_slot();
    void buttonPlugin_slot();
    void buttonPlay_slot();

private:
    Smo *smoRef;
    QGridLayout *Grid;
    QLineEdit *MACLine;
    QComboBox *comboPlugin;

    void Form_refresh(Smo *smo, bool isFormToBeRefreshed) override;
    void DeviceSettings_update(Smo* smo);
    void DeviceNodes_refresh(Smo* smo);
};

#endif // FDEVICE_H
