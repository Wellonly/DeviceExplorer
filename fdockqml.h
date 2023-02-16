#ifndef FDOCKQML_H
#define FDOCKQML_H

#include <QDockWidget>
#include <QMainWindow>
#include <QQuickWidget>
#include <QQuickItem>
#include <QQmlError>
#include <QUrl>
#include <QFileInfo>

#include "dewidget.h"

class Smo;
class fDockedQml : public DEWidget
{
    Q_OBJECT
public:
    fDockedQml(QMainWindow *parent, Smo *smo, const QString& idname = QString(), const QString& smoqml = QString());
    ~fDockedQml();

public slots:
    void quickWidgetStatusChanged(QQuickWidget::Status status);
    void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);

private:
    QUrl m_qmlurl;
    QQuickWidget* m_quickWidget = 0;
    QFileInfo m_finfo;

    void Form_refresh(Smo *smo, bool isFormToBeRefreshed) override;
    QQuickWidget *QMLWidgetLoad(const QUrl& url);
    void QMLWidgetUnload(QQuickWidget *quickWidget);
};

#endif // FDOCKQML_H
