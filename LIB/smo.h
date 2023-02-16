
#ifndef SMO_H
#define SMO_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QString>
#include <QWidget>
#include <QVariant>
#include <QDateTime>
#include <QFileInfo>
#include <QPointer>
#include <QMetaObject>
#include <QQmlListProperty>

#include "compiler.h"
#include "GlobalStatusErrorCodes.h"
#include "zvvDataTypes.h"
#include "SmoRecord.h"
#include "SmoBusData.h"
#include "deviceinfo.h"
#include "smoconstants.h"
#include "deplugin.h"
#include "g8enum.h"
#include "loginedusergroup.h"
#include "smoinfo.h"
#include "smolistproperty.h"
//#include "demodel.h"
#include "smomodel.h"
#include "treeitem.h"

class DEWidget;
class DeviceInfo;
class Smo;

//typedef SMO::Flags SmoFlags;
typedef QList<Smo*> SMOList;

#define GetBaseOfNumber(str) 0 /*0=auto detection(see desc for base param in QString::toUShort()), or use: zvvlib::BaseOfNumber()*/

class Smo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant data READ dataValue WRITE setTargetValueAndCall NOTIFY dataChanged)
    Q_PROPERTY(int /*SmoFlags::StatusFlags*/ flags READ flags NOTIFY flagsChanged)
    Q_PROPERTY(Smo* parent READ parentSmo NOTIFY parentChanged)
    Q_PROPERTY(QQmlListProperty<Smo> folderItems READ smoChildren NOTIFY childrenChanged)
    Q_PROPERTY(DeviceInfo* device READ deviceInfo NOTIFY smoNotify)
    Q_PROPERTY(int /*SmoDataType*/ dataType READ dataType NOTIFY smoNotify)
    Q_PROPERTY(int /*SmoHeadType*/ smoType READ headType NOTIFY smoNotify)
    Q_PROPERTY(int /*SmoFolderType*/ folderType READ folderType NOTIFY smoNotify)
    Q_PROPERTY(QVariant folderModel READ folderModel NOTIFY smoNotify)

public:
    Smo() {} //NOTE: Smo() {} need for metasystem!!!
    Smo(const QString& name, QObject* parent = 0);
    Smo(Smo *parent, const QString& name);
//    Smo(QObject* parent = 0);
//    Smo(Smo *source); //clone!!!

    Smo(DeviceInfo* inf, smokey key);
    Smo(Smo* parent_smo, smokey key);

    Smo(const Smo& other);
    ~Smo();

    Q_DECLARE_FLAGS(StatusFlags, SMO::Flags)

public:
    Q_INVOKABLE QString flagsToString() const;
    Q_INVOKABLE QString inviteString() const;
    Q_INVOKABLE QVariant readData();
    Q_INVOKABLE int writeText(const QString& wrtext);
    Q_INVOKABLE int writeData(QVariant data);
    Q_INVOKABLE int folderItemsCount() const;

    int flags() const;
    int flagsSet(int flags_to_set);
    int flagsSet(int flags_to_set_or_clear, bool isSetOrClear);
    int flagsClear(int flags_to_clear);
    int flagsSetClear(int flags_to_set, int flags_to_clear);
    void flagsReset(int new_flags);
    G8 mainStatusCode() const;

    QQmlListProperty<Smo> smoChildren();
    int childCountQml() const;
    Smo* childAtQml(int index) const;

    Smo *parentSmo() const;
    DeviceInfo *deviceInfo() const;

    TreeItem<Smo> *treeItem() const;
    void setTreeItem(TreeItem<Smo> *treeItem);
//    static void setTreeItem(Smo* smo, TreeItem *treeItem);

    bool isCallSuccessful() const;
    void setSuccessful(bool isSuccessful);
    const char *iconName() const;
    smokey key() const;

    DEPlugin* plugin() const;
    void setPlugin(DEPlugin* plugin);
    DEWidget *expandedWidget() const;
    void setExpandedWidget(DEWidget *expandWidget);
    DEWidget *collapsedWidget() const;
    void setCollapsedWidget(DEWidget *collapsedWidget);

    QByteArray *targetMemory() const;

    QVariant dataValue();

//Record management...
    SmoHeadType headType() const;
    void setHeadType(SmoHeadType type);

    bool IsName24() const;
    void setIsName24(bool value);

    smokey Parent_key() const;
    void setParent_key(smokey value);

    bool IsParentToCode() const; //old: IsParentToFlash

    SmoSecurityByte SecByte() const;
    void setSecByte(SmoSecurityByte value);

    SmoDataType dataType() const;
    void setDataType(SmoDataType datatype);

    SmoFolderType folderType() const;
    void setFolderType(SmoFolderType type);

    QVariant folderModel();

    u32 DataAddressU32() const;
    void setDataAddressU32(u32 value);

//end Record management!!!


//deprecate non-static methods... use static!!!
//    bool Read() {return Smo::Read(this);}
//    bool Call() {return Smo::Call(this);}
//    bool Call(QVariant    data) {return Smo::Call(this, data);}
//    bool Call(QByteArray* data) {return Smo::Call(this, data);}

    QString toString();

    QString ShortName();

    static void SetShortName(Smo* smo, const QString &value);
    static int DecodeRecordName(Smo* smo);
    static void SetName_notParsed(Smo* smo, const QString & fullname);
    static void SetName_Parsed(Smo* smo, const QString &fullname);

    void Record_clear(void);

//public static...
    static const QByteArray& DisplayFormat(Smo *smo);
    static Smo *NewChildSmo(Smo *parent_smo, u16 nKey);
//not used...    static Smo *NewFriendSmo(Smo *friendSmo, smokey newkey);

    static const char *StatusToString(StatusFlags value);

    static Smo *Message(Smo *parent, const QString &description, const char *icon);
    static Smo *MessageInfo(Smo *parent, const QString &description);
    static Smo *MessageWarning(Smo *parent, const QString &description);
    static Smo *MessageError(Smo *parent, const QString &description);

    static bool IsRecordsEqual(Smo *smoL, Smo *smoR);

    static void Record_copy(Smo *destination, QByteArray *source);
    static void Record_copy(Smo *destination, Smo *source);

    static bool Read(Smo *smo);
    static bool Call(Smo *smo);
    static bool Call(Smo *smo, QVariant data); //WARNING: do not use a ptr (because ptr used in: Call(Smo *smo, QByteArray *data))
    static bool Call(Smo *smo, QByteArray *data);

    static bool calcTargetMemory(Smo *smo);
    static bool calcTargetMemory(Smo *smo, QVariant* data);

    static bool IsEndTable(Smo *smo);

    static SmoDataType IsStringAndType(Smo *smo);

    static SmoDataType IsControlAndType(Smo* smo);

    static SmoDataType IsBitAndType(Smo *smo);
    static bool IsDateTime(Smo *smo);
    static bool IsDate(Smo *smo);
    static bool IsTime(Smo *smo);
    static bool IsPass(Smo *smo);
    static bool IsUnsigned(Smo *smo);
    static bool IsNumeric(Smo *smo);

    static u32 DataAddress(Smo *smo);

    static u8 SmoKey2tag(smoKey smo_Key);
    static smokey SmoKey2key(smoKey smo_Key);

    static u16 FolderDataAddress(Smo *smo);
    static void setFolderDataAddress(Smo *smo, u16 value);

    static smokey Child_key(Smo *smo);
    static void setChildKey(Smo *smo, smoKey value);

    static Smo* FindkeyLocaly(Smo *smo, smokey key);
//    static Smo* Readkey(DeviceInfo* devinfo, smokey key);
    static Smo* Findkey(DeviceInfo* devinfo, smokey key);

    static QString Smokey2name(DeviceInfo *devinfo, smokey key);
    static QString SmoKey2NameOrStatus(DeviceInfo *devinfo, smoKey Key);

    static const char* RecNameField(Smo *smo);

    static QString ShortName(Smo *smo);

    static SmoAccess AccessState(Smo *smo);

    static const char *Status2ImageKey(Smo *smo);
    static const char *Status2ImageKey(G8 mainStatus);

    static bool IsDevice(Smo *smo);

    static Smo *ViewItem_add(Smo *smo);
    static Smo *ViewItem_update(Smo *smo);
    static Smo *ViewItem_addOrUpdate(Smo *smo, bool isAddOrUpdate);

    static bool CallEnvironment_check(Smo *smo);
    static Smo *DeviceInfo_init(Smo *smo);
    static bool ClearTargetData(Smo *smo);
//    static bool ClearTargetData_and_setCallStatus(Smo *smo, SmoTextualErrors code);
    static bool ClearTargetData_and_setCallStatus(Smo *smo, const QString& statusMessage);
    static bool ClearTargetData_and_setCallStatus(Smo *smo, SmoFlags callStatus, const QString& statusMessage);

    static Smo *RefreshLinkedDevice(Smo *deviceSmo);
    static Smo *RefreshChildren(Smo *parentSmo, smokey smo_key = 0, const QString smo_name = QString()); //before201704    static Smo *RefreshChildren(Smo *parentSmo, Smo *targetSmo = NULL);

    static smokey nextSmo_key(Smo *smo);
//    static void Update(Smo *targetSmo, Smo *sourceSmo);

    static bool SmoBus_frame_make(Smo *smo, QByteArray *retv, QByteArray *data);
    static void SmoBus_shrinkAndCalcCRC(QByteArray *frame);
    static bool SmoBus_virtualPath_make(Smo *smo, QByteArray *cVirtualPath);
//    static bool SmoBus_LowLevelCall(Smo *smo, const QByteArray& netFrame /*, LowLevelAccessMode LLAMode*/);
    static byte SmoBus_offseter_get();
    static bool SmoDevice_TdiInfo_readAndCheck(Smo *smo);
//    static bool SmoBus_waitAnswer(TDIPort *tdiPort, Smo *smo);
    static int Smo_status_parser(Smo *smo);

    static QString StatusDescript(Smo *smo);
    static QString sMessage(Smo *smo);
    static bool setsMessage(Smo *smo, const QString& value);

    static void UserGroup_set(Smo *smo, LoginedUserGroup newGroup);
    static int  FindUserGroup(Smo *smo, bool forVisibleOrInvisible);

    static bool AccessState_and_userPassStatus_forecast(Smo *smo, bool IsVisible);
    static bool RecordParser(Smo *smo, const int recordLength);
    static bool SelfParserForInvisible(Smo *smo);
    static void NameOnStatus(Smo *smo);
//    static bool ParseAndFillDeviceInfo(Smo *smo);
    static SmoUITypes UIType(Smo *smo);

    static QString TargetMemoryToString(Smo* smo);
    static QString DecodeString(Smo* smo, const char* data, size_t length);
    static QByteArray EncodeString(Smo* smo, const QString &data);

    static QVariant SmartBackTransformValue(Smo *smo, const QString &data);
    static bool SmartBackTransform(Smo* smo, const QString& data);

    static QString Path(Smo* smo, enum PathOption whatPath = path_relative, const QChar& delimeter = QChar('/'));

    static bool IsSmoKeyRangeValid(smoKey sKey);
    static bool IntegrityCheckFailed(Smo *smo);
    static bool IntegrityCheck(Smo* smo);
    static bool IsIntegrityOk(Smo* smo);

    static bool IsDataEnabled(Smo* smo);
    static bool IsAccessible(SmoAccess smoAccess);

    static ushort DeviceCharset(Smo* smo);

    static void SetIcon(Smo *smo, const char* iconName);
    static const char* SelectIcon(Smo *smo);

    static void setParentSmo(Smo *smo, Smo *parent_smo);

    static QRegExp nameDelimeter();

//proxy for QTreeWidgetItem...
    bool isExpanded() const;
    void setExpanded(bool toBeExpanded);
    int childCount();
    Smo* child(int index);
    void setToolTip(int index = 0, const QString& tip = QString());
    QString toolTip(int index = 0);
    void addChild(Smo* smo);
    Smo *takeChild(int index);
    QList<Smo*> takeChildren();
    void setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy policy);
    void setIcon(const QIcon& icon, int index = 0);
    QIcon icon(int index = 0);
    void setHidden(bool toBeHidden);
    QString text(int index = 0);
//end proxy for QTreeWidgetItem

//TODO: move all public vars to private with getters/setters!!!
public: //public vars...
    QVariant       Tag;

signals:
    void dataChanged(); //e mit on QByteArray changes!!!
    void dataInvalid(const QVariant& data);
    void ioError();
    void recordChanged();
    void flagsChanged();
    void parentChanged();
    void childrenChanged();
    void allChildrenRefreshed();
    void smoNotify();

public slots:

private:
    static int childCountQml(QQmlListProperty<Smo>* list);
    static Smo* childAtQml(QQmlListProperty<Smo>* list, int i);


    void setTargetValueAndCall(QVariant data);
    bool setTargetValueClear();
    bool setTargetValueNull();
    G8 updateTargetValue();

//private vars.........................................................
    QByteArray     m_targetMemory; // = NULL
    QVariant       m_targetValue;  // = NULL
    QPointer<DeviceInfo> m_deviceInfo = NULL; //TODO: compare m_deviceInfo & parent() is the same???

    smokey         m_key = 0; //old: u16 Key
    const char*    m_iconName = 0;
    G8             m_mainStatusCode = err_IntegrityError; //msg_Success; //
    int /*StatusFlags*/    m_flags = 0; //0=means a new smo!!! SmoFlags::IntegrityError;

    SMORecord32_t  m_record;

    QPointer<DEPlugin> m_plugin;
    /*QPointer<*/DEWidget* m_expandedWidget = 0;  //its parent equals MainWindow
    /*QPointer<*/DEWidget* m_collapsedWidget = 0; //its parent equals MainWindow

    QPointer<Smo>  m_parentSmo = 0;
    QPointer<Smo>  m_virtualParent = NULL; /*may be always a parent??? */

    QString        m_fullname;
//    /*DEPRECATED,use: smo->objectName, smo->setObjectName()... now only for GDB helper: */ QString       m_shortName;
    QStringRef     m_inviter;
    QByteArray     m_formatter;  //with % char //old:    QStringRef    m_formatter;  //with % char
    QString        m_sMessage; //NOTE: m_sMessage doesn't valid without: smo->m_flags |= SMOStatus::SystemMessage;

    QDateTime      m_targetAccessTime;

    TreeItem<Smo>*      m_treeItem = 0;

//    QDateTime      TargetAnswerTime; //DateTime

    friend class DeviceInfo;
    friend class TDIPort;
};  //class Smo : public QTreeWidgetItem

Q_DECLARE_METATYPE(Smo)
Q_DECLARE_METATYPE(Smo*)
Q_DECLARE_METATYPE(const Smo*)

template<typename T>
T SmoScanf(const QString& data, Smo* smo, bool* isok) {
    return sscanf_to_value<T>(data.toLatin1().constData(), Smo::DisplayFormat(smo).constData(), isok);
}

template<typename T>
QString SmoData_NumberToString(Smo* smo) {
    if ((size_t)smo->targetMemory()->length() < sizeof (T)) return QString();
    if (Smo::DisplayFormat(smo).isEmpty()) {
        return QString::number(*(T*)smo->targetMemory()->data());
    }
    else {
        return QString::asprintf(Smo::DisplayFormat(smo).constData(), *(T*)smo->targetMemory()->data());
    }
}


//#ifndef IOSmoRead_TryCounter
//#define IOSmoRead_TryCounter 2
//#endif
//#ifndef IOSmoCall_TryCounter
//#define IOSmoCall_TryCounter 2
//#endif

#endif // SMO_H
