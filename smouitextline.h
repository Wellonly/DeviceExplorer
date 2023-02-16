#ifndef SMOUITEXTLINE_H
#define SMOUITEXTLINE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "smo.h"
#include "smoui.h"

class SmoUITextLine : public SmoUI
{
public:
    SmoUITextLine(QWidget *parent, SmoUITypes uitype = SmoUITypes::uiTextLine);
    ~SmoUITextLine();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

private:
    QLineEdit*   lineSmoData;
    QPushButton* buttSmoData;

private slots:
    static void buttSmoDataWrite_slot(SmoUITextLine *ui);

private:

};

#endif // SMOUITEXTLINE_H
