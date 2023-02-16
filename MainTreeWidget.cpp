
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QVariant>
#include <QHostInfo>

#include "treeitem.h"
#include "smo.h"
#include "app_config.h"
#include "MainTreeWidget.h"
#include "MainWindow.h"
#include "deviceinfo.h"
#include "deplugin.h"
#include "LocalHostWidget.h"
#include "fdefault.h"
#include "fdevice.h"
#include "zvvdebug.h"

#include "fcomport.h"
#include "fhidport.h"
#include "deconfig.h"
#include "fdockqml.h"

MainTreeWidget::MainTreeWidget(QMainWindow *parent)
: QDockWidget(parent)
{
    setObjectName("MainTree");
    setWindowTitle("Main tree");

    setMinimumSize(50,50);


    tree = new QTreeWidget(this);
    tree->setHeaderHidden(true);

    setWidget(tree);

    //tree->setColumnCount(1);

    connect(tree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(nodeExpanded(QTreeWidgetItem*)));
    connect(tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),this, SLOT(nodeCollapsed(QTreeWidgetItem*)));
    connect(tree, SIGNAL(itemSelectionChanged()),         this, SLOT(nodeActivated()));

    DEWidget *localHost = new LocalHostWidget(this, new Smo(QHostInfo::localHostName(), this));
    parent->addDockWidget(Qt::RightDockWidgetArea, localHost);
    SwitchWidgets(localHost);
}

MainTreeWidget::~MainTreeWidget()
{

}

QTreeWidget *MainTreeWidget::getTree()
{
    return tree;
}

DEWidget *MainTreeWidget::getCurrentDEWidget() const
{
    return m_currentWidget;
}

void MainTreeWidget::nodeActivated()
{
    QList<QTreeWidgetItem*> selected = tree->selectedItems();
    if (selected.length() == 1) {
        nodeActivityHandler((TreeItem<Smo>*)selected[0], false);
    }
}

void MainTreeWidget::nodeExpanded(QTreeWidgetItem *item)
{
    if (item->isSelected()) {
        nodeActivityHandler((TreeItem<Smo>*)item, true); //Expanded
    }
    else {
        setNodeSelected(item);
    }
}

void MainTreeWidget::nodeCollapsed(QTreeWidgetItem *item)
{
    if (item->isSelected()) {
        nodeActivityHandler((TreeItem<Smo>*)item, true); //Collapsed
    }
    else {
        setNodeSelected(item);
    }
}

Smo *MainTreeWidget::getCurrentSMO() const
{
    return m_currentItem;
}

void MainTreeWidget::setNodeSelected(QTreeWidgetItem *itemToSelect)
{
    auto selected = tree->selectedItems();
    foreach (QTreeWidgetItem *item, selected) {
        item->setSelected(false);
    }
    itemToSelect->setSelected(true);
}

DEWidget *MainTreeWidget::LoadWidget(Smo *smo, const QString& widget)
{
    MainWindow* mainw = (MainWindow*)parentWidget();
    DEWidget *newWidget = 0;
    if (widget == DEWIDGET_DEFAULT_NAME) {
        newWidget = new fDefault(mainw, smo);
    }
    else if (widget == "DE.fDevice") {
        newWidget = new fDevice(mainw, smo);
    }
    else if (widget == "DE.fCOMPort") {
        newWidget = new fCOMPort(mainw, smo);
    }
    else if (widget == "DE.fHIDPort") {
        newWidget = new fHIDPort(mainw, smo);
    }
    else
    { //try to load an external qml...
        DEConfig* cfg = mainw->getAppConf(); //TODO: to static
        QString pluginpath(cfg->getAppPluginsPath());
        if (!pluginpath.isEmpty()) {
            QString main_qml(pluginpath);
            //Log::addWarning("external widgets path: " + pluginpath);
            if (QFile::exists(main_qml.append('/').append(widget).append("/main.qml")))
            { //try to load it...
                newWidget = new fDockedQml(mainw, smo, widget, main_qml);
            }
            else {
                QString main_ui_qml(pluginpath);
                if (QFile::exists(main_ui_qml.append('/').append(widget).append("/main.ui.qml")))
                { //try to load it...
                    newWidget = new fDockedQml(mainw, smo, widget, main_ui_qml);
                }
            }
        }
    }
    if (!newWidget) {
        debug_tree_info(widget + ": widget doesn't found!!!");
        return 0;
    }
    smo->setExpandedWidget(newWidget);
    smo->setCollapsedWidget(newWidget);
    SwitchWidgets(newWidget);
//    if (smo->isExpanded()) {
//        smo->expandedWidget = newWidget;
//    }
//    else {
//        smo->collapsedWidget = newWidget;
//    }
    mainw->addDockWidget(Qt::RightDockWidgetArea, newWidget);
    newWidget->show();
    return newWidget;
}

DEWidget *MainTreeWidget::SelectAndActivateWidget(Smo *smo, bool isExpandedOrCollapsed)
{
    QString iwidget;
    if (smo->plugin()) {
        iwidget = smo->plugin()->AppLink;
    }
    else if ((smo->folderType() == SmoFolderType::LinkToDevice && !smo->isExpanded()))
    {
        iwidget = "DE.fDevice";
    }
    else
    {
        iwidget = iif(!smo->deviceInfo() || smo->deviceInfo()->deviceItem() == nullptr, "DE", Smo::Path(smo));
    }
#ifdef QT_DEBUG
    qDebug() << "Page: " << iwidget;
#endif
    return ActivateWidget(iwidget, smo, isExpandedOrCollapsed);
}

DEWidget*
MainTreeWidget::ActivateWidget(const QString &dewidget_name, Smo *smo, bool isExpandedOrCollapsed)
{
    DEWidget *targetWidget;
    QString dename(dewidget_name);
    do
    {
        targetWidget = parentWidget()->findChild<DEWidget*>(dename, Qt::FindDirectChildrenOnly); //FindWidget_in_loadedList(cWidget);
        if (targetWidget == NULL /* || !qobject_cast<DEWidget*>(targetWidget)*/) {
            targetWidget = LoadWidget(smo, dename);
            if (targetWidget) return targetWidget; //return: skip ShowWidget() because it has already shown!!!
        }
        if (targetWidget == NULL) {
            dename = DEWIDGET_DEFAULT_NAME;
        }
    } while (targetWidget == NULL);

    ShowWidget(targetWidget, smo, isExpandedOrCollapsed);
    return targetWidget;
}

void MainTreeWidget::SwitchWidgets(DEWidget *newWidget)
{
    if (m_currentWidget != newWidget) {
        if (m_currentWidget) {
            m_currentWidget->DEHide_slot();
        }
        m_currentWidget = newWidget;
    }
}

void MainTreeWidget::ShowWidget(DEWidget *widget, Smo *smo, bool isToBeRefreshed)
{
    SwitchWidgets(widget);
    widget->DERaise_slot(smo, isToBeRefreshed);
}

void MainTreeWidget::nodeActivityHandler(TreeItem<Smo> *item, bool isToBeRefreshed)
{ //DEItemState itemState
    m_currentItem = qobject_cast<Smo*>(item->owner());
    if (m_currentItem) {
        DEWidget *targetWidget;
        if (item->isExpanded()) {
            targetWidget = m_currentItem->expandedWidget();
        }
        else {
            targetWidget = m_currentItem->collapsedWidget();
        }

        if (targetWidget) {
            ShowWidget(targetWidget, m_currentItem, isToBeRefreshed);
        }
        else {
            targetWidget = SelectAndActivateWidget(m_currentItem, isToBeRefreshed);
        }
    }
    else {
        debug_func("no Smo in TreeItem!!!");
    }
}

