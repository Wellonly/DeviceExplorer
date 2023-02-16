#ifndef TDIPORTHID_H
#define TDIPORTHID_H

#include <QObject>
#include <QByteArray>
#include <QThread>

#include "tdiport.h"
#include "qhidapi.h"

class TDIPortHid : public TDIPort
{
    Q_OBJECT
public:
    TDIPortHid(QObject *parent = 0, const QHidDeviceInfo &info = QHidDeviceInfo());
    ~TDIPortHid();

    // TDIPort interface...
public:
    bool open() override;
    void close() override;

    QString name() const override;
    QString errorString() const override;
    QString message() const override;
    int errorCode() const override;
    bool isOpen() const override;
    QByteArray read(int timeout = -1) override;  //-1:blocking
    qint64 write(const QByteArray& data) override;

    static QHidApi* hidApi();

    const QHidDeviceInfo *deviceInfo() const;

    quint32 getDeviceId() const;

private:
    const QHidDeviceInfo m_info;
    quint32 deviceId = 0;

};
Q_DECLARE_METATYPE(TDIPortHid*)

#endif // TDIPORTHID_H
