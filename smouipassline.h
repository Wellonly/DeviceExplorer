#ifndef SMOUIPASSLINE_H
#define SMOUIPASSLINE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "smo.h"
#include "smoui.h"

class SmoUIPassLine : public SmoUI
{
public:
    SmoUIPassLine(QWidget *parent);
    ~SmoUIPassLine();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

private:
    QLineEdit*   lineSmoData;
    QPushButton* buttSmoData;

private slots:
    static void buttSmoDataWrite_slot(SmoUIPassLine *ui);
private:

};

#endif // SMOUIPASSLINE_H
