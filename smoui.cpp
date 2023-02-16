
#include "zvvdebug.h"
#include "smoui.h"
#include "smouiselectitem.h"
#include "LogTree.h"

SmoUI::SmoUI(QWidget *parent, SmoUITypes uitype)
:QWidget(parent)
,uiType(uitype)
{
    hBoxLayout = new QHBoxLayout(this);
    setLayout(hBoxLayout);

    lineSmoName = new QLineEdit(this);
    lineSmoName->setVisible(false);
    lineSmoName->setReadOnly(true);
    lineSmoName->setStyleSheet(QStringLiteral("background: palette(window);"));
    lineSmoName->setFrame(false);

    lineSmoName_firstAction = new QAction(lineSmoName);
    lineSmoName_firstAction->setIcon(zvvlib::getIcon(QStringLiteral("link")));
    lineSmoName->addAction(lineSmoName_firstAction, QLineEdit::ActionPosition::LeadingPosition);
    connect(lineSmoName_firstAction, &QAction::triggered,
            [this]()->void {SmoUI::lineSmoName_trackAction_slot(this->UISmo);});

    lineSmoName_secondAction = new QAction(lineSmoName);
    lineSmoName->addAction(lineSmoName_secondAction, QLineEdit::ActionPosition::LeadingPosition);
    connect(lineSmoName_secondAction, &QAction::triggered,
            [this]()->void {SmoUI::lineSmoName_InfoAction_slot(this->UISmo);});
    hBoxLayout->addWidget(lineSmoName);
}

SmoUI::~SmoUI()
{
    hBoxLayout->deleteLater();
    if (SmoData_refreshAction) SmoData_refreshAction->deleteLater();
}

//const QIcon& SmoUI::icon_write = getIcon("write");//TODO:findout why this is not work:        button->setIcon(SmoUI::icon_write);

SmoUITypes SmoUI::UIType()
{
    return uiType;
}

SmoUI* SmoUI::newSmoUI(QWidget *parent, SmoUITypes uitype)
{
    switch (uitype) {
    case SmoUITypes::uiPageHeader:
        return NULL; //TODO: new SmoUIPageHeader(parent);
    case SmoUITypes::uiMessage:
        return new SmoUIMessage(parent);
    case SmoUITypes::uiSelectItem:
        return new SmoUISelectItem(parent);
    case SmoUITypes::uiLogin:
        return new SmoUILogin(parent);
    case SmoUITypes::uiPassLine:
        return new SmoUIPassLine(parent);
    case SmoUITypes::uiTextLine:
        return new SmoUITextLine(parent);
    default:
        break;
    }
    return new SmoUITextLine(parent, uitype); //on default
}

void SmoUI::UIUpdate(Smo *, bool )
{
//    UISmo = smo;
}

void SmoUI::UIDisable()
{
//    UISmo = NULL;
}

//void SmoUI::DebugOnly(QWidget *dob, Smo* smo, QString prefix)
//{
//    int cou = 0;
//    QString dstr = "%1:%2; Smo:%3; object:0x%4(%5); Enabled:%6 %7";
//    for (QWidget*w = dob; w; w = w->parentWidget()) {
//        debug_tree_info(dstr
//                          .arg(cou++)
//                          .arg(prefix)
//                          .arg(smo->ShortName())
//                          .arg((intptr_t)w,0,16)
//                          .arg(w->metaObject()->className())
//                          .arg(w->isEnabled())
//                          .arg(w->isEnabledTo(0)));
//    }

//}

void SmoUI::SetButton_icon_and_tooltip(QPushButton* button, const QString& icon_name, const QString& tool_tip)
{
    if (QString::compare(button->objectName(), icon_name)) {
        button->setObjectName(icon_name);
        button->setIcon(zvvlib::getIcon(icon_name)); //TODO:findout why this is not work: button->setIcon(SmoUI::icon_write);
        button->setToolTip(iif(tool_tip.isNull(), icon_name, tool_tip));
    }
    else
    {
        button->setToolTip(iif(tool_tip.isNull(), icon_name, tool_tip));
    }
}

QString SmoUI::GetFormatTip(const QString& frm)
{
    return iif(frm.isEmpty(), frm, QString(QStringLiteral("Data format: %1")).arg(frm));
}

void SmoUI::TextAndWriteButton_update(SmoUI *ui, QLineEdit *line, QPushButton* button, SmoUIState state)
{
    Smo* smo = ui->UISmo;
    switch (state) {
    case UI_readonly:
        line->setToolTip(QString());
        line->setReadOnly(true);
        line->setEnabled(true);
        line->setVisible(true);
        button->setEnabled(true);
        button->setVisible(false);
        ui->lineSmoName_firstAction->setVisible(true);
        break;
    case UI_readwrite: /*full access*/
        line->setToolTip(QString());
        line->setReadOnly(false);
        line->setEnabled(true);
        line->setVisible(true);
        QWidget::setTabOrder(line,button);
        SetButton_icon_and_tooltip(button, QStringLiteral("write"));
        button->setEnabled(true);
        button->setVisible(true);
        ui->lineSmoName_firstAction->setVisible(true);
        break;
    case UI_write_ok:
        line->setToolTip(QString()); //ok
        line->setStyleSheet(QStringLiteral("background: white"));
        SetButton_icon_and_tooltip(button, QStringLiteral("write"));
        break;
    case UI_write_bad_value:
        line->setToolTip(tr("Change the data: %1").arg(line->text().trimmed()));
        line->setStyleSheet(QStringLiteral("background: pink"));
        SetButton_icon_and_tooltip(button, QStringLiteral("write"), tr("%1: unable convert the data: %2").arg(smo->ShortName()).arg(line->text().trimmed()));
        break;
    case UI_write_io_error:
        line->setToolTip(tr("Write status: %1").arg(Smo::StatusDescript(smo)));
        line->setStyleSheet(QStringLiteral("background: pink"));
        SetButton_icon_and_tooltip(button, QStringLiteral("next"), tr("IO error! Try write again!"));
        break;
    case UI_no_data:
    default:
        ui->lineSmoName_firstAction->setVisible(false);
        line->setEnabled(false);
        line->setVisible(false);
        button->setEnabled(false);
        button->setVisible(false);
        break;
    }
}

void SmoUI::LineText_update(QLineEdit *line, Smo* smo)
{
    if (smo->isCallSuccessful()) {
        line->setText(Smo::TargetMemoryToString(smo));
//        line->setToolTip(QString()); //line->setPlaceholderText("");
        line->setStyleSheet(QStringLiteral("background: white"));
    } else {
        line->clear();
//        line->setToolTip(Smo::StatusDescript(smo)); //line->setPlaceholderText(Smo::StatusDescript(smo));
        line->setStyleSheet(QStringLiteral("background: pink"));
    }
    line->setReadOnly(Smo::AccessState(smo) != SmoAccess::FullAccess);
}

void SmoUI::SmoInviter_update(QLineEdit *line, QAction *lineInfoAction, Smo *smo)
{
    line->setText(smo->inviteString());
    line->setToolTip(SmoUI::GetFormatTip(QString(Smo::DisplayFormat(smo))));
    const char* smoImage = Smo::SelectIcon(smo);
    if (strcmp(smo->iconName(), smoImage)) {
        lineInfoAction->setIcon(zvvlib::getIcon(smoImage));
    } else {
        lineInfoAction->setIcon(smo->icon(/*0*/));
    }
    if (smo->mainStatusCode() == msg_Success) {
        lineInfoAction->setToolTip(QString());
    }
    else {
        lineInfoAction->setToolTip(Smo::StatusDescript(smo));
    }
    line->setVisible(true);
}

void SmoUI::lineSmoName_trackAction_slot(Smo *smo)
{
    if (smo != NULL) {
        Log::addWarning(tr("%1: %2: Track action for: %3").arg(smo->ShortName()).arg("").arg(Smo::Path(smo, path_absolute)));
    }
}

void SmoUI::lineSmoName_InfoAction_slot(Smo *smo)
{
    if (smo != NULL) {
        if (smo->mainStatusCode() == msg_Success) {
            Log::addInfo(tr("%1: Info action : %2").arg(smo->ShortName()).arg(Smo::Path(smo, path_absolute)));
        }
        else {
            Log::addError(tr("%1: %2").arg(Smo::Path(smo, path_absolute)).arg(Smo::StatusDescript(smo)));
        }
    }
}

//void SmoUI::lineDataChangedAction_slot(Smo *smo, QPushButton *button, QLineEdit *data)
//{
//    disconnect(data, &QLineEdit::textChanged, 0, 0);
//    SmoUI::TextAndWriteButton_update(smo, data, button, UI_readwrite);
//}

void SmoUI::RefreshAction_update(SmoUI *ui, QWidget *parentWidget, bool isToBeEnabled)
{
    if (isToBeEnabled) {
        if (ui->SmoData_refreshAction == NULL) {
            ui->SmoData_refreshAction = new QAction(parentWidget);
            ui->SmoData_refreshAction->setIcon(zvvlib::getIcon(QStringLiteral("refresh")));
            ui->SmoData_refreshAction->setVisible(false);
            connect(ui->SmoData_refreshAction, &QAction::triggered, [ui]()->void {ui->UIUpdate(ui->UISmo);});
        }
        if (!ui->SmoData_refreshAction->isVisible()) {
            ui->SmoData_refreshAction->setVisible(true);
            if (QLineEdit*le = qobject_cast<QLineEdit*>(parentWidget)) {
                le->addAction(ui->SmoData_refreshAction, QLineEdit::ActionPosition::LeadingPosition);
            }
            else {
                parentWidget->addAction(ui->SmoData_refreshAction);
            }
        }
    } else {
        if (ui->SmoData_refreshAction != NULL && ui->SmoData_refreshAction->isVisible()) {
            parentWidget->removeAction(ui->SmoData_refreshAction);
            ui->SmoData_refreshAction->setVisible(false);
        }
    }
}

void SmoUI::UIToBeUpdated()
{
    Smo* smo = qobject_cast<Smo*>(sender());
    if (smo) {
        debug_func(smo->ShortName());
        UIUpdate(smo, true);
    }
    else {
        debug_func("no smo");
    }
}

//const EMap<> SmoUI::WriteButtonStatesMap      = EMap<>(WriteButtonStates<>().Map);
