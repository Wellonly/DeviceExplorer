#include "app_config.h" //#include "zvvlib.h"

#include "zvvdebug.h"
#include "dewidget.h"
#include "fdevice.h"

fDevice::fDevice(QMainWindow *parent, Smo *smo, const QString &idname)
: DEWidget(parent, smo, idname)
{

    Grid = zvvlib::GridLayout(this); //zvv:added a widget upon a QDockWidget

    QLabel *label = new QLabel("MAC:", this);
    Grid->addWidget(label, 0, 0);

    MACLine = new QLineEdit(this);
    Grid->addWidget(MACLine, 0, 1);

    QPushButton *button = new QPushButton("MAC", this);
    Grid->addWidget(button, 0, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonMAC_slot()));

    label = new QLabel("Device plugin:", this);
    Grid->addWidget(label, 1, 0);

    comboPlugin = new QComboBox(this);
    Grid->addWidget(comboPlugin, 1, 1);

    button = new QPushButton("Plugin", this);
    Grid->addWidget(button, 1, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonPlugin_slot()));

    button = new QPushButton("Play", this);
    Grid->addWidget(button, 1, 3);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonPlay_slot()));

    DERaise_slot(smo, true); // Form_refresh(smo, true);

}

void fDevice::buttonMAC_slot()
{
    debug_tree_info("fDevice::buttonMAC_slot() ok!!!");
}

void fDevice::buttonPlugin_slot()
{
    debug_tree_info("fDevice::buttonPlugin_slot() ok!!!");
}

void fDevice::buttonPlay_slot()
{
    debug_tree_info("fDevice::buttonPlay_slot() ok!!!");
}

void fDevice::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    (void)smo;
    (void)isFormToBeRefreshed;

//    isFormToBeRefreshed = isFormToBeRefreshed || smoRef != smo;
//    smoRef = smo;
//    bool nodesToBeRefreshed = isFormToBeRefreshed && smo->isExpanded();
//    debug_tree_info(QString("fDevice::Form_refresh() for: %1; formToBeRefreshed: %2; nodesToBeRefreshed: %3").arg(smo->ShortName()).arg(isFormToBeRefreshed).arg(nodesToBeRefreshed));
//    if (isFormToBeRefreshed) DeviceSettings_update(smo);
//    if (nodesToBeRefreshed) DeviceNodes_refresh(smo);
}

void fDevice::DeviceSettings_update(Smo *smo)
{
    debug_tree_info("fDevice::DeviceSettings_update(): " + smo->ShortName());
}

void fDevice::DeviceNodes_refresh(Smo *smo)
{
    debug_tree_info("fDevice::DeviceNodes_refresh(): " + smo->ShortName());
    Smo::RefreshChildren(smo);
}

