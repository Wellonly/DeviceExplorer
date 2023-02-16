#ifndef TREELIB_H
#define TREELIB_H

#include <QObject>
#include <QStringBuilder>
#include <QTreeWidget>

namespace TreeLib {

/// returns -1: NULL, 0: top level, so on...
template<typename T = QTreeWidgetItem>
int itemLevel(T *item) {
    int cou = 0;
    for(T* i = item; i; i = i->parent()) {
        cou++;
    }
    return cou-1;
}

template<typename T = QTreeWidgetItem>
int FindChildIndexOnText(T *parent, const QString& txt, int startIndex = 0 /*, bool isHiddenIncuded = true*/) {
    int cou = parent->childCount();
    for(int i = startIndex; i < cou; ++i) {
        if (parent->child(i)->text(0) == txt) {
            return i;
        }
    }
    return -1;
}

template<typename T = QTreeWidgetItem>
T* FindChildNodeOnText(T *parent, const QString& txt, int startIndex = 0 /*, bool isHiddenIncuded = true*/) {
    int cou = parent->childCount();
    for(int i = startIndex; i < cou; ++i) {
        T* item = (T*)parent->child(i);
        if (item->text(0) == txt) {
            return item;
        }
    }
    return (T*)0;
}

template<typename T = QTreeWidgetItem>
T* FindTopLevelNodeOnText(QTreeWidget *tree, const QString& txt, int startIndex = 0) {
    int cou = tree->topLevelItemCount();
    for(int i = startIndex; i < cou; ++i) {
        if (tree->topLevelItem(i)->text(0) == txt) {
            return (T*)tree->topLevelItem(i);
        }
    }
    return (T*)0;
}

template<typename T = QTreeWidgetItem>
int FindChildIndex(T* parent, auto func, int startIndex = 0) {
    int cou = parent->childCount();
    for(int i = startIndex; i < cou; ++i) {
        if (func((T*)parent->child(i))) {
            return i;
        }
    }
    return -1;
}

template<typename T = QTreeWidgetItem>
T* FindChildNode(T* parent, auto func, int startIndex = 0) {
    int cou = parent->childCount();
    for(int i = startIndex; i < cou; ++i) {
        T* item = (T*)parent->child(i);
        if (func(item)) {
            return item;
        }
    }
    return (T*)0;
}

template<typename T = QTreeWidgetItem>
void EvaluateChildren(T* parent, auto func, int startIndex = 0, int count = -1) {
    if (count < 0) count = parent->childCount();
    for(int i = startIndex; i < count; ++i) {
        func((T*)parent->child(i));
    }
}

template<typename T = QTreeWidgetItem>
int EvaluateTree(QTreeWidget* tree, auto func, QTreeWidgetItemIterator::IteratorFlags flags = QTreeWidgetItemIterator::All)
{
    int evaluate_count = 0;
    QTreeWidgetItemIterator it(tree, flags);
    while (*it) {
        T* item = (*it++);
        func(item);
        evaluate_count++;
    }
    return evaluate_count;
}

template<typename T = QTreeWidgetItem>
int NotHiddenChildCount(T* parent) {
    int childcou_not_hidded = 0;
    EvaluateChildren<T>(parent, [&childcou_not_hidded](QTreeWidgetItem*i)->void {if (!i->isHidden()) childcou_not_hidded++;});
    return childcou_not_hidded;
}

template<typename T = QTreeWidgetItem>
T* newItem(const QString& text, const QIcon& icon, int type=QTreeWidgetItem::UserType)
{
    T* node = new T(type);
    node->setText(0, text);
    node->setIcon(0, icon);
    return node;
}

template<typename T = QTreeWidgetItem>
T* newTwinItem(const QString& textInColumn0, const QString& textInColumn1, const QIcon& iconInColumn0=QIcon(), const QIcon& iconInColumn1=QIcon(), int type=QTreeWidgetItem::UserType)
{
    T* node = new T(type);
    node->setText(0, textInColumn0);
    node->setText(1, textInColumn1);
    node->setIcon(0, iconInColumn0);
    node->setIcon(1, iconInColumn1);
    return node;
}

template<typename T = QTreeWidgetItem>
QString ItemFullPath(T* item)
{
    QString ret = "";
    for (T* it = item; it != NULL; it = it->parent()) {
        if (it == item) {
            ret = it->text(0);
        }
        else {
            ret.prepend(it->text(0).append('/'));
        }
    }
    return ret;
}

template<typename T = QTreeWidgetItem>
void setItemVisibleSelectedChecked(T* node)
{
    node->setCheckState(0, Qt::Checked);
    node->setSelected(true);
    node->treeWidget()->setCurrentItem(node);
}

/// TDI tree routines...///////////////////////////////////////////////////////////////////
/// TDI tree routines...///////////////////////////////////////////////////////////////////
/// TDI tree routines...///////////////////////////////////////////////////////////////////
///
//old:TDITreeCalcNodes()
template<typename T>
void CalcQObjectNodes(QObject* object, auto func, bool isCalcParent = false, Qt::FindChildOptions options = Qt::FindChildrenRecursively) {
    if (isCalcParent) {
        T* parent = qobject_cast<T*>(object);
        if (parent) func(parent);
    }
    QList<T*> lst = object->findChildren<T*>(QString(), options);
    for (int i = 0; i < lst.count(); i++) { //NOTE: do not use foreach with QList!!!
        func(lst.at(i));
    }
//    for (int i = 0; i < lst.count(); i++) {
//        CalcQObjectNodes<T>(lst.at(i), func, false);
//    }
}

template<typename T>
T* FindQObject(QObject* object, auto func, bool isParentIncluded = false, Qt::FindChildOptions options = Qt::FindChildrenRecursively) {
    if (isParentIncluded) {
        T* parent = qobject_cast<T*>(object);
        if (parent && func(parent)) return parent;
    }
    QList<T*> lst = object->findChildren<T*>(QString(), Qt::FindDirectChildrenOnly);
    for (int i = 0; i < lst.count(); i++) { //NOTE: do not use foreach with QList!!!
        T* item = lst.at(i);
        if (func(item)) return item;
    }
    if (options == Qt::FindDirectChildrenOnly) return NULL;
    for (int i = 0; i < lst.count(); i++) {
        T* item = lst.at(i);
        T* ret = FindQObject<T>(item, func, false, options);
        if (ret) return ret;
    }
    return NULL;
}

//template<typename T> bool TDITreeCalcRootNode(QObject* object, void (*func)(T*)) {
//    if (T* root = qobject_cast<T*>(object)) {
//        func(root);
////find out why doesn't work:        TDITreeCalcChildNodes<T>(object, func);
//        return true;
//    }
//    return false;
//}
//template<typename T> void TDITreeCalcChildNodes(QObject* object, void (*func)(T*)) {
//    if (qobject_cast<T*>(object)) {
//        foreach (QObject* obj, object->children()) {
//            if (T* item = qobject_cast<T*>(obj)) {
//                func(item);
//            }
//        }
//        foreach (QObject* obj, object->children()) {
//            TDITreeCalcChildNodes<T>(obj, func);
//        }
//    }
//}

} //namespace TreeLib
//using namespace TreeLib;


#endif // TREELIB_H
