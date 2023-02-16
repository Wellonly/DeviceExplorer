#include "smouiMessage.h"

SmoUIMessage::SmoUIMessage(QWidget *parent) : SmoUI(parent, SmoUITypes::uiMessage)
{
    lblSmoName = new QLabel(this);
    hBoxLayout->addWidget(lblSmoName);

    lineSmoData = new QLineEdit(this);
    lineSmoData->setVisible(false);
    lineSmoData->setReadOnly(true);
    hBoxLayout->addWidget(lineSmoData);

    buttSmoData = new QPushButton(zvvlib::getIcon("refresh"),"", this);
    buttSmoData->setToolTip(tr("refresh status"));
    buttSmoData->setVisible(false);
    hBoxLayout->addWidget(buttSmoData);

}

SmoUIMessage::~SmoUIMessage()
{
    buttSmoData->deleteLater();
    lineSmoData->deleteLater();
    lblSmoName->deleteLater();
}

void SmoUIMessage::UIUpdate(Smo* smo, bool isViewUpdateOnly)
{
    (void)isViewUpdateOnly;
    lblSmoName->setText(smo->inviteString());
    lineSmoData->setVisible(true);
    lineSmoData->setText(Smo::StatusDescript(smo));
    buttSmoData->setVisible(true);
}

void SmoUIMessage::UIDisable()
{
    lblSmoName->setText(QString());
    lineSmoData->setVisible(false);
    buttSmoData->setVisible(false);
}

