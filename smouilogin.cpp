

#include <QApplication>

#include "smouilogin.h"
#include "smouiselectitem.h"
#include "zvvdebug.h"

SmoUILogin::SmoUILogin(QWidget *parent)
: SmoUI(parent, SmoUITypes::uiLogin)
{
    comboxSmoLogin = new QComboBox(this);
    comboxSmoLogin->setEditable(false);
    comboxSmoLogin->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboxSmoLogin->setVisible(false);
    connect(comboxSmoLogin, static_cast<void(QComboBox::*)(int)>(&QComboBox::highlighted),
            [this](int index)->void {SmoUISelectItem::comboxListUpdate_slot(this->UISmo, this->comboxSmoLogin, index);});
    hBoxLayout->addWidget(comboxSmoLogin);

    lineSmoData = new QLineEdit(this);
    lineSmoData->setEchoMode(QLineEdit::EchoMode::Password);
    lineSmoData->setVisible(false);
    hBoxLayout->addWidget(lineSmoData);

    buttSmoData = new QPushButton(this);
    buttSmoData->setVisible(false);
    connect(buttSmoData, &QPushButton::clicked,
            [this]()->void {SmoUILogin::buttSmoDataWrite_slot(this);});
    hBoxLayout->addWidget(buttSmoData);

    debug_tree_info(tr("init SmoUILogin(): %1").arg((intptr_t)this,0,16));
}

SmoUILogin::~SmoUILogin() {
//    disconnect(lineSmoData,  0,0,0);
//    disconnect(buttSmoData,  0,0,0);
//    disconnect(comboxSmoLogin,0,0,0);
    buttSmoData->deleteLater();
    lineSmoData->deleteLater();
    comboxSmoLogin->deleteLater();
    lineSmoName->deleteLater();
}

void SmoUILogin::UIUpdate(Smo* smo, bool isViewUpdateOnly)
{
    UISmo = smo;
    SmoUI::SmoInviter_update(lineSmoName, lineSmoName_secondAction, smo);
    SmoAccess smoAccess = Smo::AccessState(smo);
    bool isDataToBeVisible = Smo::IsAccessible(smoAccess) && Smo::IsDataEnabled(smo);
    if (isDataToBeVisible)
    {
        bool isok = isViewUpdateOnly || Smo::Call(smo);

        SmoUISelectItem::Combox_update(comboxSmoLogin, smo);
        SmoUI::RefreshAction_update(this, lineSmoData, !isok);
        SmoUI::LineText_update(lineSmoData, smo);
        if (isok) DeviceInfo::LoginedUser_update(smo, comboxSmoLogin->currentText()); //TODO: save login in deviceInfo and refresh smo->deviceInfo()->AccessState();!!!
    }
    else {
        smoAccess = SmoAccess::NoDataAccess;
    }
    comboxSmoLogin->setEnabled(smoAccess == SmoAccess::FullAccess);
    comboxSmoLogin->setVisible(isDataToBeVisible);
    SmoUI::TextAndWriteButton_update(this, lineSmoData, buttSmoData, (SmoUIState)smoAccess);
}

void SmoUILogin::UIDisable()
{
    lineSmoData->setEnabled(false);
    comboxSmoLogin->setEnabled(false);
    buttSmoData->setEnabled(false);

    lineSmoName->setVisible(false);
    comboxSmoLogin->setVisible(false);
    lineSmoData->setVisible(false);
    buttSmoData->setVisible(false);
}

void SmoUILogin::buttSmoDataWrite_slot(SmoUILogin* ui)
{
    Smo* smo = ui->UISmo;
    QComboBox* combox = ui->comboxSmoLogin;
    QPushButton* button = ui->buttSmoData;
    QLineEdit* line = ui->lineSmoData;
    debug_tree_info(tr("SmoUILogin::buttSmoData_slot: %1; Login: %2").arg(smo->ShortName()).arg(combox->currentText()));
    smoKey_t skey = {combox->currentData().value<smokey>()};
    if (skey.KeyTagValue) skey.Key = 0; //if tag!=0 then unselected value, so reset it by zeroing!!!
    bool isTargetMemOk = Smo::SmartBackTransform(smo, line->text()) && Smo::calcTargetMemory(smo);
    SmoUIState state;
    if (isTargetMemOk) {
//        if (skey.Key) {
            smo->targetMemory()->insert(0, (const char*)&skey, sizeof (smokey));
            smo->deviceInfo()->setDesc(smo);
//        }
//        else {
//            smo->targetMemory()->fill('\0', 1);
//        }
        if (Smo::Call(smo, smo->targetMemory())) {
            state = SmoUIState::UI_write_ok;
            line->setText(Smo::TargetMemoryToString(smo)); //refresh line with the call result
        } else {
            state = SmoUIState::UI_write_io_error;
        }
    }
    else {
        smo->deviceInfo()->setDesc(0);
        state = SmoUIState::UI_write_bad_value;
//        connect(line, &QLineEdit::textChanged,
//                [smo, button, line]()->void {SmoUI::lineDataChangedAction_slot(smo, button, line);}); // Log::addWarning(tr("connected!!"));
    }
    SmoUI::TextAndWriteButton_update(ui, line, button, state);
    DeviceInfo::LoginedUser_update(smo, ui->comboxSmoLogin->currentText());
}

