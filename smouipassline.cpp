#include "smouipassline.h"
#include "LogTree.h"
#include "qdebug.h"

#include <QApplication>

SmoUIPassLine::SmoUIPassLine(QWidget *parent)
: SmoUI(parent, SmoUITypes::uiPassLine)
{
    lineSmoData = new QLineEdit(this);
    lineSmoData->setEchoMode(QLineEdit::EchoMode::Password);
    lineSmoData->setVisible(false);
    hBoxLayout->addWidget(lineSmoData);

    buttSmoData = new QPushButton(this);
    buttSmoData->setVisible(false);
    connect(buttSmoData, &QPushButton::clicked,
            [this]()->void {buttSmoDataWrite_slot(this);});
    hBoxLayout->addWidget(buttSmoData);

//    debug_tree_info(tr("buttSmoData: %1").arg("created!!!"));
}

SmoUIPassLine::~SmoUIPassLine() {
//    disconnect(lineSmoData, 0, 0, 0);
//    disconnect(buttSmoData, 0, 0, 0);
    buttSmoData->deleteLater();
    lineSmoData->deleteLater();
    lineSmoName->deleteLater();
}

void SmoUIPassLine::UIUpdate(Smo* smo, bool isViewUpdateOnly)
{
    UISmo = smo;
    SmoUI::SmoInviter_update(lineSmoName, lineSmoName_secondAction, smo);
//    debug_tree_info(tr("smo : %1 %2").arg(Smo::DisplayInvite(smo)).arg(qobject_cast<QObject*>(smo)));
    SmoAccess smoAccess = Smo::AccessState(smo);
    bool isAccessible = (smoAccess == SmoAccess::ReadOnly || smoAccess == SmoAccess::FullAccess);
    bool isDataEnabled = Smo::DataAddress(smo) != 0;
    if (isAccessible && isDataEnabled)
    {
        bool isok = isViewUpdateOnly || Smo::Call(smo);
        if (isok) DeviceInfo::LoginedUser_update(smo); //TODO: save login in deviceInfo and refresh smo->deviceInfo()->AccessState();!!!
        SmoUI::RefreshAction_update(this, lineSmoData, !isok);
        SmoUI::LineText_update(lineSmoData, smo); //text_update(lineSmoData, smo);
    }
    else {
        smoAccess = SmoAccess::NoDataAccess;
    }
    SmoUI::TextAndWriteButton_update(this, lineSmoData, buttSmoData, (SmoUIState)smoAccess);
//next: debug only...
//    SmoUI::DebugOnly(lineSmoData, smo, "update SmoUIPassLine()");
}

void SmoUIPassLine::UIDisable()
{
    lineSmoData->setEnabled(false);
    buttSmoData->setEnabled(false);

    lineSmoName->setVisible(false);
    lineSmoData->setVisible(false);
    buttSmoData->setVisible(false);
}

void SmoUIPassLine::buttSmoDataWrite_slot(SmoUIPassLine* ui)
{
    Smo* smo = ui->UISmo;
    QPushButton* button = ui->buttSmoData;
    QLineEdit* line = ui->lineSmoData;
//    debug_tree_info(tr("buttSmoData_slot: %1").arg(smo->ShortName()));
    SmoUIState state;
    bool isTargetMemOk = Smo::SmartBackTransform(smo, line->text()) && Smo::calcTargetMemory(smo);
    if (isTargetMemOk) {
        smo->deviceInfo()->setDesc(smo);
        if (Smo::Call(smo, smo->targetMemory())) {
            line->setText(Smo::TargetMemoryToString(smo));
            state = SmoUIState::UI_write_ok;
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
    DeviceInfo::LoginedUser_update(smo);
}

