#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdbool.h>
#include <QByteArray>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMap>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "tdiport.h"

template <class Key = const char *, class Value = int>
struct SerialBaudRate {
    const std::map<Key, Value> Map = {
        {"300",   300},
        {"600",   600},
        {"1200",  1200},
        {"2400",  2400},
        {"4800",  4800},
        {"9600",  9600},
        {"19200", 19200},
        {"38400", 38400},
        {"57600", 57600},
        {"115200",115200},
        {"230400",230400},
        {"460800",460800},
        {"921600",921600},
    };
};

template <class Key = const char *, class Value = int>
struct SerialDataBits {
    const std::map<Key, Value> Map = {
        {"5", 5},
        {"6", 6},
        {"7", 7},
        {"8", 8},
    };
};

template <class Key = const char *, class Value = int>
struct SerialStopBits {
    const std::map<Key, Value> Map = {
        {"1",  QSerialPort::StopBits::OneStop},
        {"2",  QSerialPort::StopBits::TwoStop},
        {"1.5",QSerialPort::StopBits::OneAndHalfStop},
    };
};

template <class Key = const char *, class Value = int>
struct SerialParity {
    const std::map<Key, Value> Map = {
        {"None", QSerialPort::Parity::NoParity},
        {"Even", QSerialPort::Parity::EvenParity},
        {"Odd",  QSerialPort::Parity::OddParity},
        {"Space",QSerialPort::Parity::SpaceParity},
        {"Mark", QSerialPort::Parity::MarkParity},
    };
};

template <class Key = const char *, class Value = int>
struct SerialFlowControl {
    const std::map<Key, Value> Map = {
        {"None",    QSerialPort::FlowControl::NoFlowControl},
        {"RTS/CTS", QSerialPort::FlowControl::HardwareControl},
        {"XON/XOFF",QSerialPort::FlowControl::SoftwareControl},
    };
};

template <class Key = const char *, class Value = int>
struct SerialTimeout {
    const std::map<Key, Value> Map = {
        {"100", 100},
        {"200", 200},
        {"300", 300},
        {"500", 500},
        {"1000",1000},
        {"2000",2000},
    };
};

template<class Key = const char *, class Value = int>
class EMap : public QMap<Key,Value> {
public:
    EMap(const std::map<Key, Value> M = {{"",0}}) : QMap<Key,Value>(M) {}
};

//cs: PortData
class TDIPortSerial : public TDIPort
{
    Q_OBJECT
    Q_PROPERTY(qint32 BaudRate READ BaudRate WRITE setBaudRate)
    Q_PROPERTY(int DataBits READ DataBits WRITE setDataBits)
    Q_PROPERTY(QString StopBits READ StopBits WRITE setStopBits)
    Q_PROPERTY(QString FlowControl READ FlowControl WRITE setFlowControl)
    Q_PROPERTY(QString Parity READ Parity WRITE setParity)
    Q_PROPERTY(int Timeout READ get_timeout WRITE set_timeout)

public:
    TDIPortSerial(QObject* parent, const QSerialPortInfo &info = QSerialPortInfo());
    virtual ~TDIPortSerial() {}

    qint32 BaudRate() {return toBaudRate;}
    void setBaudRate(qint32 baud) {toBaudRate = baud;}

    int DataBits() {return (int)toDataBits;}
    void setDataBits(int dbits) {toDataBits = (QSerialPort::DataBits)dbits;}

    QString StopBits();
    void setStopBits(QString sbits);

    QString FlowControl();
    void setFlowControl(const QString &value);

    QString Parity();
    void setParity(const QString &value);

    int get_timeout();
    void set_timeout(int timeout);

    bool Settings_engage();

    QString message() const override;

    bool open() override;
    void close() override;

    bool isOpen() const override;

    QString name() const override;

    void clear(Directions direction = AllDirections) override;

    QByteArray read(int timeout = -1) override; //-1:blocking for sPort->timeout()
    qint64 write(const QByteArray& data) override;

    bool waitForBytesToBeWritten(int msec) override;
    bool waitForBytesToBeRead(int msec) override;

    int errorCode() const override;
    QString errorString() const override;

    static const EMap<> BaudRateMap;
    static const EMap<> DataBitsMap;
    static const EMap<> StopBitsMap;
    static const EMap<> FlowControlMap;
    static const EMap<> ParityMap;
    static const EMap<> TimeoutMap;

    static QString ErrorToString(QSerialPort::SerialPortError err);

    //next: hardware port settings...
    const QSerialPortInfo& getInfo() const;

private:
    qint32 toBaudRate = 9600;
    QSerialPort::DataBits toDataBits = QSerialPort::Data8;
    QSerialPort::StopBits toStopBits = QSerialPort::OneStop;
    QSerialPort::Parity toParity = QSerialPort::NoParity;
    QSerialPort::FlowControl toFlowControl = QSerialPort::NoFlowControl;
    const QSerialPortInfo m_info;

    QSerialPort port;
    static int Read(TDIPortSerial *sPort, QByteArray *rxBuff, int nCount, int startIndex, int timeout = -1); //-1:blocking for sPort->timeout()
};

#endif // SERIALPORT_H

