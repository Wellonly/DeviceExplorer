#ifndef TDIPORT_H
#define TDIPORT_H

#include <QObject>
#include <QByteArray>
#include <stdbool.h>
#include <QReadWriteLock>
//#include <QTimer>
#include <QPointer>

#include "deviceinfo.h"

//TODO: move SCAN_PERIOD_mS in app config!!!
#ifndef SCAN_PERIOD_mS
#define SCAN_PERIOD_mS 111
#endif

class Smo;
class DeviceInfo;

class TDIPort : /*public QObject*/ public QTimer
{
    Q_OBJECT
public:
    explicit TDIPort(QObject *parent, int timeout_mS = 100, int scan_time_mS = SCAN_PERIOD_mS);
    ~TDIPort();

    enum Direction  {
        Input = 1,
        Output = 2,
        AllDirections = Input | Output
    };
    Q_FLAG(Direction)
    Q_DECLARE_FLAGS(Directions, Direction)

virtual bool open();
virtual void close();
virtual QString name() const;
virtual QString errorString() const;
virtual QString message() const;
virtual int errorCode() const;
virtual bool isOpen() const;
virtual void clear(TDIPort::Directions direction = AllDirections);
virtual QByteArray read(int timeout_mS = -1); //-1:blocking
virtual qint64 write(const QByteArray &data);
virtual QByteArray transaction(const QByteArray &wdata, bool* isok);
virtual bool waitForBytesToBeWritten(int msec);
virtual bool waitForBytesToBeRead(int msec);
virtual int txCounter();
virtual bool checkRxFrameCounter(int rxCounter);
virtual int timeout() const;
virtual void setTimeout(int timeout_mS);

    void portScan(/*DeviceInfo *deviceInfo, */bool isEnabled);
    bool dataPort(Smo* syncSMO, const QByteArray& data);
    DeviceInfo* rootDeviceInfo() const;
    Smo* readFrame();
    Smo *asyncSmo() const;
    Smo *syncSmo() const;
    Smo* identifySMO(uint deviceID, smokey key);

signals:
//    void ItemDataNotPorted(QObject* item, int error_code, const QString& message);
//    void InputActivityDetected(const QByteArray& data);

public slots:
    void timerEvent(QTimerEvent *e);
//    void ItemDataPort(QObject* item, const QByteArray& data);

protected:
    QReadWriteLock lock;

private: /* private func... */
    static inline bool SMOTargetMemory_update(Smo* smo, const char* data, const int dataLength, const int resultLength);
    static bool SMOTargetRecord_update(Smo* smo, const char* data, const int dataLength, const int resultLength);

private: /* private vars... */
    QPointer<DeviceInfo> m_rootDevice;
    QPointer<Smo>        m_syncSmo;
    int m_timeout_mS;
    int m_scan_period_mS;
    int _TxCounter;
    int _RxCounter;
    Smo* m_asyncSmo = 0;
    G8 lastError = msg_Success;

    friend class DeviceInfo;
};

#endif // TDIPORT_H
