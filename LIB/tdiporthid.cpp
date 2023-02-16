#include <QApplication>

#include "zvvdebug.h"
#include "tdiporthid.h"

static QHidApi hidapi(qApp);

TDIPortHid::TDIPortHid(QObject *parent, const QHidDeviceInfo &info)
: TDIPort(parent)
, m_info(info)
{

}

TDIPortHid::~TDIPortHid()
{
    debug_func(name() << "distroyed");
}

bool TDIPortHid::open()
{
    deviceId = hidapi.open(m_info.vendorId, m_info.productId, m_info.serialNumber);

    return !!deviceId;
}

void TDIPortHid::close()
{
    hidapi.close(deviceId);
}

QString TDIPortHid::name() const
{
    return m_info.productString;
}

QString TDIPortHid::errorString() const
{
    return hidapi.error(deviceId);
}

QString TDIPortHid::message() const
{
    QString mess = hidapi.error(deviceId);
    if (mess.isEmpty()) {
        return QString("Ok");
    }
    return mess;
}

int TDIPortHid::errorCode() const
{
    return hidapi.error(deviceId).length();  //FIXME: where is error code?
}

bool TDIPortHid::isOpen() const
{
    return !!deviceId;
}

QByteArray TDIPortHid::read(int timeout)
{
    QReadLocker locker(&lock);
    return hidapi.read(deviceId, timeout);
}

qint64 TDIPortHid::write(const QByteArray &data)
{
    QWriteLocker locker(&lock);
    return hidapi.write(deviceId, data);
}

QHidApi *TDIPortHid::hidApi()
{
    return &hidapi;
}

const QHidDeviceInfo *TDIPortHid::deviceInfo() const
{
    return &m_info;
}

quint32 TDIPortHid::getDeviceId() const
{
    return deviceId;
}
