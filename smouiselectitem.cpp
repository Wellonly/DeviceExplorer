

#include <QApplication>

#include "smo.h"
#include "smouiselectitem.h"
#include "zvvdebug.h"

SmoUISelectItem::SmoUISelectItem(QWidget *parent)
: SmoUI(parent, SmoUITypes::uiSelectItem)
{
    lineSmoName_refreshAction = new QAction(lineSmoName);
    lineSmoName_refreshAction->setIcon(zvvlib::getIcon("refresh"));
    lineSmoName_refreshAction->setVisible(true);
    lineSmoName->addAction(lineSmoName_refreshAction, QLineEdit::ActionPosition::LeadingPosition);
    connect(lineSmoName_refreshAction, &QAction::triggered,
            [this]()->void {SmoUISelectItem::SmoName_refreshAction_slot(this);});

    comboxSmoSelect = new QComboBox(this);
    comboxSmoSelect->setEditable(false);
    comboxSmoSelect->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    comboxSmoSelect->setVisible(false);
    connect(comboxSmoSelect, static_cast<void(QComboBox::*)(int)>(&QComboBox::highlighted),
          [this](int index)->void {SmoUISelectItem::comboxListUpdate_slot(this->UISmo, this->comboxSmoSelect, index);});
    connect(comboxSmoSelect, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
          [this](int index)->void {SmoUISelectItem::comboxSelectItem_slot(this, index);});
    hBoxLayout->addWidget(comboxSmoSelect);

//    debug_tree_info(tr("buttSmoData: %1").arg("created!!!"));
}

SmoUISelectItem::~SmoUISelectItem()
{
//    disconnect(comboxSmoSelect, 0, 0, 0);
    comboxSmoSelect->deleteLater();
    lineSmoName->deleteLater();
}

void SmoUISelectItem::UIDisable()
{
    lineSmoName->setVisible(false);
    comboxSmoSelect->setEnabled(false);
    comboxSmoSelect->setVisible(false);
}

void SmoUISelectItem::UIUpdate(Smo* smo, bool isViewUpdateOnly)
{
    UISmo = smo;
    SmoUI::SmoInviter_update(lineSmoName, lineSmoName_secondAction, smo);
    SmoAccess smoAccess = Smo::AccessState(smo);
    bool isDataToBeVisible = Smo::IsAccessible(smoAccess) && Smo::IsDataEnabled(smo);
    if (isDataToBeVisible)
    {
        if (!isViewUpdateOnly) Smo::Call(smo);

        Combox_update(comboxSmoSelect, smo);
    }
    lineSmoName_firstAction->setVisible(isDataToBeVisible);
    comboxSmoSelect->setEnabled(smoAccess == SmoAccess::FullAccess);
    comboxSmoSelect->setVisible(isDataToBeVisible);
}

void SmoUISelectItem::Combox_update(QComboBox *combox, Smo* smo)
{
    combox->blockSignals(true);
    combox->clear();
    if (smo->isCallSuccessful() && (uint)smo->targetMemory()->length() >= sizeof (smoKey))
    {
        QString itemText;
        QString itemToolTipText;
        smoKey_t targetKeyValue;
        targetKeyValue.Key = *(smoKey*)smo->targetMemory()->data();
        smokey isItemSelectedAndkey = iif(targetKeyValue.KeyTagValue, 0, targetKeyValue.Key);
        Smo* selectedSMO;
        if (isItemSelectedAndkey)
        { // already selected mode...
            selectedSMO = Smo::Findkey(smo->deviceInfo(), isItemSelectedAndkey);
            if (selectedSMO) {
                if (!selectedSMO->iconName()) Smo::SetIcon(selectedSMO, Smo::SelectIcon(selectedSMO));
                SmoUISelectItem::AddNotSelectedItem(combox, selectedSMO->Parent_key() | smoSelectKey_NotSelected_bm);
                itemText = Smo::ShortName(selectedSMO);
                combox->addItem(selectedSMO->icon(/*0*/), itemText, QVariant::fromValue(targetKeyValue.Key));
            } else {
                itemToolTipText = tr("not found");
                combox->addItem(zvvlib::getIcon(QStringLiteral("warning")), itemText, QVariant::fromValue((smokey)0));
            }
            combox->setCurrentText(itemText);
        }
        else
        { // not selected mode...
            SmoUISelectItem::AddNotSelectedItem(combox, targetKeyValue.Key);
        }
        combox->setToolTip(itemToolTipText); //line->setPlaceholderText("");
        combox->setStyleSheet(QStringLiteral("background: white"));
    } else {
        combox->setToolTip(Smo::StatusDescript(smo)); //line->setPlaceholderText(Smo::StatusDescript(smo));
        combox->setStyleSheet(QStringLiteral("background: pink"));
    }
    combox->setEnabled(Smo::AccessState(smo) == SmoAccess::FullAccess);
    combox->setStyleSheet(QStringLiteral("QComboBox:hover:enabled { color: blue }"));
    combox->blockSignals(false);
}

void SmoUISelectItem::AddNotSelectedItem(QComboBox *combox, smoKey data)
{
    QString itemText = tr("not selected");
    combox->addItem(zvvlib::getIcon(QStringLiteral("info")), itemText, QVariant::fromValue(data));
    combox->setCurrentText(itemText);
}

void SmoUISelectItem::comboxSelectItem_slot(SmoUISelectItem* ui, int currIndex)
{
    Smo* smo = ui->UISmo;
    QComboBox* combox = ui->comboxSmoSelect;
    debug_tree_info(tr("comboxSelectItem_slot: %1 : %2 : %3").arg(smo->ShortName()).arg(currIndex).arg(combox->itemData(currIndex).value<smokey>(),0,16));
    smoKey_t skey = {combox->itemData(currIndex).value<smokey>()};
    if (skey.KeyTagValue) skey.Key = 0; //if tag!=0 then unselected value, so reset it by zeroing!!!
    if (Smo::Call(smo, skey.Key))
    {
        combox->setToolTip(QString());
//find out why this is brake the style...        combox->setStyleSheet(QStringLiteral("background: white"));
    } else {
        combox->setToolTip(tr("Write status: %1").arg(Smo::StatusDescript(smo)));
//find out why this is brake the style...        combox->setStyleSheet(QStringLiteral("background: pink"));
    }
}

void SmoUISelectItem::comboxListUpdate_slot(Smo* smo, QComboBox* combox, int currIndex)
{
    debug_tree_info(tr("comboxListUpdate_slot: %1 : %2 : %3").arg(smo->ShortName()).arg(currIndex).arg(combox->count()));
    smokey selected_key, pkey;
    Smo* selectedSMO = NULL;
    Smo* parentSMO = NULL;
    if (combox->count()) {
        smoKey_t currKey = {combox->itemData(currIndex).value<smoKey>()};
        if (currKey.KeyTagValue) {
            pkey = Smo::SmoKey2key(currKey.Key);
            selected_key = 0;
        }
        else {
            selected_key = currKey.Key;
            if (selected_key) selectedSMO = Smo::Findkey(smo->deviceInfo(), selected_key);
            pkey = iif(selectedSMO, selectedSMO->Parent_key(), 0);
        }
        parentSMO = Smo::Findkey(smo->deviceInfo(), pkey);
    }
    else {
        selected_key = 0;
        pkey = 0;
    }

    if (parentSMO)
    {
        if (!(parentSMO->flags() & SmoFlags::FlagAllChildrenRead)) Smo::RefreshChildren(parentSMO);
        int children = parentSMO->childCount();
        for (int i = 0; i < children; i++) {
            Smo* child = parentSMO->child(i);
            int cfound = combox->findData(QVariant::fromValue(child->key()));
            if (cfound < 0) combox->insertItem(i+1, child->icon(/*0*/), child->ShortName(), QVariant::fromValue(child->key()));
        }
        debug_tree_info(tr("parentSMO: %1 : 0x%2; children: %3").arg(parentSMO->ShortName()).arg(parentSMO->key(),0,16).arg(children));
//        if (children) {
//            connect(combox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
//                  [smo, combox](int index)->void {SmoUISelectItem::comboxSelectItem_slot(smo, combox, index);});
//        }
    }
    else {
        debug_tree_info(tr("parentSMO: not found"));
    }
}

void SmoUISelectItem::SmoName_refreshAction_slot(SmoUISelectItem* ui)
{
    if (ui->UISmo) ui->UIUpdate(ui->UISmo);
}
