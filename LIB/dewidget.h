#ifndef DEWIDGET_H
#define DEWIDGET_H

#include <stdbool.h>
#include <QDockWidget>
#include <QObject>
#include <QAction>
#include <QDateTime>
#include <QPointer>

//enum smopath {
//    smopath_inDevice = 0,
//    smopath_full = 1,
//};

class Smo;
class DEWidget : public QDockWidget
{
    Q_OBJECT
public:
    DEWidget(QMainWindow *parent, Smo *smo, const QString& idname = QString());

public slots:
    void DEHide_slot();
    void DERaise_slot(Smo *smo, bool isFormToBeRefreshed);

protected:
    bool isSmoToBeRefreshed = false;

    virtual void Form_refresh(Smo *smo, bool isFormToBeRefreshed);
    Smo *smoRef() const;
    void setSmoRef(Smo *value);
    void updateRefreshTime();
    QDateTime refreshTime() const;

private:
    QPointer<Smo> m_smo;
    QAction *hideAction = 0;
    QAction *raiseAction = 0;
    QDateTime m_refreshTime;
};

#endif // DEWIDGET_H
