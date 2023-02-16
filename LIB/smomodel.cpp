
#include "zvvdebug.h"
#include "smomodel.h"

DeModel::DeModel(QObject *parent, Smo *folderItem)
: QAbstractListModel(parent)
{
    setFoldSmo(folderItem);
}

int DeModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_childCount;
}

QVariant DeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_childCount)
        return QVariant();

    Smo* smo = m_foldSmo->child(index.row());
    if (role == SmoRole) {
        return QVariant::fromValue(smo);
    }
    return QVariant();
}

QHash<int, QByteArray> DeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SmoRole] = "smo";
//    roles[SizeRole] = "size";
    return roles;
}

Smo *DeModel::folderItem() const
{
    return m_foldSmo;
}

void DeModel::refreshModel()
{
    beginResetModel();
    int cou = iif(m_foldSmo.isNull(), 0, m_foldSmo->children().count());
    if (cou) {
        Smo* smo = qobject_cast<Smo*>(m_foldSmo->children()[cou -1]);
        if (!smo || (smo->flags() & SmoFlags::FlagIntegrityError))
        { //exclude last Smo...
            --cou;
        }
    }
    m_childCount = cou;
    endResetModel();
    debug_func("model items count:" << cou << m_foldSmo->flagsToString());
}

void DeModel::setFoldSmo(Smo *foldSmo)
{
    m_foldSmo = foldSmo;
    refreshModel();
}

