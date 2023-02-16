#ifndef DEPLUGIN_H
#define DEPLUGIN_H

#include <QObject>
#include <QString>
#include <QWidget>

class DEWidget;

class DEPlugin : public QObject
{
    //excluded if no slots/signals in the class... Q_OBJECT
public:
//    explicit DEPlugin(const QString &fName, QObject* parent = 0);
    DEPlugin(const QString &appNameSpace, const QString &appFormName, QObject *parent = 0);
    ~DEPlugin();

//vars...
    DEWidget *expandedWidget = 0;
//    QString pluginFName;
    QString AppLink;

signals:

public slots:

public:

};

#endif // DEPLUGIN_H
