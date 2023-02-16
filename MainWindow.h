/****************************************************************************
**
** Copyright (C) 2014 Well and/or its subsidiary(-ies).
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainTreeWidget.h"
#include "zvvlib.h"
#include "deconfig.h"

#include <QMainWindow>
#include <QSignalMapper>
#include <QTextEdit>

#ifndef DAFAULT_SETTINGS_FILENAME
#define DAFAULT_SETTINGS_FILENAME "default.cfg"
#endif

#ifndef Q_OS_MAC
#define WFlags 0
#else
#define WFlags Qt::Drawer
#endif

class ToolBar;
class MainTreeWidget;
class DEWidget;
class Log;
//class DESettings;

QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

typedef void (MainWindow::*MenuHandler)(QDockWidget* widgt, QAction *action);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QList<char*>& arglist, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

    DEConfig *getAppConf() const;
    DEWidget* getCurrentDEWidget() const;

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
public slots:
    void actionTriggered(QAction *action);

    void saveLayout();

    void loadLayout();

    void setCorner(int id);
    void setDockOptions();
    void setGlobalOptions();

    void HelpContent();
    void HelpAbout();

    void openDefaultDEWidget(bool checked);
    void refreshDEWidget(bool checked);

private slots:
    void changeVerticalTitleBarSlot(QDockWidget* widget, bool newValue);
    void tabInto(QDockWidget* widgt, QAction *action);
    void splitIntoV(QDockWidget* widgt, QAction *action);
    void splitIntoH(QDockWidget* widgt, QAction *action);

private:
    QTextEdit *center;
    QList<ToolBar*> toolBars;
    QMenu *dockWidgetMenu;
    QMenu *editMenu;
    QMenu *mainWindowMenu;
    QSignalMapper *mapper;
    Log *mainLogTree;
    //QList<QDockWidget*> mainWidgetList;
    MainTreeWidget* mainTreeWidget;

    const char *Profile = DAFAULT_SETTINGS_FILENAME;
    void setupToolBar();
    void setupMenuBar();
    void setupDockWidgets(/*const Config &config*/);

    void saveLayout(const QString &FName);

    void loadLayout(const char *fileName);
    void loadLayout(const QString &FName);

    void splitInto(QDockWidget* widgt, QAction *action, bool isVerticallyOrHorizontally);

    DEConfig* appConf;
};


#endif
