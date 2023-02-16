/****************************************************************************
**
** Copyright (C) 2014 Well and/or its subsidiary(-ies).
**
****************************************************************************/

#include "app_config.h"
#include "zvvdebug.h"
#include "LogTree.h"
#include "MainWindow.h"
#include "toolbar.h"
#include "LocalHostWidget.h"
#include "fdefault.h"

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <qdebug.h>
#include <QTreeWidget>

static const char * const message =
    "<p><b> Device explorer 0.1.0 </b></p>"

    "<p><b> Usage: DE.exe [layout_settings] </b></p>"
    "<p><b> layout_settings: a filename with settings previosly saved. </b></p>"
    "<p><b> (See: Save/Load in File menu)</b></p>"

    "<p> Copyright 2013-2015 Valentin Zotov. All rights reserved. </p>"

    "<p> E-mails: wellmanmail@gmail.com; wellman@mail.ru </p>"

    "<p> Powered by Qt </p>"

#ifdef Q_OS_MAC
/*    "<p>On Mac OS X, the \"Black\" dock widget has been created as a "
    "<em>Drawer</em>, which is a special kind of QDockWidget.</p>" */
#endif
    ;

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

MainWindow::MainWindow(const QList<char*>& arglist, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    setObjectName("MainWindow");
    setWindowTitle(qApp->applicationName());
    setWindowIcon(zvvlib::getIcon("Well"));

    if (arglist.length()>1) {
        if (strcmp(arglist[1],"/?") == 0 || strcmp(arglist[1],"?")==0) {
            HelpAbout();
        }
        else
        {
            Profile = arglist[1];
        }
    }

    appConf = new DEConfig(this, qApp->applicationName().append(".conf"));

    QMainWindow::setDockOptions(AnimatedDocks | AllowNestedDocks | AllowTabbedDocks);

    setupToolBar();

    setupMenuBar();

    setupDockWidgets(/*config_main*/);

    center = new QTextEdit(this);
    center->setReadOnly(true);
    center->setMinimumSize(0, 0);
    center->setMaximumSize(0, 9999);
    setCentralWidget(center);

    if (arglist.length()>1)
    { //load profile if specified by app's parameters!!!
        loadLayout(Profile);
    }
    else
    { //on default settings...
        resize(800,600);
    }
}

MainWindow::~MainWindow()
{
    delete appConf;
}

void MainWindow::actionTriggered(QAction *action)
{
    debug_tree_info(tr("action '%s' triggered").arg(action->text().toLocal8Bit().data()));
}

void MainWindow::setupToolBar()
{
    //for
    ToolBar *tb = new ToolBar(QString::fromLatin1("Main tool bar"), this);
    toolBars.append(tb);
    addToolBar(tb);
}

void MainWindow::setupMenuBar()
{
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    QAction *action = menu->addAction(tr("Save layout"));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

    action = menu->addAction(tr("Load layout"));
    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));

    menu->addSeparator();

    menu->addAction(tr("&Quit"), this, SLOT(close()));

//eidt...
    editMenu = menuBar()->addMenu(tr("&Edit"));
    action = editMenu->addAction(tr("Options..."));
    connect(action, SIGNAL(triggered()), this, SLOT(setGlobalOptions()));

//
    mainWindowMenu = menuBar()->addMenu(tr("Main window"));

    action = mainWindowMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowNestedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    QMenu *toolBarMenu = menuBar()->addMenu(tr("Tool bars"));
    for (int i = 0; i < toolBars.count(); ++i)
        toolBarMenu->addMenu(toolBars.at(i)->menu);

    dockWidgetMenu = menuBar()->addMenu(tr("&Dock Widgets"));

    menu = menuBar()->addMenu(tr("&Help"));
    action = menu->addAction(tr("Contents"));
    connect(action, SIGNAL(triggered()), this, SLOT(HelpContent()));

    action = menu->addAction(tr("About"));
    connect(action, SIGNAL(triggered()), this, SLOT(HelpAbout()));

}

void MainWindow::HelpAbout()
{
    QMessageBox::information(this, "About the program", message);
}

void MainWindow::HelpContent()
{
    QMessageBox::information(this, "Help contents", "Under development...");
}

void MainWindow::setDockOptions()
{
    DockOptions opts;
    QList<QAction*> actions = mainWindowMenu->actions();

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= AllowNestedDocks;
    if (actions.at(2)->isChecked())
        opts |= AllowTabbedDocks;
    if (actions.at(3)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(4)->isChecked())
        opts |= VerticalTabs;

    QMainWindow::setDockOptions(opts);
}

void MainWindow::setGlobalOptions()
{
    if (appConf) appConf->showUI(); /* QMessageBox::information(this, "setGlobalOptions", "enable");*/
}

void MainWindow::saveLayout()
{
    saveLayout(QString::fromLocal8Bit(Profile)); //saveLayout(QFileDialog::getSaveFileName(this, tr("Save layout")));
}

void MainWindow::saveLayout(const QString& FName)
{
    QFile file(FName);
    if (file.fileName().isEmpty()) return;
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(file.fileName())
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok) ok = file.write(geo_data) == geo_data.size();
    if (ok) ok = file.write(layout_data) == layout_data.size();

    if (ok)
    {
        statusBar()->showMessage(tr("Profile saved: %1").arg(FName));
    }
    else
    {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(file.fileName())
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::loadLayout()
{
    loadLayout(QString::fromLocal8Bit(Profile)); //loadLayout(QFileDialog::getOpenFileName(this, tr("Load layout")));
}
void MainWindow::loadLayout(const char *fileName)
{
    loadLayout(QString::fromLocal8Bit(fileName));
}
void MainWindow::loadLayout(const QString& FName)
{
    if (FName.isEmpty()) return;
    QFile file(FName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(FName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok) ok = restoreGeometry(geo_data);
    if (ok) ok = restoreState(layout_data);
    if (ok)
    {
        statusBar()->showMessage(tr("Profile loaded: %1").arg(FName));
    }
    else
    {
        QString msg = tr("Error reading %1").arg(FName);
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

QAction *addAction(QMenu *menu, const QString &text, QActionGroup *group, QSignalMapper *mapper, int id)
{
    bool first = group->actions().isEmpty();
    QAction *result = menu->addAction(text);
    result->setCheckable(true);
    result->setChecked(first);
    group->addAction(result);
    QObject::connect(result, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(result, id);
    return result;
}

void MainWindow::setupDockWidgets()
{
    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    mainLogTree = new Log(this);
    addDockWidget(Qt::BottomDockWidgetArea, mainLogTree);

    mainTreeWidget = new MainTreeWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mainTreeWidget);

    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setCorner(int)));

    QMenu *corner_menu = dockWidgetMenu->addMenu(tr("Top left corner"));
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 0);
    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 1);

    corner_menu = dockWidgetMenu->addMenu(tr("Top right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 2);
    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 3);

    corner_menu = dockWidgetMenu->addMenu(tr("Bottom left corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 4);
    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 5);

    corner_menu = dockWidgetMenu->addMenu(tr("Bottom right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 6);
    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 7);

    dockWidgetMenu->addSeparator();

    static struct SubMenu {
        const char * name;
        QMenu *menuPtr;
        MenuHandler menuHandler;
    } subMenu [] = {
        { "Tab into",                0, &MainWindow::tabInto    },
        { "Split horizontally into", 0, &MainWindow::splitIntoH },
        { "Split vertically into",   0, &MainWindow::splitIntoV },
    };
    const int menusCount = sizeof(subMenu) / sizeof(SubMenu);

    static const struct DockedWidget {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
        QDockWidget* widgetPtr;
    } dockedWidgets [] = {
        { "Log",      WFlags, Qt::BottomDockWidgetArea, (QDockWidget*)mainLogTree },
        { "MainTree", WFlags, Qt::LeftDockWidgetArea,   (QDockWidget*)mainTreeWidget },
    };
    const int dockedWidgetsCount = sizeof(dockedWidgets) / sizeof(DockedWidget);

    for (int i = 0; i < dockedWidgetsCount; ++i) {
        QMenu *widgetMenu = new QMenu(tr(dockedWidgets[i].name), this);
        widgetMenu->addAction(dockedWidgets[i].widgetPtr->toggleViewAction());
        QAction *action = widgetMenu->addAction(tr("Raise"));
        connect(action, SIGNAL(triggered()), dockedWidgets[i].widgetPtr, SLOT(raise()));

        QAction *vTitleBarAction = widgetMenu->addAction(tr("Vertical title bar"));
        vTitleBarAction->setCheckable(true);
        connect(vTitleBarAction, &QAction::triggered, [this, i](bool newValue)->void {this->changeVerticalTitleBarSlot(dockedWidgets[i].widgetPtr, newValue);});

        for (int m = 0; m < menusCount; ++m) {
            QMenu *mnu = new QMenu(this);
            subMenu[m].menuPtr = mnu;
            mnu->setTitle(tr(subMenu[m].name));
            connect(mnu, &QMenu::triggered, [this, m, i](QAction* action)->void {(this->*subMenu[m].menuHandler)(dockedWidgets[i].widgetPtr, action);});
            widgetMenu->addMenu(mnu);
        }

        for (int m = 0; m < dockedWidgetsCount; ++m) {
            if (strcmp(dockedWidgets[i].name,dockedWidgets[m].name)) {
                QString widgetName = tr(dockedWidgets[m].name);
                for (int a = 0; a < menusCount; ++a) {
                    subMenu[a].menuPtr->addAction(widgetName);
                }
            }
        }

        dockWidgetMenu->addMenu(widgetMenu);
    }
}

void MainWindow::splitIntoV(QDockWidget *widgt, QAction *action)
{
    splitInto(widgt, action, true);
}
void MainWindow::splitIntoH(QDockWidget *widgt, QAction *action)
{
    splitInto(widgt, action, false);
}

void MainWindow::openDefaultDEWidget(bool /*checked*/)
{
    Smo* smo = mainTreeWidget->getCurrentSMO();
    if (smo && smo->key()) { //smo->key defined only for a device's smo!!!
        mainTreeWidget->ActivateWidget(DEWIDGET_DEFAULT_NAME, smo, smo->isExpanded());
    }
}

void MainWindow::refreshDEWidget(bool /*checked*/)
{
    Smo* smo = mainTreeWidget->getCurrentSMO();
    DEWidget *currentDEWidget = mainTreeWidget->getCurrentDEWidget();
    if (currentDEWidget && smo) {
        currentDEWidget->DERaise_slot(smo, true /*smo->isExpanded()*/);
    }
}

void MainWindow::splitInto(QDockWidget* widgt, QAction *action, bool isVerticallyOrHorizontally)
{
    QList<QDockWidget*> dock_list = findChildren<QDockWidget*>();
    QDockWidget* target = 0;
    foreach (QDockWidget *dock, dock_list) {
        if (action->text() == dock->objectName()) {
            target = dock;
            break;
        }
    }
    if (target) splitDockWidget(target, widgt, isVerticallyOrHorizontally ? Qt::Vertical : Qt::Horizontal);
}

DEConfig *MainWindow::getAppConf() const
{
    return appConf;
}

DEWidget *MainWindow::getCurrentDEWidget() const
{
    return mainTreeWidget->getCurrentDEWidget();
}

void MainWindow::tabInto(QDockWidget* widgt, QAction *action)
{
    QList<QDockWidget*> dock_list = findChildren<QDockWidget*>();
    QDockWidget* target = 0;
    foreach (QDockWidget* dock, dock_list) {
        if (action->text() == dock->objectName()) {
            target = dock;
            break;
        }
    }
    if (target) tabifyDockWidget(target, widgt);
}

void MainWindow::changeVerticalTitleBarSlot(QDockWidget* widgt, bool newValue)
{
    widgt->setFeatures(newValue ? widgt->features() | QDockWidget::DockWidgetVerticalTitleBar : widgt->features() & ~QDockWidget::DockWidgetVerticalTitleBar);
}

void MainWindow::setCorner(int id)
{
    switch (id) {
        case 0:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
            break;
        case 1:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 2:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
            break;
        case 3:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
            break;
        case 4:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
            break;
        case 5:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 6:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
            break;
        case 7:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
            break;
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}


