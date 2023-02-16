
#include "app_config.h" //#include "zvvlib.h"
#include "fdefault.h"
#include "dewidget.h"
#include "LogTree.h"
#include "SmoRecord.h"

#include <QDateTime>
#include <QDebug>
//using namespace std;

static fDefault* fDefaultPtr = NULL;

fDefault::fDefault(QMainWindow *parent, Smo *smo, const QString &idname)
: DEWidget(parent, smo, idname)
{
    fDefaultPtr = this;
    isSmoToBeRefreshed = true;

    //Grid = GridLayout(this); //zvv:added the VBoxLayout upon a QDockWidget
    vboxLayout = zvvlib::VBoxLayout(this);

    uiPageHeader = new SmoUIPageHeader(this);
    vboxLayout->addWidget(uiPageHeader);
    connect(uiPageHeader->buttPageRefresh, &QPushButton::clicked, [this]() {DERaise_slot(smoRef(), true);});


    uiParentData = SmoUI::newSmoUI(this, Smo::UIType(smo));
    vboxLayout->addWidget(uiParentData);

    vboxLayout->addStretch(0);
    endOfListSpacer = vboxLayout->itemAt(vboxLayout->count()-1);

    DERaise_slot(smo, true); // Form_refresh(smo, true);
}

void fDefault::PageHeader_update(Smo* smo)
{
    if (fDefaultPtr) fDefaultPtr->uiPageHeader->UIUpdate(smo);
}

SmoUI* fDefault::GetSmoUI(SmoUITypes smoUItype)
{
    SmoUI* UI = getExcludedUI(smoUItype);
    if (UI == NULL) UI = SmoUI::newSmoUI(this, smoUItype);
    return UI;
}

SmoUI* fDefault::InsertSmoUI(SmoUITypes smoUItype, int row)
{
    SmoUI* UI = GetSmoUI(smoUItype);
    vboxLayout->insertWidget(row, UI);
    UI->setEnabled(true);
    UI->setVisible(true);
//    vboxLayout->update();
    return UI;
}

SmoUI* fDefault::ChangeSmoUIOnType(SmoUITypes smoUItype, int row)
{
    SmoUI* UI = GetSmoUI(smoUItype);
    smoUIDisable(row);
    vboxLayout->insertWidget(row, UI);
    UI->setEnabled(true);
    UI->setVisible(true);
    return UI;
}

void fDefault::ChildrenUIUpdate(int childs, Smo *parentSmo, const int rowFirstChild_UI)
{
    for (int i = 0; i < childs; ++i) {
        Smo* smo = parentSmo->child(i);
        int row = rowFirstChild_UI + i;
        QLayoutItem* item =	vboxLayout->itemAt(row);
        SmoUITypes smoUItype = Smo::UIType(smo);
        SmoUI* UI;
        if (item == NULL || item->isEmpty()) {
            UI = InsertSmoUI(smoUItype, row);
        }
        else
        {
            UI = qobject_cast<SmoUI*>(item->widget());
            if (!UI || UI->UIType() != smoUItype) {
                UI = ChangeSmoUIOnType(smoUItype, row);
            }
        }
        UI->UIUpdate(smo);
        connect(smo, &Smo::dataChanged, UI, &SmoUI::UIToBeUpdated, SMO::Connection_unique_auto);
    }
}

void fDefault::smoUIDisable(int row)
{
    QLayoutItem* item = vboxLayout->itemAt(row);
    SmoUI* widget = iif(item, qobject_cast<SmoUI*>(item->widget()), NULL);
    if (widget)
    {
        widget->UIDisable();
        ExcludedUIList.append(widget);
        widget->setHidden(true);
    }
    if (item) {
        item->invalidate();
        vboxLayout->takeAt(row);
    }
}

void fDefault::ChildrenUIsDisable(int rowsToRemove)
{
    int currRow = vboxLayout->count();
    for (int i = 0; i < rowsToRemove; ++i) {
        smoUIDisable(--currRow);
    }
}

SmoUI *fDefault::getExcludedUI(SmoUITypes uitype)
{
    for (int i=0; i< ExcludedUIList.length(); ++i) {
        if (ExcludedUIList[i]->UIType() == uitype) {
            return ExcludedUIList.takeAt(i);
        }
    }
    return NULL;
}

void fDefault::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    if (isFormToBeRefreshed) {
        bool nodesToBeRefreshed = smo->isExpanded();
        int childs = nodesToBeRefreshed ? smo->childCount() : 0;
//form refresh...
        uiPageHeader->UIUpdate(smo);
//Parent row update...
        int rowFirstChild_UI;
        if (smo == smo->deviceInfo()->deviceItem()) {
            rowFirstChild_UI = rowFirstChild_UI_forParentDevice;
        }
        else
        { ///ParentData update...
            SmoUITypes smoUItype = Smo::UIType(smo);
            if (uiParentData->UIType() != smoUItype) {
                uiParentData = ChangeSmoUIOnType(smoUItype, rowParentUI);
            }
            uiParentData->UIUpdate(smo);
            rowFirstChild_UI = rowFirstChild_UI_forParentNotDevice;
        }
///children rows update...
        childs = nodesToBeRefreshed ? smo->childCount() : 0; //refresh child count!!!
        int rows = vboxLayout->count();
        if (childs) {
            ChildrenUIUpdate(childs, smo, rowFirstChild_UI);
        }
        if (rows-rowFirstChild_UI-1 > childs) { //-1 for skipping a QSpacerItem* endOfListSpacer
            ChildrenUIsDisable(rows-rowFirstChild_UI-childs);
        }
        //Grid->update();
    }
    if (vboxLayout->itemAt(vboxLayout->count()-1) != endOfListSpacer) {
        vboxLayout->insertItem(-1, endOfListSpacer);
    }
    vboxLayout->invalidate();
    vboxLayout->update();
    setMinimumSize(vboxLayout->sizeHint());
    uiPageHeader->UIUpdateRefreshTime(refreshTime()/*startDT*/);
}


