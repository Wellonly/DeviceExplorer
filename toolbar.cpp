
#include "app_config.h"
#include "toolbar.h"

#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QSpinBox>
#include <QLabel>
#include <QToolTip>

#include <stdlib.h>
#include "MainWindow.h"

ToolBar::ToolBar(const QString &title, QWidget *parent)
: QToolBar(parent)
, mainWindow((MainWindow*)parent)
{
    setWindowTitle(title);
    setObjectName(title);

    setIconSize(QSize(32, 32));

    QAction* action = addAction(zvvlib::getIcon("info"), "info action");
    action = addAction(zvvlib::getIcon("blocks"), "default");
    connect(action, SIGNAL(triggered(bool)), mainWindow, SLOT(openDefaultDEWidget(bool)));
    action = addAction(zvvlib::getIcon("refresh"), "refresh");
    connect(action, SIGNAL(triggered(bool)), mainWindow, SLOT(refreshDEWidget(bool)));

    movableAction = new QAction(tr("Movable"), this);
    movableAction->setCheckable(true);
    connect(movableAction, SIGNAL(triggered(bool)), SLOT(changeMovable(bool)));

    allowedAreasActions = new QActionGroup(this);
    allowedAreasActions->setExclusive(false);

    allowLeftAction = new QAction(tr("Allow on Left"), this);
    allowLeftAction->setCheckable(true);
    connect(allowLeftAction, SIGNAL(triggered(bool)), SLOT(allowLeft(bool)));

    allowRightAction = new QAction(tr("Allow on Right"), this);
    allowRightAction->setCheckable(true);
    connect(allowRightAction, SIGNAL(triggered(bool)), SLOT(allowRight(bool)));

    allowTopAction = new QAction(tr("Allow on Top"), this);
    allowTopAction->setCheckable(true);
    connect(allowTopAction, SIGNAL(triggered(bool)), SLOT(allowTop(bool)));

    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
    allowBottomAction->setCheckable(true);
    connect(allowBottomAction, SIGNAL(triggered(bool)), SLOT(allowBottom(bool)));

    allowedAreasActions->addAction(allowLeftAction);
    allowedAreasActions->addAction(allowRightAction);
    allowedAreasActions->addAction(allowTopAction);
    allowedAreasActions->addAction(allowBottomAction);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    connect(movableAction, SIGNAL(triggered(bool)), areaActions, SLOT(setEnabled(bool)));

    connect(movableAction, SIGNAL(triggered(bool)), allowedAreasActions, SLOT(setEnabled(bool)));

    menu = new QMenu(title, this);
    menu->addAction(toggleViewAction());
    menu->addSeparator();
    menu->addAction(movableAction);
    menu->addSeparator();
    menu->addActions(allowedAreasActions->actions());
    menu->addSeparator();
    menu->addActions(areaActions->actions());

    connect(menu, SIGNAL(aboutToShow()), this, SLOT(updateMenu()));

}

void ToolBar::updateMenu()
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    Q_ASSERT(mainWindow != 0);

    const Qt::ToolBarArea area = mainWindow->toolBarArea(this);
    const Qt::ToolBarAreas areas = allowedAreas();

    movableAction->setChecked(isMovable());

    allowLeftAction->setChecked(isAreaAllowed(Qt::LeftToolBarArea));
    allowRightAction->setChecked(isAreaAllowed(Qt::RightToolBarArea));
    allowTopAction->setChecked(isAreaAllowed(Qt::TopToolBarArea));
    allowBottomAction->setChecked(isAreaAllowed(Qt::BottomToolBarArea));

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftToolBarArea);
        allowRightAction->setEnabled(area != Qt::RightToolBarArea);
        allowTopAction->setEnabled(area != Qt::TopToolBarArea);
        allowBottomAction->setEnabled(area != Qt::BottomToolBarArea);
    }

    leftAction->setChecked(area == Qt::LeftToolBarArea);
    rightAction->setChecked(area == Qt::RightToolBarArea);
    topAction->setChecked(area == Qt::TopToolBarArea);
    bottomAction->setChecked(area == Qt::BottomToolBarArea);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftToolBarArea);
        rightAction->setEnabled(areas & Qt::RightToolBarArea);
        topAction->setEnabled(areas & Qt::TopToolBarArea);
        bottomAction->setEnabled(areas & Qt::BottomToolBarArea);
    }
}


void ToolBar::allow(Qt::ToolBarArea area, bool a)
{
    Qt::ToolBarAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftToolBarArea);
        rightAction->setEnabled(areas & Qt::RightToolBarArea);
        topAction->setEnabled(areas & Qt::TopToolBarArea);
        bottomAction->setEnabled(areas & Qt::BottomToolBarArea);
    }
}

void ToolBar::place(Qt::ToolBarArea area, bool p)
{
    if (!p)
        return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    Q_ASSERT(mainWindow != 0);

    mainWindow->addToolBar(area, this);

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftToolBarArea);
        allowRightAction->setEnabled(area != Qt::RightToolBarArea);
        allowTopAction->setEnabled(area != Qt::TopToolBarArea);
        allowBottomAction->setEnabled(area != Qt::BottomToolBarArea);
    }
}

void ToolBar::changeMovable(bool movable)
{ setMovable(movable); }

void ToolBar::allowLeft(bool a)
{ allow(Qt::LeftToolBarArea, a); }

void ToolBar::allowRight(bool a)
{ allow(Qt::RightToolBarArea, a); }

void ToolBar::allowTop(bool a)
{ allow(Qt::TopToolBarArea, a); }

void ToolBar::allowBottom(bool a)
{ allow(Qt::BottomToolBarArea, a); }

void ToolBar::placeLeft(bool p)
{ place(Qt::LeftToolBarArea, p); }

void ToolBar::placeRight(bool p)
{ place(Qt::RightToolBarArea, p); }

void ToolBar::placeTop(bool p)
{ place(Qt::TopToolBarArea, p); }

void ToolBar::placeBottom(bool p)
{ place(Qt::BottomToolBarArea, p); }

