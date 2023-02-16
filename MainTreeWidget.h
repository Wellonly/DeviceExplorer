#ifndef MAINTREEWIDGET_H
#define MAINTREEWIDGET_H

#include <QDockWidget>
#include <QTreeWidget>


class Smo;
template <class T> class TreeItem;
class DEWidget;
class MainWindow;

class MainTreeWidget : public QDockWidget
{
    Q_OBJECT
    QTreeWidget *tree;
public:
    explicit MainTreeWidget(QMainWindow *parent = 0);
    ~MainTreeWidget();
    QTreeWidget* getTree();
    DEWidget* getCurrentDEWidget() const;
    Smo *getCurrentSMO() const;

signals:

public slots:
    void nodeActivated();
    void nodeExpanded(QTreeWidgetItem * item);
    void nodeCollapsed(QTreeWidgetItem * item);

private:
    DEWidget *m_currentWidget = 0;
    Smo* m_currentItem = 0;

    void SwitchWidgets(DEWidget *newWidget);

    void nodeActivityHandler(TreeItem<Smo>* item, bool isToBeRefreshed);
    void setNodeSelected(QTreeWidgetItem * itemToSelect);

    DEWidget *LoadWidget(Smo *smo, const QString &widget);
    DEWidget *SelectAndActivateWidget(Smo *smo, bool isExpandedOrCollapsed);
    DEWidget *ActivateWidget(const QString &dewidget_name, Smo *smo, bool isExpandedOrCollapsed);

    void ShowWidget(DEWidget *newWidget, Smo *smo, bool isToBeRefreshed);

    friend class MainWindow;
};


#endif // MAINTREEWIDGET_H
