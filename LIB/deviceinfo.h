#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QPointer>
#include <QTimer>


#include "app_config.h"
#include "smo.h"
#include "tdiport.h"
//#include "SmoRecord.h"
//#include "tdiroot.h"
//#include "TextCodecs.h"
#include "loginedusergroup.h"
//#include "smodatatypedefs.h"

//#ifndef TextCodec_onDefault
//#define TextCodec_onDefault TextCodecs::UTF_8
//#endif

enum TDISupportedVersions {
    TDISupportedVersion11 = 0x0101,
};

class Smo;
class TDIPort;
//class PortTimer;

typedef enum LoginedUserGroup LoginedUserGroup_t;
typedef QMap<smokey, QPointer<Smo>> Smokey2SMO_map;

class DeviceInfo : public QObject
{
    Q_OBJECT    /* excluded if no slots/signals in the class... Q_OBJECT */
    Q_PROPERTY(Smo* deviceItem READ deviceItem NOTIFY deviceChanged)

public:
    DeviceInfo(TDIPort *devPort, Smo* port_smo = 0);
    DeviceInfo(Smo* port_smo = 0);
    DeviceInfo(const DeviceInfo& other) : QObject(other.parent()) {}
    ~DeviceInfo();

    TDIPort *HWPort() const;

    QByteArray NETPath() const;

    Smo *deviceItem() const;
    void setDeviceSmo(Smo *device_smo);

    Smo *PortSMO() const;

    UInt16 FirstSmoKey() const;
    void setFirstSmoKey(UInt16 firstSmoKey);

    QString       DeviceName();
    void setDeviceName(const QString &deviceName);

    inline int    UserGroupShifter();

    bool          IsUserAccepted();
    SmoAccess     AccessState(Smo *smo);

    QTextCodec*   textCodec() const;
    void setTextCodec(QTextCodec *textCodec);

    QByteArray *getDescPtr() const;
    void setDesc(Smo *desc);

    void SetupUserInfo(LoginedUserGroup lug, const QString &user_name = QString());

    Smo* getLoginSMO() const;
    void setLoginSMO(Smo *LoginSMO);

    LoginedUserGroup_t getUserGroup() const;

    QString getUserName() const;
    void SetUserName(const QString &userName);

    QString getUserGroupName() const;

    void portScanInit();

    Smo* findSMO(smokey key) const;
    DeviceInfo* findChildDevice(uint address);
    uint netAddress() const;

//next: public static...
    static const Qt::ConnectionType Connection_unique_auto;
    static int  ReadTdiInfo(DeviceInfo* devinfo);
    static bool IsDeviceVersionSupported(u16 ver);
    static u16  TdiInfo_version(DeviceInfo* info);
    static u16  TdiInfo_charset(DeviceInfo* info);
    static u32  TdiInfo_crc(DeviceInfo* info);
    static bool LoginedUser_update(Smo* smo, const QString& user_name = QString());
    static QString LoginedUserGroup2UserGroupName(LoginedUserGroup lug);


signals:
    void deviceChanged();
    void SMODataToPort(Smo* smo, const QByteArray& data);

public slots:
//    void portError(QObject* item, int error_code, const QString &message);
//    void portInputData(const QByteArray& rxBuff);

private:
    //TODO:it    Smokey2SMO_map Smo_map;
    void TdiInfo_reset();

    QPointer<Smo>      m_portSMO = 0; /* parent of the m_deviceSmo */
    QPointer<Smo>      m_deviceSmo = 0; /* next, child of the m_portSMO */

    QPointer<TDIPort>  m_HWPort = 0;
    QByteArray         m_NETPath;
    QByteArray         m_TdiInfo; //will be read from a device(no TDIInfo_t because a device may have an extra info)
    UInt16             m_firstSmoKey = 0;
    QTextCodec*        m_textCodec;
//deprecated,use QObject() objectName()/setObjectName()    QString            m_deviceName;

    QPointer<Smo>      m_LoginSMO = 0;
    LoginedUserGroup_t m_userGroup = LoginedUserGroup::NoLoginedUser;
    smoKey             m_userKey   = 0;
    QString            m_userName;
    QPointer<Smo>      m_descriptSmo = 0;
    uint               m_netAddress = 0;

//    QThread*           m_thread = 0;
};
Q_DECLARE_METATYPE(DeviceInfo)


#endif // DEVICEINFO_H
