#ifndef FDEFAULT_H
#define FDEFAULT_H

#include <QCheckBox>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>

#include <smo.h>

#include "smoui.h"

#include "app_config.h"
#include "dewidget.h"

#ifndef DEWIDGET_DEFAULT_NAME
#define DEWIDGET_DEFAULT_NAME "DE.fDefault"
#endif
//#define ColumnCount 4

class fDefault : public DEWidget
{
    Q_OBJECT
public:
   fDefault(QMainWindow *parent, Smo *smo, const QString& idname = QString(DEWIDGET_DEFAULT_NAME));

   static void PageHeader_update(Smo* smo);

signals:

public slots:

private slots:

private:
    static const int rowPageHeader   = 0;
    static const int rowParentUI     = 1;
    static const int rowFirstChild_UI_forParentDevice = 1;
    static const int rowFirstChild_UI_forParentNotDevice = 2;

    QList<SmoUI*> ExcludedUIList;

    QVBoxLayout* vboxLayout;
    QLayoutItem* endOfListSpacer;

    SmoUIPageHeader* uiPageHeader;
    SmoUI* uiParentData;

    void Form_refresh(Smo *smo, bool isFormToBeRefreshed);
    void ChildrenUIUpdate(int childs, Smo *parentSmo, const int rowFirstChild_UI);
    void ChildrenUIsDisable(int rowsToRemove);

    void smoUIDisable(int row);
    SmoUI *InsertSmoUI(SmoUITypes smoUItype, int row);
    SmoUI *ChangeSmoUIOnType(SmoUITypes smoUItype, int row);
    SmoUI * GetSmoUI(SmoUITypes smoUItype);

    SmoUI* getExcludedUI(SmoUITypes uitype);

};

#endif // FDEFAULT_H
