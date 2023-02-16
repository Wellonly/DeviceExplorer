#ifndef SMOUI_H
#define SMOUI_H

//next includes must be after SmoUI class definition!!!
//#include "smouipageheader.h"
//#include "smouitextline.h"
//#include "smouiMessage.h"

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "smo.h"

class SmoUI : public QWidget
{
    Q_OBJECT
public:
    explicit SmoUI(QWidget *parent, SmoUITypes uitype);
    ~SmoUI();

    SmoUITypes UIType();

    typedef enum SmoUIState
    { //
        UI_no_data         = SmoAccess::NoDataAccess, /*0*/
        UI_readonly        = SmoAccess::ReadOnly,     /*1*/
        UI_readwrite       = SmoAccess::FullAccess,   /*2*/
        UI_invisible       = SmoAccess::Invisible,    /*3*/
        UI_write_ok        = 4,
        UI_write_bad_value = 5,
        UI_write_io_error  = 6,
    } SmoUIState;

    static SmoUI* newSmoUI(QWidget *parent, SmoUITypes UIType);

    static void SetButton_icon_and_tooltip(QPushButton* button, const QString &icon_name, const QString &tool_tip = QString());
    static QString GetFormatTip(const QString &frm);

    virtual void UIUpdate(Smo* smo, bool isViewUpdateOnly = false);
    virtual void UIDisable();

//    static void DebugOnly(QWidget *dob, Smo *smo, QString prefix);

protected:
    static void TextAndWriteButton_update(SmoUI *ui, QLineEdit *line, QPushButton* button, SmoUIState state);
    static void LineText_update(QLineEdit *line, Smo* smo);
    static void SmoInviter_update(QLineEdit *line, QAction *lineInfoAction, Smo* smo);
    static void RefreshAction_update(SmoUI *ui, QWidget* parentWidget, bool isToBeEnabled);

public slots:
    void UIToBeUpdated();
    static void lineSmoName_trackAction_slot(Smo *smo);
    static void lineSmoName_InfoAction_slot(Smo *smo);
//    static void lineDataChangedAction_slot(Smo *smo, QPushButton *button, QLineEdit *data);

public: //private:
    const SmoUITypes uiType;
    Smo*             UISmo = NULL;
    QHBoxLayout* hBoxLayout;

protected:
    QLineEdit*   lineSmoName;
    QAction*     lineSmoName_firstAction; //old:track
    QAction*     lineSmoName_secondAction; //old:info

private:
    QAction*     SmoData_refreshAction = NULL;

};

#include "smouipageheader.h"
#include "smouitextline.h"
#include "smouilogin.h"
#include "smouipassline.h"
#include "smouiMessage.h"

#endif // SMOUI_H
