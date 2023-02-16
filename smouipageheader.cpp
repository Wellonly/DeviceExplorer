#include "smouipageheader.h"
#include "LogTree.h"

static const char* Login_icon_names[] = { "UserGroup" /*user*/
                                         ,"UserGroup" /*power user*/
                                         ,"UserGroup" /*owner*/
                                         ,"UserGroup" /*tuner*/
                                         ,"group0" /*next: for logined ones*/
                                         ,"group1"
                                         ,"group2"
                                         ,"group3"};

SmoUIPageHeader::SmoUIPageHeader(QWidget *parent)
: SmoUI(parent, SmoUITypes::uiPageHeader)
{
//reconfig lineSmoName Actions...
    lineSmoName_firstAction->setIcon(zvvlib::getIcon(QStringLiteral("device")));
    disconnect(lineSmoName_firstAction, &QAction::triggered,0,0);
    connect(lineSmoName_firstAction, &QAction::triggered,
            [this]()->void {SmoUIPageHeader::Device_status_slot(this);});
    disconnect(lineSmoName_secondAction, &QAction::triggered, 0,0);
    connect(lineSmoName_secondAction, &QAction::triggered,
            [this]()->void {SmoUIPageHeader::Change_login_status_slot(this);});
    lineSmoName_secondAction->setToolTip("Change login status");

    checkTimer = new QCheckBox(this);
    hBoxLayout->addWidget(checkTimer);

    buttPageRefresh = new QPushButton(zvvlib::getIcon(QStringLiteral("refresh")), "", this);
    buttPageRefresh->setToolTip(tr("page refresh"));

    hBoxLayout->addWidget(buttPageRefresh);
}

SmoUIPageHeader::~SmoUIPageHeader()
{
    buttPageRefresh->deleteLater();
    checkTimer->deleteLater();
}

void SmoUIPageHeader::UIUpdate(Smo *smo, bool isViewUpdateOnly)
{
    (void)isViewUpdateOnly;
    UISmo = smo;
    DeviceInfo* inf = smo->deviceInfo();
//lineSmoName update...
    if (inf->getUserName().isEmpty()) {
        lineSmoName->setText(inf->DeviceName());
    }
    else {
        lineSmoName->setText(QStringLiteral("%1::%2").arg(inf->DeviceName()).arg(inf->getUserName()));
    }
//secondAction(Login status) update...
    Smo* loginSMO = inf->getLoginSMO();
    if (loginSMO) {
        int icon_index = inf->getUserGroup(); //UserGroup_masked(
        if (IconOnLoginAction_index != icon_index) {
            IconOnLoginAction_index = icon_index;
            lineSmoName_secondAction->setIcon(zvvlib::getIcon(Login_icon_names[icon_index])); //getIcon(inf->getUserGroupName())
        }
        lineSmoName_secondAction->setVisible(true);
    }
    else {
        lineSmoName_secondAction->setVisible(false);
    }
    lineSmoName->setVisible(true);
//    lineSmoName->setToolTip("user group");
}

void SmoUIPageHeader::UIDisable()
{

}

void SmoUIPageHeader::UIUpdateRefreshTime(QDateTime dtStartTime)
{
    qint64 duration_mS = dtStartTime.msecsTo(QDateTime::currentDateTime());
    checkTimer->setText(QString("%1 (%2 mS)").arg(dtStartTime.toString(Qt::ISODate)).arg(duration_mS));
}

void SmoUIPageHeader::Change_login_status_slot(SmoUIPageHeader *ui)
{
    Smo* smo = ui->UISmo;
    DeviceInfo* inf = iif(smo, smo->deviceInfo(), NULL);
    Smo* loginSMO = iif(inf, inf->getLoginSMO(), NULL);
    QByteArray clear_cmd(2,'\0');
    if (inf->IsUserAccepted()) {
        if (loginSMO) {
            if (Smo::Call(loginSMO, &clear_cmd)) DeviceInfo::LoginedUser_update(loginSMO);

        }
    }
    else {
        if (loginSMO) {
            if (Smo::Call(loginSMO, iif(inf->getDescPtr()->length(), inf->getDescPtr(), &clear_cmd))) {
                DeviceInfo::LoginedUser_update(loginSMO);
            }
        }
    }
    ui->UIUpdate(smo);
}

void SmoUIPageHeader::Device_status_slot(SmoUIPageHeader *ui)
{
    Smo* smo = ui->UISmo;
    DeviceInfo* inf = iif(smo, smo->deviceInfo(), NULL);
    if (inf) {
        Log::addInfo(tr("Device: %1; Logined user: %2").arg(inf->DeviceName()).arg(iif(inf->getUserName().isEmpty(), QStringLiteral("no logined"), inf->getUserName())));
    }
    else {
        Log::addError(tr("No device info at this time"));
    }
}

