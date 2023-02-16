
//#include "smo.h"

#include "zvvdebug.h"
#include "smoinfo.h"

const Qt::ConnectionType SMO::Connection_unique_auto = (Qt::ConnectionType)(Qt::AutoConnection | Qt::UniqueConnection);
const Qt::ConnectionType SMO::Connection_unique_queued = (Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection);

QVariant SMO::tstFunc(QObject *smoFolder)
{
    debug_func(" called with:" << smoFolder);
    return QVariant::fromValue(smoFolder);
}
