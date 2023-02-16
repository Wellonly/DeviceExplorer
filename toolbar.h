
#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QLabel)

class MainWindow;

class ToolBar : public QToolBar
{
    Q_OBJECT

    QAction *movableAction;

    QActionGroup *allowedAreasActions;
    QAction *allowLeftAction;
    QAction *allowRightAction;
    QAction *allowTopAction;
    QAction *allowBottomAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;

public:
    ToolBar(const QString &title, QWidget *parent);

    QMenu *menu;

protected:

private slots:

    void changeMovable(bool movable);

    void allowLeft(bool a);
    void allowRight(bool a);
    void allowTop(bool a);
    void allowBottom(bool a);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void updateMenu();

private:
    MainWindow* mainWindow;

    void allow(Qt::ToolBarArea area, bool allow);
    void place(Qt::ToolBarArea area, bool place);

};

#endif
