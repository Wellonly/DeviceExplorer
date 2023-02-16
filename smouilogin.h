#ifndef SMOUILOGIN_H
#define SMOUILOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "smo.h"
#include "smoui.h"

class SmoUILogin : public SmoUI
{
public:
    SmoUILogin(QWidget *parent);
    ~SmoUILogin();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

private:
    QComboBox*   comboxSmoLogin;
    QLineEdit*   lineSmoData;
    QPushButton* buttSmoData;

private slots:
    static void buttSmoDataWrite_slot(SmoUILogin *ui);

private:

};

#endif // SMOUILOGIN_H
