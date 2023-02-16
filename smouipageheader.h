#ifndef SMOUIPAGEHEADER_H
#define SMOUIPAGEHEADER_H

#include <QWidget>
#include <QLineEdit>

#include "smo.h"
#include "smoui.h"

//#define GROUP_ICON_PREFIX "group"

class SmoUIPageHeader : public SmoUI
{
public:
    SmoUIPageHeader(QWidget *parent);
    ~SmoUIPageHeader();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

    void UIUpdateRefreshTime(QDateTime dtStartTime);

    QCheckBox*   checkTimer;
    QPushButton* buttPageRefresh;

private:
    static void Change_login_status_slot(SmoUIPageHeader* ui);
    static void Device_status_slot(SmoUIPageHeader* ui);

    int IconOnLoginAction_index = -1;
//    QAction*     loginStatusAction;
//    LoginedUserGroup_t loginedGroup;

};

#endif // SMOUIPAGEHEADER_H
