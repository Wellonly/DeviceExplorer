

#include "smouitextline.h"
#include "zvvdebug.h"

#include <QApplication>

SmoUITextLine::SmoUITextLine(QWidget *parent, SmoUITypes uitype)
:SmoUI(parent, uitype) //SmoUITypes::uiTextLine
{
    lineSmoData = new QLineEdit(this);
    lineSmoData->setVisible(false);
    hBoxLayout->addWidget(lineSmoData);

    buttSmoData = new QPushButton(this);
    buttSmoData->setVisible(false);
    connect(buttSmoData, &QPushButton::clicked,
            [this]()->void {buttSmoDataWrite_slot(this);});
    hBoxLayout->addWidget(buttSmoData);

    debug_tree_info(tr("init SmoUITextLine() at: 0x%1").arg((intptr_t)this,0,16));
}

SmoUITextLine::~SmoUITextLine() {
//    disconnect(lineSmoData, 0, 0, 0);
    buttSmoData->deleteLater();
    lineSmoData->deleteLater();
    lineSmoName->deleteLater();
}

void SmoUITextLine::UIDisable()
{
    lineSmoName->setVisible(false);
    lineSmoData->setVisible(false);
    lineSmoData->setEnabled(false);
    buttSmoData->setVisible(false);
}

void SmoUITextLine::UIUpdate(Smo* smo, bool isViewUpdateOnly)
{
    UISmo = smo;
//    SmoUI::SmoInviter_update(lineSmoName, lineSmoName_secondAction, smo);
    SmoAccess smoAccess = Smo::AccessState(smo);
    bool isAccessible = (smoAccess == SmoAccess::ReadOnly || smoAccess == SmoAccess::FullAccess);
    bool isDataAddressEmpty = Smo::DataAddress(smo) == 0;
    bool isDataConstant = smo->dataType() == SmoDataType::Enumerator;
    bool isDataEnabled = isDataConstant || smo->headType() == SMOHeadTypes::SmoHeadType_Data || !isDataAddressEmpty;
    if (isAccessible && isDataEnabled)
    {
        bool isok = isDataConstant || isViewUpdateOnly || Smo::Call(smo);
        SmoUI::RefreshAction_update(this, lineSmoData, !isok);
        SmoUI::LineText_update(lineSmoData, smo); // text_update(lineSmoData, smo);
    }
    else {
        smoAccess = SmoAccess::NoDataAccess;
    }
    SmoUI::SmoInviter_update(lineSmoName, lineSmoName_secondAction, smo);
    SmoUI::TextAndWriteButton_update(this, lineSmoData, buttSmoData, (SmoUIState)smoAccess);
//debug only...
//SmoUI::DebugOnly(lineSmoData, smo, "update SmoUITextLine()");
}

void SmoUITextLine::buttSmoDataWrite_slot(SmoUITextLine* ui)
{
    Smo* smo = ui->UISmo;
    QPushButton* button = ui->buttSmoData;
    QLineEdit* line = ui->lineSmoData;
//  debug_tree_info(tr("buttSmoData_slot: %1").arg(smo->ShortName()));
    bool isTargetMemOk = Smo::SmartBackTransform(smo, line->text()) && Smo::calcTargetMemory(smo);
    SmoUIState state;
    if (isTargetMemOk) {
        if (Smo::Call(smo, smo->targetMemory()))
        {
            state = SmoUIState::UI_write_ok;
        } else {
            state = SmoUIState::UI_write_io_error;
        }
    }
    else {
        state = SmoUIState::UI_write_bad_value;
//TODO:it        connect(line, &QLineEdit::textChanged,
//                [smo, button, line]()->void {SmoUI::lineDataChangedAction_slot(smo, button, line);});
    }
    SmoUI::TextAndWriteButton_update(ui, line, button, state);
}

