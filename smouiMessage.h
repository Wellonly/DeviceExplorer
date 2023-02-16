#ifndef SmoUIMessage_H
#define SmoUIMessage_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "smo.h"
#include "smoui.h"

class SmoUIMessage : public SmoUI
{
public:
    SmoUIMessage(QWidget *parent);
    ~SmoUIMessage();

    void UIUpdate(Smo *smo, bool isViewUpdateOnly = false);
    void UIDisable();

private:
    QLabel *lblSmoName;
    QLineEdit* lineSmoData;
    QPushButton *buttSmoData;

};

#endif // SmoUIMessage_H
