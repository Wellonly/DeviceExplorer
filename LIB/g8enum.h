#ifndef G8ENUM_H
#define G8ENUM_H

#include "zvvlib.h"
//see include below: #include "GlobalStatusErrorCodes.h"

#include <QObject>

#include <QMetaObject>
#include <QMetaEnum>

class G8Enum : public QObject
{
    Q_OBJECT
    Q_ENUMS(G8) //Q_ENUMS() absolete!!! ...and Q_ENUM() not worked!!!
public:
    explicit G8Enum(QObject *parent = 0);
    ~G8Enum();

#undef GlobalStatusErrorCodes_h__
#include "GlobalStatusErrorCodes.h"

    static QMetaEnum G8MetaEnum;

signals:

public slots:
};

#endif // G8ENUM_H
