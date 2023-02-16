
#include <QThread>

#include "treelib.h"
#include "zvvdebug.h"
#include "tdiroot.h"
#include "TextCodecs.h"
#include "smodatatypedefs.h"

#include "deviceinfo.h"

#ifndef TextCodec_onDefault
#define TextCodec_onDefault TextCodecs::UTF_8
#endif

static const QString User_groups[] = {QString("user"), QString("power user"), QString("owner"), QString("tuner")};
const Qt::ConnectionType DeviceInfo::Connection_unique_auto = (Qt::ConnectionType)(Qt::AutoConnection | Qt::UniqueConnection);

/// \brief DeviceInfo::DeviceInfo on a HW port!!!
/// \param devPort
/// \param port_smo or parent or originator
DeviceInfo::DeviceInfo(TDIPort *devPort, Smo *port_smo)
: QObject(devPort)
, m_portSMO(port_smo)
, m_HWPort(devPort)
//, m_deviceName(devPort->name())
, m_netAddress(SmoBus_FirstPhAddress)
{
    setObjectName(devPort->name());
    TdiInfo_reset();
    m_HWPort->m_rootDevice = this;
    port_smo->m_deviceInfo = this;
}

/// \brief DeviceInfo::DeviceInfo upon a port Smo!!!
/// \param port_smo or parent or originator
DeviceInfo::DeviceInfo(Smo *port_smo)
: QObject(port_smo->deviceInfo())
, m_portSMO(port_smo)
{
    TdiInfo_reset();
    if (port_smo->deviceInfo()) {
        m_HWPort = port_smo->deviceInfo()->m_HWPort;
        m_NETPath += port_smo->deviceInfo()->m_NETPath;
        QDataStream strim(&m_NETPath, QIODevice::WriteOnly);
        strim << port_smo->key();
    }
}

/// \brief DeviceInfo::DeviceInfo on a device Smo!!!
/// \param parent
/// \param port_smo or parent or originator
//DeviceInfo::DeviceInfo(DeviceInfo *parent, Smo *port_smo)
//: QObject(parent)
//, m_portSMO(port_smo)
//{ // clone device info for a child device...
//    TdiInfo_reset();
//    if (port_smo->deviceInfo()) {
//        m_HWPort = port_smo->deviceInfo()->m_HWPort;
//        m_NETPath += port_smo->deviceInfo()->m_NETPath;
//        QDataStream strim(&m_NETPath, QIODevice::WriteOnly);
//        strim << port_smo->key();
//    }
//}

DeviceInfo::~DeviceInfo()
{
    debug_func("deviceItem:" << iif(m_deviceSmo, m_deviceSmo->ShortName(), "wo name") << "; " << QThread::currentThread() << QThread::currentThreadId());
//    if (m_HWPort) {
//        m_HWPort->deleteLater();
//    }
//    if (m_thread) {
//        m_thread->quit();
//        m_thread->wait(10000);
//    }
}

void DeviceInfo::portScanInit()
{
    if (m_HWPort) {
//        if (!m_thread) {
//            m_thread = /*qApp->thread();*/ new QThread(parent());
//            m_thread->setObjectName(m_parentSMO->ShortName()); //same as: qobject_cast<Smo*>(m_HWPort->portItem())->ShortName()
//        }
//        if (m_HWPort->parent()) {
//            m_HWPort->setParent(0);
//        }
        m_HWPort->portScan(true);
//        m_HWPort->moveToThread(m_thread);

//        connect(this,  &DeviceInfo::SMODataToPort, m_HWPort, &TDIPort::ItemDataPort, Connection_unique_auto);
//        connect(m_HWPort, &TDIPort::ItemDataNotPorted, this, &DeviceInfo::portError, Connection_unique_auto);
//        connect(m_HWPort, &TDIPort::InputActivityDetected, this, &DeviceInfo::portInputData, Connection_unique_auto);
//        m_thread->start();
//    }

//    if (!m_portTimer) {
//        m_portTimer = new PortTimer(this);

//        m_thread = new QThread(parent());
//        m_thread->setObjectName("portScan");
//        m_portTimer->setParent(0);
//        m_portTimer->moveToThread(m_thread);
//        m_thread->start();
    }
}

///TdiInfo_reset() set version to 0(it means no actual device info present!!!)
///TdiInfo_reset() set TextCodec_onDefault
void DeviceInfo::TdiInfo_reset()
{
    m_TdiInfo.resize(sizeof (TDIInfo_t));
    TDIInfo_t* idata = (TDIInfo_t*)m_TdiInfo.data();
    idata->all = 0;
    idata->TDICharset = TextCodec_onDefault;
    m_textCodec = QTextCodec::codecForMib(TextCodec_onDefault);
}

uint DeviceInfo::netAddress() const
{
    return m_netAddress;
}

void DeviceInfo::setLoginSMO(Smo *LoginSMO)
{
    m_LoginSMO = LoginSMO;
}

QByteArray* DeviceInfo::getDescPtr() const
{
    return (QByteArray*)iif(m_descriptSmo, &m_descriptSmo->m_targetMemory, 0);
}

void DeviceInfo::setDesc(Smo* desc)
{ //TODO:it
    m_descriptSmo = desc;
}

void DeviceInfo::setDeviceName(const QString &deviceName)
{
    setObjectName(deviceName); //    m_deviceName = deviceName;
}

void DeviceInfo::SetUserName(const QString &userName)
{
    m_userName = userName;
}

QByteArray DeviceInfo::NETPath() const
{
    return m_NETPath;
}

TDIPort *DeviceInfo::HWPort() const
{
    return m_HWPort;
}

UInt16 DeviceInfo::FirstSmoKey() const
{
    return m_firstSmoKey;
}

void DeviceInfo::setFirstSmoKey(UInt16 firstSmoKey)
{
    m_firstSmoKey = firstSmoKey;
}

Smo *DeviceInfo::PortSMO() const
{
    return m_portSMO;
}

void DeviceInfo::setDeviceSmo(Smo *device_smo)
{
    if (device_smo) {
        setObjectName(device_smo->ShortName());
    }
    m_deviceSmo = device_smo;
}

Smo *DeviceInfo::deviceItem() const
{
    return m_deviceSmo;
}

QString DeviceInfo::getUserName() const
{
    return m_userName;
}

LoginedUserGroup_t DeviceInfo::getUserGroup() const
{
    return m_userGroup;
}

Smo *DeviceInfo::getLoginSMO() const
{
    return m_LoginSMO;
}

//ReadTdiInfo() returns data length if read successfull else 0...
int DeviceInfo::ReadTdiInfo(DeviceInfo *devinfo)
{
    Smo *smo = devinfo->m_deviceSmo;
    QByteArray param;
    if (Smo::Call(smo, &param)) {
        uint result_length = smo->m_targetMemory.length();
        if (result_length >= sizeof (TDIInfo_t))
        {
            if (result_length > sizeof (TDIInfo_t)) devinfo->m_TdiInfo.resize(result_length);
            memcpy(devinfo->m_TdiInfo.data(), smo->m_targetMemory.data(), result_length);
            return result_length;
        }
    }
    return 0;
}

bool DeviceInfo::IsDeviceVersionSupported(u16 ver)
{
    switch (ver) {
    case TDISupportedVersion11:
        return true;
    default:
        return false;
    }
}

bool DeviceInfo::IsUserAccepted()
{
    return this->m_userGroup & smoLoginAccepted_bm;
}

SmoAccess DeviceInfo::AccessState(Smo *smo)
{
    return (SmoAccess)((smo->SecByte().all >> this->UserGroupShifter()) & SmoAccess_mask);
}

int DeviceInfo::UserGroupShifter()
{
    uint ugroup = (uint)this->m_userGroup;
    return UserGroup_masked(ugroup) * 2;
}
QTextCodec *DeviceInfo::textCodec() const
{
    return m_textCodec;
}
void DeviceInfo::setTextCodec(QTextCodec *textCodec)
{
    if (textCodec) {
        m_textCodec = textCodec;
    }
}

u16 DeviceInfo::TdiInfo_version(DeviceInfo *info)
{
    return ((TDIInfo_t*)info->m_TdiInfo.data())->TDIVersion;
}

u16 DeviceInfo::TdiInfo_charset(DeviceInfo *info)
{
    return ((TDIInfo_t*)info->m_TdiInfo.data())->TDICharset;
}

u32 DeviceInfo::TdiInfo_crc(DeviceInfo *info)
{
    return ((TDIInfo_t*)info->m_TdiInfo.data())->TdiCRC;
}

QString DeviceInfo::DeviceName()
{
    return objectName(); //iif(m_deviceName.isEmpty() && m_deviceSmo, m_deviceSmo->ShortName(), m_deviceName);
}

//user info updaters...
void DeviceInfo::SetupUserInfo(LoginedUserGroup lug, const QString& user_name)
{
    m_userGroup = lug;
    if ((uint)lug & smoLoginAccepted_bm)
    {
        m_userName = iif(user_name.isEmpty(), User_groups[UserGroup_masked(lug)], user_name);
    }
    else {
        m_userName = QString();
    }
}
///LoginedUser_update() expects the smo with m_targetMemory retrieved...
bool DeviceInfo::LoginedUser_update(Smo *smo, const QString& user_name)
{
    SmoDataType sdt = smo->dataType();
    LoginedUserGroup lug = LoginedUserGroup::NoLoginedUser;
    smoKey user_Key = 0;
    if (sdt == SmoDataType::ControlLogin)
    {
        if (smo->m_targetMemory.length() == sizeof (ControlLogin_t)) {
            ControlLogin_t* ctrlLoginData = (ControlLogin_t*)smo->m_targetMemory.data();
            user_Key = ctrlLoginData->Loginkey;
            lug = (LoginedUserGroup)ctrlLoginData->UserGroup;
        }
    }
    else if (sdt == SmoDataType::ControlPassLine)
    {
        if (smo->m_targetMemory.length() == sizeof (ControlPassLine_t)) {
            ControlPassLine_t* ctrlPassLineData = (ControlPassLine_t*)smo->m_targetMemory.data();
            lug = (LoginedUserGroup)ctrlPassLineData->UserGroup;
        }
    }
    else {
        return false;
    }
    DeviceInfo* inf = smo->deviceInfo();
    inf->m_LoginSMO = smo;
    inf->m_userKey = user_Key;
    inf->SetupUserInfo(lug, user_name);
//DEPRECATED, use signal/slot in when init...    fDefault::PageHeader_update(smo);
    return true;
}

QString DeviceInfo::LoginedUserGroup2UserGroupName(LoginedUserGroup lug)
{
    return User_groups[UserGroup_masked(lug)];
}

Smo *DeviceInfo::findSMO(smokey key) const
{
    return Smo::FindkeyLocaly(m_deviceSmo, key);
}

DeviceInfo *DeviceInfo::findChildDevice(uint address)
{
    return TreeLib::FindQObject<DeviceInfo>(this, [address](DeviceInfo* d)->bool { return d->netAddress() == address;}, true, Qt::FindDirectChildrenOnly);
}


