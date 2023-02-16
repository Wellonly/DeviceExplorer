
#include <QMetaEnum>
#include <QMetaObject>
#include <QDebug>

#include "zvvlib.h"
#include "SmoBusData.h"
#include "tdiportserial.h"

TDIPortSerial::TDIPortSerial(QObject *parent, const QSerialPortInfo &info)
: TDIPort(parent)
, m_info(info)
{
    port.setPortName(info.systemLocation());
    toBaudRate = port.baudRate();
    toDataBits = port.dataBits();
    toStopBits = port.stopBits();
    toParity = port.parity();
    toFlowControl = port.flowControl();
}

QString TDIPortSerial::StopBits()
{
    return StopBitsMap.key(toStopBits);
}

void TDIPortSerial::setStopBits(QString sbits)
{
    toStopBits = (QSerialPort::StopBits)StopBitsMap.value(sbits.toLatin1().constData());
}

QString TDIPortSerial::FlowControl()
{
    return FlowControlMap.key(toFlowControl);
}

void TDIPortSerial::setFlowControl(const QString& value)
{
    toFlowControl = (QSerialPort::FlowControl)FlowControlMap.value(value.toLatin1().constData());
}

QString TDIPortSerial::Parity()
{
    return ParityMap.key(toParity);
}

void TDIPortSerial::setParity(const QString& value)
{
    toParity = (QSerialPort::Parity)ParityMap.value(value.toLatin1().constData());
}

int TDIPortSerial::get_timeout()
{
    return timeout();
}

void TDIPortSerial::set_timeout(int timeout)
{
    setTimeout(timeout);
}

bool TDIPortSerial::Settings_engage()
{
    port.setBaudRate(toBaudRate);
    port.setDataBits(toDataBits);
    port.setStopBits(toStopBits);
    port.setFlowControl(toFlowControl);
    port.setParity(toParity);
    return true;
}

QString TDIPortSerial::message() const
{
    if (port.error() == QSerialPort::NoError) {
        return QString("Ok");
    }
    return TDIPortSerial::ErrorToString(port.error());
}

bool TDIPortSerial::open()
{
    return port.open(QIODevice::ReadWrite);
}

void TDIPortSerial::close()
{
    port.close();
}

bool TDIPortSerial::isOpen() const
{
    return port.isOpen();
}

QString TDIPortSerial::name() const
{
    return port.portName();
}

void TDIPortSerial::clear(Directions direction)
{
    port.clearError();
    static_assert(((int)QSerialPort::Direction::AllDirections) == ((int)TDIPort::Direction::AllDirections), "QSerialPort::Direction enum changed!!!");
    port.clear((QSerialPort::Direction)(int)direction);
}

QByteArray TDIPortSerial::read(int timeout)
{
    QReadLocker locker(&lock);
    QByteArray rxBuff(256, 0);

    if (!timeout) {
        timeout = get_timeout();
    }

    int frameHdrSize = offsetof(SmoBusFrame_t, Packet);
    int readRemainder = Read(this, &rxBuff, frameHdrSize, 0, timeout);
    if (readRemainder != 0)
    {
        return QByteArray();
    }
    const int frameTotalLength = rxBuff[(int)offsetof(SmoBusFrame_t, TotalLength)];
    readRemainder = Read(this, &rxBuff, frameTotalLength - frameHdrSize /*+1*/, frameHdrSize, timeout);
    if (readRemainder != 0)
    {
        return QByteArray();
    }
    rxBuff.resize(frameTotalLength);
    return rxBuff;
}

qint64 TDIPortSerial::write(const QByteArray &data)
{
    QWriteLocker locker(&lock);
    clear(TDIPort::Direction::Output);
    return port.write(data.constData(), data.length());
}

bool TDIPortSerial::waitForBytesToBeWritten(int msec)
{
    return port.waitForBytesWritten(msec);
}

bool TDIPortSerial::waitForBytesToBeRead(int msec)
{
    return port.waitForReadyRead(msec);
}

/// returns zero if all bytes has read else returns remains or -1 if an error...
int TDIPortSerial::Read(TDIPortSerial *sPort, QByteArray *rxBuff, int nCount, int startIndex, int timeout)
{ // retv zero if all bytes has read else returns remains...
//    qDebug() << "startIndex=" << startIndex << "Count=" << nCount;
    int remains = nCount;
    char* data = rxBuff->data() + startIndex;
    while (remains > 0 && sPort->port.waitForReadyRead(iif(timeout < 0, sPort->timeout(), timeout))) {
#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
        auto rdCount = sPort->port.read(data, remains);
#else
        auto rdCount = sPort->port.read(data, sPort->port.bytesAvailable());
#endif
//        qDebug() << "dataPtr=" << data-rxBuff->data() << "; remains=" << remains << "; rdCount=" <<rdCount;
        if (rdCount == remains) return 0;
        if (rdCount == -1) return -1;
        remains -= rdCount;
        data += rdCount;
    }
    return remains;
}

int TDIPortSerial::errorCode() const
{
    return port.error();
}

QString TDIPortSerial::errorString() const
{
    return ErrorToString((QSerialPort::SerialPortError)errorCode());
}

QString TDIPortSerial::ErrorToString(QSerialPort::SerialPortError err)
{
    return QString(EnumKey<QSerialPort>("SerialPortError", (int)err));
}

const QSerialPortInfo &TDIPortSerial::getInfo() const
{
    return m_info;
}

const EMap<> TDIPortSerial::BaudRateMap    = EMap<>(SerialBaudRate<>().Map);
const EMap<> TDIPortSerial::DataBitsMap    = EMap<>(SerialDataBits<>().Map);
const EMap<> TDIPortSerial::StopBitsMap    = EMap<>(SerialStopBits<>().Map);
const EMap<> TDIPortSerial::FlowControlMap = EMap<>(SerialFlowControl<>().Map);
const EMap<> TDIPortSerial::ParityMap      = EMap<>(SerialParity<>().Map);
const EMap<> TDIPortSerial::TimeoutMap     = EMap<>(SerialTimeout<>().Map);

