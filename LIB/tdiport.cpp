
#include <QThread>

#include "zvvdebug.h"
#include "zvvenum.h"
#include "CRC.h"
#include "Offseters.h"

#include "tdiport.h"


#ifndef PORT_CLOSED_SCAN_MULT
#define PORT_CLOSED_SCAN_MULT 2
#endif


TDIPort::TDIPort(QObject *parent, int timeout_mS, int scan_time_mS)
: QTimer(parent)
//,m_portItem(parent)
,m_timeout_mS(timeout_mS)
,m_scan_period_mS(scan_time_mS)
,_TxCounter(0)
,_RxCounter(-1)
{
    setSingleShot(true);
    start(m_scan_period_mS);
}

TDIPort::~TDIPort()
{
    stop();
    debug_func(QThread::currentThread() << QThread::currentThreadId());
}

/// \brief mem_update update dest memory with source data padded by zeroes!!!
/// \param dest   ptr
/// \param source ptr
/// \param sourceLength
/// \param destLength
/// \return true if mem changes detected!!!
static bool mem_update(char* dest, const char* source, const int sourceLength, const int destLength) {
    bool retv = false;
    for (int i = 0; i < destLength; i++) {
        char schar = iif(i < sourceLength, *source++, 0);
        retv = retv || *dest != schar;
        *dest++ = schar;
    }
    return retv;
}


bool TDIPort::open()
{
    return false;
}

void TDIPort::close()
{
}

QString TDIPort::name() const
{
    return QString();
}

QString TDIPort::errorString() const
{
    return QString();
}

QString TDIPort::message() const
{
    return QString();
}

int TDIPort::errorCode() const
{
    return 0;
}

bool TDIPort::isOpen() const
{
    return false;
}

void TDIPort::clear(Directions direction)
{
    (void)direction;
}

/// \brief TDIPort::read
/// \param timeout 0: no wait for timeout!!!
QByteArray TDIPort::read(int timeout_mS)
{
    (void)timeout_mS;
    return QByteArray();
}

qint64 TDIPort::write(const QByteArray& data)
{
    (void)data;
    return 0;
}

QByteArray TDIPort::transaction(const QByteArray &wdata, bool *isok)
{
    (void)wdata;
    (void)isok;
    return QByteArray();
}

bool TDIPort::waitForBytesToBeWritten(int msec)
{
    (void)msec;
    return true;
}

bool TDIPort::waitForBytesToBeRead(int msec)
{
    (void)msec;
    return true;
}

int TDIPort::txCounter()
{
    return _TxCounter++; //++_TxCounter or _TxCounter++ ???
}

bool TDIPort::checkRxFrameCounter(int rxCounter)
{
    if (rxCounter == _RxCounter)
    {
        return false;
    }
    _RxCounter = rxCounter;
    return true;
}

int TDIPort::timeout() const
{
    return m_timeout_mS;
}

void TDIPort::setTimeout(int timeout_mS)
{
    m_timeout_mS = timeout_mS;
}

void TDIPort::portScan(bool isEnabled)
{
    if (isEnabled) {
        start(m_scan_period_mS);
    }
    else {
        stop();
    }
}

/// \brief TDIPort::dataPort out data to port!!!
/// \param syncSMO
/// \param data
/// \return true if write count equals data length
bool TDIPort::dataPort(Smo* syncSMO, const QByteArray &data)
{
    debug_tree_info(QString("Send %1 (length: %2) : %3").arg(name()).arg(data.length(), 3, 10, QChar('0')).arg(Log::HexToString(data)));
    m_syncSmo = syncSMO;
    m_asyncSmo = 0;
    return write(data) == data.length();
}

DeviceInfo *TDIPort::rootDeviceInfo() const
{
    return m_rootDevice;
}

Smo *TDIPort::readFrame()
{
    QByteArray data = read(0); //0: no wait for timeout!!!
    if (data.length()) {
        debug_tree_info(QString("Read %1 (length: %2) : %3").arg(name()).arg(data.length(),3,10,QChar('0')).arg(Log::HexToString(data)));

        SmoBusFrame_t *frame = (SmoBusFrame_t*)data.data();
        //debug.Assert((smo->Key & 0x0001) != 0);
        if (frame->FrameType != FrameType_SmoBus)
        {
            debug_func("Frame type unsupported!");
            lastError = err_Unsupported;
            return 0;
        }
        if (frame->DestAddress != SmoBus_MasterAddress)
        {
            debug_func("Not in our address!");
            lastError = msg_NotInOurAddress;
            return 0;
        }
        if (frame->SourceAddress == SmoBus_MasterAddress) {
            debug_func("Source address mismatch!");
            lastError = err_Unsupported;
            return 0;
        }
        if (!checkRxFrameCounter(frame->Packet.packID))
        {
            debug_func("Dublicate frame catched");
            lastError = wrn_Doubled;
            return 0;
        }

        const int frameTotalLength = frame->TotalLength;
        if (data.length() != frameTotalLength) {
            debug_func("Data length error");
            lastError = err_DataLength;
            return 0;
        }
        ::DeOffsetData(data.data(), offsetof(SmoBusFrame_t, Packet.packID), frameTotalLength - offsetof(SmoBusFrame_t, Packet.packID));

        if (!CRC16_verify(CRC16_INIT, (char*)&frame->FrameType, frameTotalLength))
        {
            lastError = err_WrongCRC;
            return 0;
        }

        uint deviceID = frame->SourceAddress;
        smoKey frameKey = frame->Packet.packKey.Key;
        Smo* smo = m_syncSmo;
        if (!(smo && smo->deviceInfo()->netAddress() == deviceID && smo->key() == (frameKey & smoKeyMask)))
        { //async packet...
            if (!(smo = identifySMO(deviceID, frameKey & smoKeyMask))) {
                lastError = err_WrongAddress;
                return 0;
            }
        }
        //Smo has identifyed!!!...

        //if we are here then the Smo IO stage & lower levels passed OK, start application level ...
        const int CargoTotalLength = frame->Packet.DataLength;
        int CargoRealLength = frameTotalLength - offsetof(SmoBusFrame_t, Packet.Data) - SmoBusCRCLength;
        if (CargoRealLength < 0) CargoRealLength = 0; //NOTE: negative values may be present(because Packet header may also be truncated)!!!

        const int old_flags = smo->m_flags;
        bool isDataToBeUpdated = false;
        //do actions on PackType...
        bool isCallSuccessful = false;
        switch ((enum SmoBusKeyFlags)(frameKey & SmoBus_KeyFlags_mask)) {
        case SmoBusKey_retData:
            if (SMOTargetMemory_update(smo, frame->Packet.Data, CargoRealLength, CargoTotalLength) || smo->m_mainStatusCode != G8::msg_Success)
            { //update target value ...
                smo->m_mainStatusCode = smo->updateTargetValue(); //G8::msg_Success;
                isDataToBeUpdated = true;
            }
            isCallSuccessful = true;
            smo->flagsSetClear(SmoFlags::FlagIOData, SmoFlags::FlagIOError | SmoFlags::FlagIOStatus);
            break;
        case SmoBusKey_retRecord:
        {
            bool isRecordToBeUpdated = !(old_flags & SmoFlags::FlagIntegrityError);
//            smo->m_flags = 0; //SMOStatus::_AllClear;
            isDataToBeUpdated = mem_update((char*)&smo->m_record, frame->Packet.Data, CargoRealLength, CargoTotalLength);
            isCallSuccessful = Smo::RecordParser(smo, CargoTotalLength);
            if (isCallSuccessful) {
                Smo::DecodeRecordName(smo);
                if (isDataToBeUpdated) {
                    if ((isDataToBeUpdated = smo->m_targetValue.isValid())) {
                        smo->m_targetValue.clear();
                    }
                    isRecordToBeUpdated = true;
                }
                else {
                    isRecordToBeUpdated = false;
                }
            }
            else {
                Smo::NameOnStatus(smo);
                if ((isDataToBeUpdated = smo->m_targetValue.isValid())) {
                    smo->m_targetValue.clear();
                }
            }
            //debug.Assert(smo->ShortName == "Vset,mV");
            debug_tree_info("Smo: " + iif(Smo::ShortName(smo).isEmpty(), "<anonymous>", Smo::ShortName(smo)) + iif(isCallSuccessful, "", QObject::tr("; error: %1; %2").arg(QString(EnumKeys<SMO::Flags>((int)smo->flags()))).arg(Smo::sMessage(smo))));
            if (isRecordToBeUpdated)
            { //TODO: may be union record to be updated and flags changed!!!
                smo->flagsSet(SmoFlags::FlagIOk, isCallSuccessful);
                emit smo->recordChanged(); // signal to uptate if only previous integrity were ok!!!
                emit smo->parentSmo()->childrenChanged();
            }
        }
            break;
        case SmoBusKey_retStatus:  //for Ack frames get its status (for Ack frames no multy status supported!!!)...
        {
            smo->flagsSetClear(SmoFlags::FlagIOStatus, SmoFlags::FlagIOError | SmoFlags::FlagIOData);
            isDataToBeUpdated = SMOTargetMemory_update(smo, frame->Packet.Data, CargoRealLength, CargoTotalLength);
            G8 mainStatus = iif(CargoTotalLength, (G8)(*(byte*)smo->m_targetMemory.data()), err_Undefined);
            smo->m_mainStatusCode = mainStatus;
//            G8 mainStatus = Smo::Smo_status_parser(smo);
            isCallSuccessful = (mainStatus == G8::msg_Success || mainStatus == G8::msg_ZeroString || mainStatus == G8::msg_Executed);
            if (isCallSuccessful)
            {
                isDataToBeUpdated = smo->setTargetValueNull();
            }
            else
            {
                Smo::setsMessage(smo, smo->deviceInfo()->DeviceName() + " : " + Smo::StatusDescript(smo));
                debug_tree_info("Status Smo:" + Smo::sMessage(smo));
                isDataToBeUpdated = smo->setTargetValueClear();
            }
        }
            break;
        case SmoBusKey_retDevice: //else if (LLAMode == LowLevelAccessMode::LLA_SlaveDeviceInit)
            //TODO: handle SmoBusKey_retDevice!!!
            //        if (Smo::ParseAndFillDeviceInfo(smo)) {
            //            Smo::setsMessage(smo, smo->TargetValue.value<Smo>().deviceInfo->DeviceName());
            //        }
            //        else {
            //            smo->m_isCallSuccessful = false;
            //            Smo::setsMessage(smo, "Device error: " /*+ Smo::sMessage(smo->TargetValue.value<Smo>())*/);
            //        }
            break;
        case SmoBusKey_readRecord:
        case SmoBusKey_call:
        case SmoBusKey_callGroup:
        case SmoBusKey_callGroupWithData:
            isCallSuccessful = false;
            break;
        }

        smo->flagsSet(SmoFlags::FlagIOk, isCallSuccessful);
        if (old_flags != smo->m_flags) {
            emit smo->flagsChanged();
        }

        if (isDataToBeUpdated) {
//            debug_func("e m i t smo->data Changed()!!!");
//            if (smo->ShortName().isEmpty()) {
//                debug_func("empty name" << smo->targetValue());
//            }
//            else {
//                debug_func(smo->ShortName() << smo->targetValue());
//            }
            emit smo->dataChanged();
        }
//        lastError = msg_Success;
        return smo;
    }

    lastError = msg_NotReady;
    return 0;
}

void TDIPort::timerEvent(QTimerEvent *e)
{
    if (isOpen()) {
//        delay_thread(0);
//        debug_func(" port open: ok;" << QThread::currentThread() << QThread::currentThreadId() << QThread::idealThreadCount());
        m_asyncSmo = readFrame();
        start(m_scan_period_mS);
    }
    else
    { //TODO: what?
//        debug_msg(__func__ << " port open: no");
        start(m_scan_period_mS * PORT_CLOSED_SCAN_MULT);
    }
    e->accept();
}

///
/// \brief TDIPort::SMOTargetData_update
/// \param smo
/// \param data
/// \param dataLength
/// \param resultLength
/// \return true if data changed & need updated!!!
bool TDIPort::SMOTargetMemory_update(Smo *smo, const char *data, const int dataLength, const int resultLength)
{ //check & set m_targetMemory...
    if (smo->m_targetMemory.length() == resultLength) {
        return mem_update(smo->m_targetMemory.data(), data, dataLength, resultLength);
    }
    smo->m_targetMemory.resize(resultLength);
    smo->m_targetMemory.replace(0, dataLength, data, dataLength);
    if (resultLength > dataLength) ::memset((char*)smo->m_targetMemory.constData() + dataLength, 0, resultLength - dataLength);
    return true;
}

///
/// \brief TDIPort::SMOTargetRecord_update
/// \param smo
/// \param data
/// \param dataLength
/// \param resultLength
/// \return true if data changed & need updated!!!
bool TDIPort::SMOTargetRecord_update(Smo *smo, const char *data, const int dataLength, const int resultLength)
{
    if (resultLength == smo32Size || resultLength == smo16Size) {
        bool isok = mem_update((char*)&smo->m_record, data, dataLength, resultLength);
        if (resultLength != iif(smo->IsName24(), smo32Size, smo16Size)) smo->flagsSet(SmoFlags::FlagIntegrityError);
        return isok;
    }
    if (resultLength == smoAccessDenied_return_size) {
        smo->flagsSet(SmoFlags::FlagInvisible);
        return mem_update((char*)&smo->m_record, data, dataLength, resultLength);
    }
    debug_func(QString("resultLength wrong: %1").arg(resultLength));
    smo->flagsSet(SmoFlags::FlagIntegrityError);
    return false;
}

Smo *TDIPort::syncSmo() const
{
    return m_syncSmo;
}

Smo *TDIPort::identifySMO(uint deviceID, smokey key)
{
    DeviceInfo* device = iif(m_rootDevice->netAddress() == deviceID, m_rootDevice.data(), m_rootDevice->findChildDevice(deviceID));
    if (!device) {
        debug_func("device has not found for address: " << QString("0x%1").arg(deviceID, 0, 16));
        return 0;
    }
    Smo* smo = device->findSMO(key);
    if (!smo) {
        debug_func("Smo has not found for key: " << QString("0x%1").arg(key, 0, 16));
        return 0;
    }
    return smo;
}

Smo *TDIPort::asyncSmo() const
{
    return m_asyncSmo;
}

