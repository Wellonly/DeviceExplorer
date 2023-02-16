#ifndef SMOUISELECTITEM_H
#define SMOUISELECTITEM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "smo.h"
#include "smoui.h"

typedef QMap<int, Smo*> Int2SMO_map;

class SmoUISelectItem : public SmoUI
{
public:
    SmoUISelectItem(QWidget *parent);
    ~SmoUISelectItem();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

public: /*static...*/
    static void Combox_update(QComboBox* combox, Smo* smo);
    static void AddNotSelectedItem(QComboBox *combox, smoKey data);

public slots:
    static void comboxListUpdate_slot(Smo *smo, QComboBox *combox, int currIndex);

private:
    Int2SMO_map  comboxItemsMap;

    QAction*     lineSmoName_refreshAction;
//    QLineEdit*   lineInCombo;
    QComboBox*   comboxSmoSelect;

private slots:
    static void comboxSelectItem_slot(SmoUISelectItem *ui, int currIndex);
    static void SmoName_refreshAction_slot(SmoUISelectItem *ui);
private:

};

#endif // SMOUISELECTITEM_H
