#ifndef SMOTREEITEM_H
#define SMOTREEITEM_H

#include <QPointer>
#include <QTreeWidgetItem>

template <class T>
class TreeItem : public QTreeWidgetItem
{
public:
    TreeItem(T* owner_qobject, const QString& text)
    : QTreeWidgetItem(QTreeWidgetItem::UserType)
    , m_owner(owner_qobject)
    {
        setText(0, text);
    }

    ~TreeItem() {
        if (m_owner) m_owner->setTreeItem(0); //clear pointer on itself!
    }

    T *owner() const {
        return m_owner;
    }

    T *takeChild_returnOwner(int index)
    {
        TreeItem<T>* item = (TreeItem<T>*)takeChild(index);
        if (item) {
            return item->owner();
        }
        return 0;
    }

    QList<T*> takeChildren_returnOwners()
    {
        QList<QTreeWidgetItem*> lst = takeChildren();
        QList<T*> retlist;
        if (lst.count() > 0) {
            foreach (QTreeWidgetItem* i, lst) {
                TreeItem<T>* it = (TreeItem<T>*)i;
                T* obj = it->owner();
                if (obj) {
                    retlist.append(obj);
                }
            }
        }
        return retlist;
    }

private:
    QPointer<T> m_owner;
};

#endif // SMOTREEITEM_H
