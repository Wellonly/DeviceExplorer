#ifndef SMOMODEL_H
#define SMOMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QPointer>

#include "smo.h"

class Smo;

class DeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DeModel(QObject *parent = 0, Smo* folderItem = 0);
    enum SmoRoles {
        SmoRole = Qt::UserRole + 1,
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE Smo *folderItem() const;
    void setFoldSmo(Smo *folderItem);

public slots:
    void refreshModel();

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QPointer<Smo> m_foldSmo = 0;
    int m_childCount = 0;

};

#endif // SMOMODEL_H
