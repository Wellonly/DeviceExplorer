/*
 ** This file is part of the DE project.
 ** Copyright 2017 Well <iconn8@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/


#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QRegExp>
#include <QApplication>

#if QT_VERSION >= 0x050800
#include <QDeadlineTimer>
#else
#include <QElapsedTimer>
#endif

#include "smolib.h"
#include "treeitem.h"
#include "LogTree.h"
#include "realtime32.h"
#include "compiler.h"
#include "treelib.h"
#include "zvvdebug.h"
#include "CRC.h"
#include "Offseters.h"
#include "smo.h"
#include "zvvserialize.h"

static Smo smodebug;

static QString TargetMemoryToString_data_length_wrong(int len);
static QString loginString(byte lug);

//TODO:find out if a Delimeter needed for every codec!!!
//static const QRegExp ShortNameDelimeter = QRegExp("([\x01-\x1f]|[\x21-\x2f]|[\x3a-\x40]|[\x5b-\x60]|[\x7b-\xbf])"); //this actual only for windows-1251!!!
static const QRegExp ShortNameDelimeter = QRegExp(QStringLiteral("(\\\\|\\||\"|\\^|\\(|\\~|\\!|\\@|\\#|\\$|\\&|\\*|\\+|\\-|\\/|\\?|\\;|\\<|\\>|\\:|\\{|\\[|№|,|%)")); //"-\"~!@#$^&*+/|\\?;<>=:({[№\0"
//const Qt::ConnectionType SMO::Connection_unique_auto = (Qt::ConnectionType)(Qt::AutoConnection | Qt::UniqueConnection);
//const Qt::ConnectionType SMO::Connection_unique_queued = (Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection);

Smo::Smo(const QString &name, QObject *parent)
: QObject(parent)
{
    m_treeItem = new TreeItem<Smo>(this, ShortName());
    SetName_notParsed(this, name);
}

Smo::Smo(Smo *parent, const QString &name)
: QObject(parent/*->parent()*/)
, m_deviceInfo(parent->m_deviceInfo)
, m_parentSmo(parent)
{
    m_treeItem = new TreeItem<Smo>(this, ShortName());
    parent->addChild(this);
    SetName_notParsed(this, name);
}

Smo::Smo(DeviceInfo *inf, smokey key)
: QObject(inf)
,m_deviceInfo(inf)
,m_key(key)
{
}

Smo::Smo(Smo *parent_smo, smokey key)
: QObject(parent_smo)
, m_deviceInfo(parent_smo->m_deviceInfo)
, m_key(key)
, m_parentSmo(parent_smo)
{

}

Smo::Smo(const Smo &other)
: QObject(other.parent())
//, m_parentSmo(parent_smo)
{
    Record_copy(this, (Smo*)&other);
}

Smo *Smo::NewChildSmo(Smo *parent_smo, u16 nKey)
{ // new Smo that will be a child with a blank record...
    Smo *smo = new Smo(parent_smo /*->m_deviceInfo*/, nKey);
//    smo->m_treeItem = new TreeItem<Smo>(smo);
//    smo->m_parentSmo = parentSmo;
    smo->m_virtualParent = iif(parent_smo->folderType() == SmoFolderType::LinkToFolder, parent_smo, parent_smo->m_virtualParent.data());
    return smo;
}

// A friend means: a Smo in the same device!!!
//Smo *Smo::NewFriendSmo(Smo *friendSmo, smokey newkey)
//{
//    Smo *smo = new Smo();
//    smo->m_parentSmo = NULL; //parentSmo->addChild(smo);
//    smo->key = newkey;     // (smokey)(key & smoKeyMask);
//    smo->deviceInfo = friendSmo->deviceInfo; //will be point to the same object
//    smo->VirtualParent = friendSmo->VirtualParent;
//    return smo;
//}

Smo::~Smo()
{
    debug_func("deleted:" << this);
    if (m_treeItem) {
        delete m_treeItem;
    }
}

int /*Smo::StatusFlags*/ Smo::flags() const
{
    return /*(Smo::StatusFlags)*/m_flags;
}

int Smo::flagsClear(int flags_to_clear)
{
    return (m_flags &= ~flags_to_clear);
}

int Smo::flagsSet(int flags_to_set)
{
    return (m_flags |= flags_to_set);
}

int Smo::flagsSet(int flags_to_set_or_clear, bool isSetOrClear)
{
    return iif(isSetOrClear, (m_flags |= flags_to_set_or_clear), (m_flags &= ~flags_to_set_or_clear));
}

int Smo::flagsSetClear(int flags_to_set, int flags_to_clear)
{
    int newFlagsValue = m_flags | flags_to_set;
    return (m_flags = newFlagsValue & ~flags_to_clear);
}

void Smo::flagsReset(int new_flags)
{
    if (m_flags != new_flags) {
        m_flags = new_flags;
        emit flagsChanged();
    }
}

QString Smo::flagsToString() const
{
    return QString(EnumKeys<SMO::Flags>((int)flags()));
}

int Smo::childCountQml(QQmlListProperty<Smo> *list)
{
    return reinterpret_cast<Smo*>(list->data)->childCountQml();
}

Smo *Smo::childAtQml(QQmlListProperty<Smo> *list, int i)
{
    return reinterpret_cast<Smo*>(list->data)->childAtQml(i);
}

//not used yet! may be useful DEPlugin!!!
//APPExtention::APPExtention(QString FName)
//{
//    dllFName = FName;
//    FormName = QFileInfo(FName).baseName();
//    AppLink = "DEX." + FormName;
//}
//APPExtention::APPExtention(QString appNameSpace, QString appFormName)
//{
//    FormName = appFormName;
//    AppsetTargetValueSpace + "." + FormName;
//}

bool Smo::IsName24() const
{
    return m_record.HeadByte.IsName24; //return ((SMORecord32_t*)this->Record)->HeadByte.IsName24;
}
void Smo::setIsName24(bool value)
{
    m_record.HeadByte.IsName24 = value;
}

smokey Smo::Parent_key() const
{
    return m_record.ParentKey & smoKeyMask;
}
void Smo::setParent_key(smokey value)
{
    u16 svb = m_record.ParentKey & ~smoKeyMask;
    m_record.ParentKey = (value & smoKeyMask) | svb;
}

bool Smo::IsParentToCode() const
{ //IsParentToFlash
    return m_record.HeadByte.IsParentToFlash;
}

SmoHeadType Smo::headType() const
{
    return m_record.HeadByte.HeadType; //return (SmoHeadType)(this->_Record[SmoConstants::PKey_offset] & SmoConstants::SmoHeadType_bm);
}
void Smo::setHeadType(SmoHeadType type)
{
    m_record.HeadByte.HeadType = type; //int svb = this->_Record[SmoConstants::PKey_offset] & ~SmoConstants::SmoHeadType_bm; this->_Record[SmoConstants::PKey_offset] = (byte)type | svb;
}

SmoSecurityByte Smo::SecByte() const
{
    return m_record.Security; //(byte)_Record[SmoConstants::SecByte_offset];
}
void Smo::setSecByte(SmoSecurityByte value)
{
    m_record.Security = value; //_Record[SmoConstants::SecByte_offset] = value;
}

SmoDataType Smo::dataType() const
{
    return m_record.Type.DataType; //_Record[SmoConstants::Type_offset];
}
void Smo::setDataType(SmoDataType datatype)
{
    m_record.Type.all = (byte)datatype; //_Record[SmoConstants::Type_offset] = (byte)datatype;
}

SmoFolderType Smo::folderType() const
{
    return (SmoFolderType)m_record.Data.Folder.FolderType;
}
void Smo::setFolderType(SmoFolderType type)
{
    m_record.Data.Folder.FolderType = (byte)type;
}

#define FOLDER_MODEL_DYNAMIC_PROPERTY_NAME "_d_foldModel"
QVariant Smo::folderModel()
{
    QVariant retv = property(FOLDER_MODEL_DYNAMIC_PROPERTY_NAME);
    if (retv.isValid()) {
        debug_func("retrieved for:" << objectName());
        return retv;
    }
    DeModel* model = new DeModel(deviceInfo(), this);
    if (model) {
        connect(this, &Smo::allChildrenRefreshed, model, &DeModel/*<Smo>*/::refreshModel, SMO::Connection_unique_queued);
        retv = QVariant::fromValue(model);
        setProperty(FOLDER_MODEL_DYNAMIC_PROPERTY_NAME, retv);
        debug_func("created for:" << objectName());
    }
    else {
        debug_func("NOT created for:" << objectName());
    }
    return retv;
}

u32 Smo::DataAddressU32() const
{
    return m_record.Data.MemAddress.Uint32LH; //Record.Data.DataAddress
}
void Smo::setDataAddressU32(u32 value)
{
    m_record.Data.DataAddress = value;
}

bool inline Smo::IsEndTable(Smo* smo)
{
    return (smo->m_flags & SmoFlags::FlagEndTable); //return smo->Record.ParentKey == smoEndTableMarker;
}

SmoDataType Smo::IsStringAndType(Smo *smo)
{
    SmoDataType retvalue = smo->dataType();
    return iif((int)retvalue & (1<<smoDTIsString_bp), retvalue, (SmoDataType)0);
}

SmoDataType Smo::IsControlAndType(Smo* smo)
{
    int dtype = (int)smo->dataType();
    return (SmoDataType)iif(dtype >= SmoDTControls_rangeStart && dtype <= SmoDTControls_rangeEnd, dtype, 0);
}

SmoDataType Smo::IsBitAndType(Smo* smo)
{
    int dtype = (int)smo->dataType();
    return (SmoDataType)iif(dtype >= SmoDTBits_rangeStart && dtype <= SmoDTBits_rangeEnd, dtype, 0);
}

bool Smo::IsDateTime(Smo *smo)
{
    return smo->dataType() == SmoDataType::DateTime64;
}

bool Smo::IsDate(Smo *smo)
{
    return smo->dataType() == SmoDataType::DateBSD32;
}

bool Smo::IsTime(Smo *smo)
{
    return smo->dataType() == SmoDataType::RealTime32;
}

bool Smo::IsPass(Smo *smo)
{
    return smo->dataType() == SmoDataType::ControlPassLine;
}

bool Smo::IsUnsigned(Smo *smo)
{
    return ((int)smo->dataType() & (1<<smoDT_IsUnsigned_bp)) !=0;
}

bool Smo::IsNumeric(Smo *smo)
{
    SmoDataType dt = smo->dataType();
    if ((int)dt & ((1<<smoDTIsString_bp) | (1<<smoDTIsControl_bp) | (1<<smoDT_IsBit_bp))) {
       return false;
    }
    return  dt != SmoDataType::Void &&
            dt != SmoDataType::Pointer16 && /*SmoPointer*/
            dt != SmoDataType::DateBSD32 &&
            dt != SmoDataType::RealTime32 &&
            dt != SmoDataType::DateTime64 ;
}

u32 Smo::DataAddress(Smo *smo)
{
    return smo->headType() == SmoHeadType::SmoHeadType_Folder ? smo->m_record.Data.MethodAddress & 0xFFFF : smo->DataAddressU32();
}

u8 Smo::SmoKey2tag(smoKey smo_Key)
{
    return smo_Key & ~smoKeyMask;
}
smokey Smo::SmoKey2key(smoKey smo_Key)
{
    return smo_Key & smoKeyMask;
}

u16 Smo::FolderDataAddress(Smo *smo) //old: FolderMethodAddress
{
    return smo->m_record.Data.Folder.FolderMethodAddress;
}
void Smo::setFolderDataAddress(Smo *smo, u16 value)
{
    smo->m_record.Data.Folder.FolderMethodAddress = value;
}

smokey Smo::Child_key(Smo *smo)
{
    return smo->m_record.Data.Folder.ChildKey & smoKeyMask;
}
void Smo::setChildKey(Smo *smo, smoKey value)
{
    smo->m_record.Data.Folder.ChildKey = value;
}

//FindOnkey() find a Smo with the key within its device and start from the smo branch; recursively!...
Smo *Smo::FindkeyLocaly(Smo* smo, smokey key)
{
    if (smo == NULL || smo->m_key == key) return smo;
    const DeviceInfo* target_device = smo->m_deviceInfo;
    const QList<Smo*> ls = smo->findChildren<Smo*>(QString(), Qt::FindDirectChildrenOnly);
    for (int i = 0; i < ls.count(); i++) {
        Smo* child = ls.at(i);
        if (child->m_deviceInfo == target_device) {
            if (Smo* found = Smo::FindkeyLocaly(child, key)) return found;
        }
    }
    return NULL;
}

/// \brief Smo::Findkey in a device!!!
/// \param devinfo
/// \param key
/// \return
Smo *Smo::Findkey(DeviceInfo *devinfo, smokey key)
{
    Smo* retv = Smo::FindkeyLocaly(devinfo->deviceItem(), key);
    if (!retv)
    {
        retv = new Smo(devinfo, key);
        if (!Smo::Read(retv))
        {
            //retv->deleteLater();
            delete retv;
            return NULL;
        }
    }
    return retv;
}

///use isNull() to check the result!!!
QString Smo::Smokey2name(DeviceInfo *devinfo, smokey key)
{
    Smo* smo = Smo::Findkey(devinfo, key);
    if (smo) return smo->ShortName();
    return QString();
}

QString Smo::SmoKey2NameOrStatus(DeviceInfo *devinfo, smoKey Key)
{
    if (Key == smoEndTableMarker) {
        return QString("SmoKey status: end table");
    }
    if (!Key) {
        return QString("SmoKey status: zero key");
    }
    smoKeyStatus_t keyt = {Key};
    switch (keyt.KeyStatus) {
    case SmoKeyStatus::SmoKeyStatus_Ok:
        return Smo::Smokey2name(devinfo, Key);
    case SmoKeyStatus::SmoKeyStatus_NotSelected:
        return QString("SmoKey & status: 0x%1(not selected)").arg(Key,0,16);
    default:
        break;
    }
    //for not defined statuses...
    return QString("SmoKey & status: 0x%1").arg(Key,0,16);
}

const char* Smo::RecNameField(Smo *smo)
{
    return smo->m_record.RecordName;
}

QString Smo::ShortName(Smo *smo)
{
    return smo->ShortName();
}

void Smo::SetShortName(Smo *smo, const QString& value)
{
//    smo->m_shortName = value; //now only for GDB helper!!!
    smo->setObjectName(value);
    if (smo->m_treeItem) smo->m_treeItem->setText(0, value); //update also tree item if exist!!!
}

int Smo::DecodeRecordName(Smo *smo)
{ // construct from Record...
    const char* nameField = Smo::RecNameField(smo);
    int field_max_len = iif(smo->IsName24(), smoFullNameMaxLength, smoShortNameMaxLength);
    int length_before_zero = strnlen(nameField, field_max_len);
    SetName_Parsed(smo, DecodeString(smo, nameField, length_before_zero));
    return length_before_zero;
}

void Smo::SetName_notParsed(Smo *smo, const QString& fullname)
{
    smo->m_fullname = fullname;
    SetShortName(smo, fullname);
    smo->m_inviter = QStringRef(&smo->m_fullname, 0, fullname.length());
    smo->m_formatter.clear(); //    smo->m_formatter = QStringRef();
}

void Smo::SetName_Parsed(Smo *smo, const QString& fullname)
{
    smo->m_fullname = fullname;
    SetShortName(smo, iif(fullname.isEmpty(),QString(),fullname.split(ShortNameDelimeter, QString::SkipEmptyParts)[0]));
//next: make inviter... std::sprintf((char*)InviteStr.data(), Record.RecordName);
    int fullname_len = fullname.length();
    int display_formatChar_index = 0;
    while (1) {
        display_formatChar_index = fullname.indexOf('%', display_formatChar_index) + 1; //+1 means next char index after %
        if (display_formatChar_index > 0)
        {
            if (fullname_len > display_formatChar_index && fullname[display_formatChar_index] == '%')
            {
                smo->m_fullname[display_formatChar_index++] = ' '; //replace double % to one
                continue;
            }
        }
        break;
    }
    smo->m_inviter = QStringRef(&smo->m_fullname, 0, iif(display_formatChar_index == 0, fullname_len, display_formatChar_index-1));
    if (display_formatChar_index == 0) {
        smo->m_formatter.clear();
    }
    else
    { //-1,+1: include % char
        smo->m_formatter = QStringRef(&smo->m_fullname, display_formatChar_index-1, fullname_len-display_formatChar_index+1).toLatin1();
    }
}

//end properties!!!

QString Smo::inviteString() const
{
    return m_inviter.toString();
}

int /*Q_INVOKABLE SMO::WriteStatus*/ Smo::writeText(const QString& wrtext)
{
    if (Smo::SmartBackTransform(this, wrtext) && Smo::calcTargetMemory(this)) {
        if (Smo::Call(this, targetMemory()))
        {
            emit dataChanged();
            return SMO::WriteOk;
        } else {
            return SMO::WriteIOError;
        }
    }
    emit dataInvalid(QVariant::fromValue(wrtext));
    return SMO::WriteBadValue;
}

int /*Q_INVOKABLE SMO::WriteStatus*/ Smo::writeData(QVariant data)
{
    if (Smo::calcTargetMemory(this, &data)) {
        if (Smo::Call(this, targetMemory()))
        {
            emit dataChanged();
            return SMO::WriteOk;
        } else {
            return SMO::WriteIOError;
        }
    }
    emit dataInvalid(data);
    return SMO::WriteBadValue;
}

int Smo::folderItemsCount() const
{
    return children().count();
}

const QByteArray& Smo::DisplayFormat(Smo* smo)
{
    return smo->m_formatter;
}

SmoAccess Smo::AccessState(Smo* smo)
{
    return smo->m_deviceInfo.isNull() ? SmoAccess::NoDataAccess : smo->m_deviceInfo->AccessState(smo);
}

const char* Smo::Status2ImageKey(Smo *smo)
{
    return Smo::Status2ImageKey(smo->m_mainStatusCode);
}
const char* Smo::Status2ImageKey(G8 mainStatus)
{
    if (mainStatus == G8::msg_Success || mainStatus == G8::msg_Executed)
    {
        return "ok";
    }
    if (mainStatus == G8::msg_ZeroString)
    {
        return "";
    }
    if (mainStatus == G8::msg_Play)
    {
        return "play";
    }
    if (mainStatus == G8::msg_Cancel)
    {
        return "cancel";
    }
    if (mainStatus == G8::msg_Forward)
    {
        return "next";
    }
    if (mainStatus == G8::msg_Save)
    {
        return "save";
    }
    if (mainStatus == G8::msg_Delete)
    {
        return "delete";
    }
    //next if no private icon then check GlobalStatusErrorCodes severity...
    int GlobalStatusErrorCodes_Severity = (int)mainStatus & Severity_mask;
    if (GlobalStatusErrorCodes_Severity == Severity_Error_gc)
    {
        return "error";
    }
    if (GlobalStatusErrorCodes_Severity == Severity_Warning_gc)
    {
        return "warning";
    }
    return "info";
}

bool Smo::IsDevice(Smo *smo)
{
    if (!smo->m_deviceInfo) return false;
    Smo* devsmo = smo->m_deviceInfo->deviceItem();
    return devsmo && devsmo == smo;
}

QString Smo::sMessage(Smo *smo)
{ //NOTE: m_sMessage doesn't valid without SMOStatus::SystemMessage flag...
    return iif(smo->m_flags & SmoFlags::FlagIOMessage, smo->m_sMessage, QString());
}
bool Smo::setsMessage(Smo *smo, const QString& value)
{
    smo->m_sMessage = value;
    smo->flagsSet(SmoFlags::FlagIOMessage, !value.isEmpty());
    return false;
}

Smo *Smo::ViewItem_add(Smo *smo)
{
    Smo* parent_smo = qobject_cast<Smo*>(smo->parent());
    if (parent_smo) {
        smo->m_treeItem = new TreeItem<Smo>(smo, smo->ShortName());
        if (parent_smo->m_treeItem) {
            parent_smo->m_treeItem->addChild(smo->m_treeItem);
        }
        Smo::SetIcon(smo, Smo::SelectIcon(smo));
        if (smo->headType() == SmoHeadType::SmoHeadType_Folder) smo->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
    }
    return smo;
}

Smo *Smo::ViewItem_update(Smo *smo)
{
    Smo::SetIcon(smo, Smo::SelectIcon(smo));
    return smo;
}

Smo *Smo::ViewItem_addOrUpdate(Smo *smo, bool isAddOrUpdate)
{
    return iif(isAddOrUpdate, ViewItem_add(smo), ViewItem_update(smo));
}

void Smo::SetIcon(Smo *smo, const char *iconName)
{
    smo->m_iconName = iconName;
    smo->setIcon(zvvlib::getIcon(iconName));
}

const char *Smo::SelectIcon(Smo *smo)
{
    const char* cImage; //QString cImage;
    if (smo->m_flags & (SmoFlags::FlagIOk))
    {
        if (smo->headType() == SmoHeadType::SmoHeadType_Folder)
        {
            if (smo->folderType() == SmoFolderType::LinkToDevice)
            {
                cImage = "device";
            }
            else
            {
                cImage = "folder";
            }
        }
        else if (Smo::IsControlAndType(smo))
        {
            cImage = "control";
        }
        else if (smo->headType() == SmoHeadType::SmoHeadType_Property) //smo->IsMethod)
        {
            cImage = "blocks";
        }
        else
        {
            cImage = "data";
        }
    }
    else
    {
        cImage = Smo::Status2ImageKey(smo);
    }
    return cImage;
}

Smo *Smo::parentSmo() const
{
    return qobject_cast<Smo*>(parent()); /*smo->m_parentSmo;*/
}

void Smo::setParentSmo(Smo *smo, Smo *parent_smo)
{
    smo->setParent(parent_smo);
    smo->m_parentSmo = parent_smo;
}

QRegExp Smo::nameDelimeter()
{
    return ShortNameDelimeter;
}

void Smo::setExpanded(bool toBeExpanded)
{
    if (m_treeItem) {
        m_treeItem->setExpanded(toBeExpanded);
    }
}

int Smo::childCount()
{
    return iif(m_treeItem, m_treeItem->childCount(), 0);
}

Smo *Smo::child(int index)
{
    if (m_treeItem) {
        TreeItem<Smo>* item = (TreeItem<Smo>*)m_treeItem->child(index);
        return iif(item, item->owner(), 0);
    }
    return 0;
}

void Smo::setToolTip(int index, const QString &tip)
{
    if (m_treeItem) {
        m_treeItem->setToolTip(index, tip);
    }
}

QString Smo::toolTip(int index)
{
    return iif(m_treeItem, m_treeItem->toolTip(index), QString());
}

void Smo::addChild(Smo *smo)
{
    smo->setParent(this); // setParentSmo(smo, this);
    TreeItem<Smo>* item = smo->treeItem();
    if (m_treeItem && item) {
        m_treeItem->addChild(item);
    }
}

Smo *Smo::takeChild(int index)
{
    if (m_treeItem) {
        return m_treeItem->takeChild_returnOwner(index);
    }
    return 0;
}

QList<Smo*> Smo::takeChildren()
{
    if (m_treeItem) {
        return m_treeItem->takeChildren_returnOwners();
    }
    return QList<Smo*>();
}

void Smo::setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy policy)
{
    if (m_treeItem) {
        m_treeItem->setChildIndicatorPolicy(policy);
    }
}

void Smo::setIcon(const QIcon &icon, int index)
{
    if (m_treeItem) {
        m_treeItem->setIcon(index, icon);
    }
}

QIcon Smo::icon(int index)
{
    return iif(m_treeItem, m_treeItem->icon(index), QIcon());
}

void Smo::setHidden(bool toBeHidden)
{
    if (m_treeItem) {
        m_treeItem->setHidden(toBeHidden);
    }
}

QString Smo::text(int index)
{
    return iif(m_treeItem, m_treeItem->text(index), QString());
}

QByteArray* Smo::targetMemory() const
{
    return (QByteArray*)&m_targetMemory;
}

G8 Smo::mainStatusCode() const
{
    return m_mainStatusCode;
}

QQmlListProperty<Smo> Smo::smoChildren()
{
    return QQmlListProperty<Smo>(this, this, &Smo::childCountQml, &Smo::childAtQml);
}

int Smo::childCountQml() const
{
    return children().count();
}

Smo *Smo::childAtQml(int index) const
{
    return qobject_cast<Smo*>(children().at(index));
}

DEWidget *Smo::expandedWidget() const
{
    return m_expandedWidget;
}

void Smo::setExpandedWidget(DEWidget *expandWidget)
{
    m_expandedWidget = expandWidget;
}

DEWidget *Smo::collapsedWidget() const
{
    return m_collapsedWidget;
}

void Smo::setCollapsedWidget(DEWidget *collapsedWidget)
{
    m_collapsedWidget = collapsedWidget;
}

QVariant Smo::dataValue()
{
//    if (m_flags & (SmoFlags::Ok | SmoFlags::IOData)) {
    if (m_targetValue.isValid()) {
        debug_func("buffered value:" << m_targetValue);
        return m_targetValue;
    }
    debug_func("will be (re)read...");
    return readData();
}

QVariant Smo::readData()
{
    QByteArray param; // no data: read target value
    if (Call(this, &param)) {
        debug_func("successful???: " << m_targetValue); //TODO: from QByteArray to m_targetValue
        emit dataChanged();
    }
    else {
        Log::addWarning(Smo::ShortName(this).append(": data not read!!!"));
        m_targetValue.clear();
    }
    return m_targetValue;
}

void Smo::setTargetValueAndCall(QVariant data)
{ //called by external objects...
    if (Call(this, data)) {
        m_targetValue.swap(data);
//            debug_func("e mit data Changed for:" << m_targetValue);
        emit dataChanged();
    }
    else {
        Log::addWarning(Smo::ShortName(this).append(": data not writed!!!"));
    }
}

/// \brief Smo::setTargetValueClear
/// \return true if value changed!!!
bool Smo::setTargetValueClear()
{
    if (m_targetValue.isValid()) {
        m_targetValue.clear();
        return true;
    }
    return false;
}

/// \brief Smo::setTargetValueNull
/// \return true if value changed!!!
bool Smo::setTargetValueNull()
{
    if (!m_targetValue.isNull()) {
        m_targetValue = QVariant::fromValue(QByteArray());
        return true;
    }
    return false;
}

#define updateTargetValue_helper_on_type(T) { \
    if ((size_t)m_targetMemory.length() < sizeof (T)) { \
        m_targetValue.clear(); \
        return G8::err_DataLength; \
    } else { \
        m_targetValue = qVariantFromValue<T>(*(T*)m_targetMemory.constData()); \
        return G8::msg_Success; \
    } \
}

#define updateTargetValue_helper_on_uncompleted_type(T, size) { \
    if (m_targetMemory.length() < size) { \
        m_targetValue.clear(); \
        return G8::err_DataLength; \
    } else { \
        T value = DeserializeUncompletedNumber<T>(m_targetMemory.constData(), size); \
        m_targetValue = qVariantFromValue<T>(value); \
        return G8::msg_Success; \
    } \
}

#define updateTargetValue_helper_on_pointer_type(T) { \
    if ((size_t)m_targetMemory.length() < sizeof (T)) { \
        m_targetValue.clear(); \
        return G8::err_DataLength; \
    } else { \
        m_targetValue = qVariantFromValue<void*>((void*)(long)(*(T*)m_targetMemory.constData())); \
        return G8::msg_Success; \
    } \
}

#define updateTargetValue_helper_on_sized_data(size) { \
    if (m_targetMemory.length() < size) { \
        m_targetValue.clear(); \
        return G8::err_DataLength; \
    } else { \
        m_targetValue = qVariantFromValue<QByteArray>(m_targetMemory); \
        return G8::msg_Success; \
    } \
}

/// \brief Smo::updateTargetValue
/// \return G8
static QString loginString(byte lug)
{
    if (lug != LoginedUserGroup::NoLoginedUser) {
        return QString::number((uint)lug & LoginedUserGroup_mask).repeated(20); //TODO: make same length with the hash algo (i.e. sha1,sha256,sha512)
    }
    return QString();
}

G8 Smo::updateTargetValue()
{
    G8 retv = G8::msg_Success;
    SmoDataType smoDT = dataType();
    int targetMemLen = m_targetMemory.length();
    if (Smo::IsControlAndType(this))
    { //return status...
        switch (smoDT) {
        case SmoDataType::ControlLogin: /*datasize: answer:3, request:1+ */
            if (targetMemLen == 3) {
                m_targetValue = QVariant::fromValue(loginString(((LoginAndGroup_t*)m_targetMemory.constData())->LoginGroup));
                return G8::msg_Success;
            }
            break;
        case SmoDataType::ControlPassLine: /*datasize: answer:1, request:1+ */
            if (targetMemLen == 1) {
                m_targetValue = QVariant::fromValue(loginString(*(byte*)m_targetMemory.data()));
                return G8::msg_Success;
            }
            break;
        case SmoDataType::ControlSelectItem: /*datasize: 2; SelectTdi() is a handler for this */
            if (targetMemLen == 2) {
                m_targetValue = QVariant::fromValue(Smo::SmoKey2NameOrStatus(m_deviceInfo.data(), *(smokey*)m_targetMemory.data()));
                return G8::msg_Success;
            }
            break;
        default:
            if (targetMemLen != 0)
            { //TODO:it
                m_targetValue = QVariant::fromValue(QString("TODO: CtrlBytes (count:%1): 0x%2").arg(targetMemLen).arg(QString(m_targetMemory.toHex())));
                return G8::msg_Success;
            }
            break;
        }
        retv = G8::err_DataLength;
    }
    else if (smoDT == SmoDataType::Pointer16) /*SmoPointer*/
    {
        if (targetMemLen == 2) {
            m_targetValue = qVariantFromValue<smokey>(*(smokey*)m_targetMemory.data()); //Smo::SmoKey2NameOrStatus(smo->m_deviceInfo.data(), *(smokey*)smo->m_targetMemory.data());
            return G8::msg_Success;
        }
        retv = G8::err_DataLength;
    }
    else if (folderType() == SmoFolderType::LinkToDevice)
    {
        retv = G8::err_Unsupported; //Unsupported yet!!!
//        if (!m_targetValue.isNull() && m_targetValue.canConvert<Smo>())
//        {
//            const Smo &netDevice = smo->m_targetValue.value<Smo>();
//            if (Smo::sMessage((Smo*)&netDevice).isEmpty())
//            {
//                return DisplayInvite((Smo*)&netDevice);
//            }
//            return Smo::sMessage((Smo*)&netDevice);
//        }
//        else if (targetMemLen == 1)
//        {
//            if (Smo::DisplayFormat(smo).isNull() || Smo::DisplayFormat(smo).indexOf('x') != -1 || Smo::DisplayFormat(smo).indexOf('X') != -1)
//            { //for hex...
//                return QString("0x%1").arg(QString(smo->m_targetMemory.toHex()));
//            }
//            return QString::number(*(byte*)smo->m_targetMemory.data());
//        }
//        return Smo::sMessage(smo);
    }
    else if (smoDT == SmoDataType::ControlPassLine)
    {
        if (m_deviceInfo->IsUserAccepted())
        {
            m_targetValue = QVariant::fromValue(QByteArray(16,'*'));
        }
        else
        {
            setTargetValueNull();
        }
        return msg_Success;
    }
    else
    {
        if (Smo::IsStringAndType(this))
        { //To do: encoding selectors!!!
            if (targetMemLen == 0) {
                setTargetValueNull();
            }
            else {
                int resultLength = smoDT == SmoDataType::ZeString ? m_targetMemory.indexOf('\0') : (int)smoDT & smoDTStringLengthMask;
                resultLength = resultLength == 0 ? targetMemLen : resultLength;
                m_targetValue = QVariant::fromValue(Smo::DecodeString(this, m_targetMemory.data(), resultLength));
            }
            return msg_Success;
        }
        else if (Smo::IsBitAndType(this))
        {
            uint bitPosition = (uint)dataType() & smoDTBitPositionInByte_bm;
            if ((uint)targetMemLen >= ((bitPosition + 8) / 8))
            {
                m_targetValue = qVariantFromValue<bool>(!!((*(byte*)m_targetMemory.data()) & (uint)(1 << bitPosition)));
                return msg_Success;
            }
            retv = G8::err_DataLength;
        }
        else
        { //next for 0..15 data types...
            switch (smoDT)
            { // commented options unused yet...
            case SmoDataType::S8:
                updateTargetValue_helper_on_type(s8);
            case SmoDataType::U8:
                updateTargetValue_helper_on_type(u8);
            case SmoDataType::S16:
                updateTargetValue_helper_on_type(short);
            case SmoDataType::U16:
                updateTargetValue_helper_on_type(ushort);
            case SmoDataType::RGB:
            case SmoDataType::U24:
                updateTargetValue_helper_on_uncompleted_type(u32, 3);
            case SmoDataType::S32:
                updateTargetValue_helper_on_type(s32);
            case SmoDataType::U32:
                updateTargetValue_helper_on_type(u32);
            case SmoDataType::Float32:
                updateTargetValue_helper_on_type(float);
            case SmoDataType::DateBSD32: //TODO:
                if (m_targetMemory.length() < 4) {
                    m_targetValue.clear();
                    return G8::err_DataLength;
                } else {
                    m_targetValue = qVariantFromValue<QDate>(QDate::fromJulianDay(*(u32*)m_targetMemory.constData()));
                    return G8::msg_Success;
                }
            case SmoDataType::RealTime32: //TODO:
//                if (targetMemLen < 4) return QString();
//                return Realtime32::ToString(smo->m_targetMemory.toUInt());
                updateTargetValue_helper_on_type(u32);
            case SmoDataType::Decimal64:
            case SmoDataType::S64:
                updateTargetValue_helper_on_type(s64);
            case SmoDataType::U64:
                updateTargetValue_helper_on_type(u64);
            case SmoDataType::Float64:
                updateTargetValue_helper_on_type(f64);
            case SmoDataType::DateTime64:
//                if (targetMemLen < 8) return QString();
//                return QDateTime::fromMSecsSinceEpoch(smo->m_targetMemory.toLongLong()).toString();
                if (m_targetMemory.length() < 8) {
                    m_targetValue.clear();
                    return G8::err_DataLength;
                } else {
                    m_targetValue = qVariantFromValue<QDateTime>(QDateTime::fromMSecsSinceEpoch(*(s64*)m_targetMemory.constData()));
                    return G8::msg_Success;
                }
            case SmoDataType::Pointer16:
                updateTargetValue_helper_on_pointer_type(u16);
            case SmoDataType::Pointer32:
                updateTargetValue_helper_on_pointer_type(u32);
            case SmoDataType::Pointer64:
                updateTargetValue_helper_on_pointer_type(u64);
            case SmoDataType::U128:
                updateTargetValue_helper_on_sized_data(16);
            case SmoDataType::SizedU8Data:
            case SmoDataType::SizedU32Data:
                m_targetValue = qVariantFromValue<QByteArray>(m_targetMemory);
                return G8::msg_Success;
            case SmoDataType::Enumerator:
                updateTargetValue_helper_on_type(s32);
            case SmoDataType::Void:
            case SmoDataType::Undefined:
                break;
            default:
                updateTargetValue_helper_on_sized_data(m_targetMemory.length());
            }
        }
    }
    m_targetValue.clear();
    return retv;
}

DEPlugin *Smo::plugin() const
{
    return m_plugin;
}

void Smo::setPlugin(DEPlugin *plugin)
{
    m_plugin = plugin;
}

DeviceInfo* Smo::deviceInfo() const
{
    return m_deviceInfo;
}

smokey Smo::key() const
{
    return m_key;
}

const char *Smo::iconName() const
{
    return m_iconName;
}

bool Smo::isCallSuccessful() const
{
    return m_flags & SmoFlags::FlagIOk;
}

void Smo::setSuccessful(bool isSuccessful)
{
    flagsSet(SmoFlags::FlagIOk, isSuccessful);
}

bool Smo::isExpanded() const
{
    return iif(m_treeItem, m_treeItem->isExpanded(), false);
}

TreeItem<Smo> *Smo::treeItem() const
{
    return m_treeItem;
}

void Smo::setTreeItem(TreeItem<Smo> *treeItem)
{
    m_treeItem = treeItem;
}

//void Smo::setTreeItem(Smo *smo, TreeItem *treeItem)
//{
//    smo->setTreeItem(treeItem);
//}

bool Smo::CallEnvironment_check(Smo *smo)
{
    smo->m_targetAccessTime = QDateTime::currentDateTime();
//    smo->m_flags = _AllClear;
    if (smo->m_deviceInfo == NULL)
    {
//        Smo* init = Smo::DeviceInfo_init(smo);
//        if (init == NULL) {
//            return Smo::ClearTargetData_and_setCallStatus(smo, "device info init fails");
//        }
        return Smo::ClearTargetData_and_setCallStatus(smo, "device info is NULL");
    }

//    if (smo->parent() != smo->m_deviceInfo) {
//        smo->setParent(smo->m_deviceInfo);
//    }

    if (smo->m_deviceInfo->HWPort() == NULL)
    {
        return Smo::ClearTargetData_and_setCallStatus(smo, "device info port data is not init");
    }
    if (!smo->m_deviceInfo->HWPort()->isOpen())
    {
        return Smo::ClearTargetData_and_setCallStatus(smo, "device port has not opened");
    }
    return true;
}

Smo *Smo::DeviceInfo_init(Smo *smo)
{
    return smo; //???
}

bool Smo::ClearTargetData(Smo *smo)
{
    smo->m_targetMemory.clear();
    smo->setTargetValueClear();
    return false;
}

//bool Smo::ClearTargetData_and_setCallStatus(Smo *smo, SmoTextualErrors code)
//{
//    return Smo::ClearTargetData_and_setTextualStatus(smo, QMetaEnum::fromType<SmoTextualErrors>().valueToKey((int)code));
//}
bool Smo::ClearTargetData_and_setCallStatus(Smo *smo, const QString& statusMessage)
{
    return Smo::ClearTargetData_and_setCallStatus(smo, SmoFlags::FlagIOStatus, statusMessage);
}
bool Smo::ClearTargetData_and_setCallStatus(Smo *smo, SmoFlags callStatus, const QString& statusMessage)
{
    smo->m_mainStatusCode = G8::err_TextualError;
    smo->flagsSetClear(callStatus, SmoFlags::FlagIOk);
    Smo::setsMessage(smo, statusMessage);
    ClearTargetData(smo);
    emit smo->ioError();
    return false;
}

Smo *Smo::RefreshLinkedDevice(Smo *deviceSmo)
{ //NOT TESTED!
    if (deviceSmo != deviceSmo->m_deviceInfo->deviceItem())
    { //skip(exclude) a first gate...
        if (deviceSmo->m_targetValue.isNull()
         || (int)deviceSmo->m_targetValue.type() != QMetaType::type("Smo")
         || !deviceSmo->m_targetValue.value<Smo>().isCallSuccessful())
        {
            if (!Smo::Call(deviceSmo))
            {
//                Smo::ClearTargetData_and_setCallStatus(targetSmo, parentSmo->SmoStatus, QObject::tr("Device.Call() error: %1").arg(Smo::StatusDescript(parentSmo)));
                return NULL;
            }
        }
//        *deviceSmo = deviceSmo->m_targetValue.value<Smo>(); //TODO:it
        return deviceSmo; //TODO: &();
    }
    return NULL;
}

/// \brief Smo::RefreshChildren
/// \param parentSmo
/// \param smo_key : if > 0 start from this key; if 0 then start from first child!
/// \param smo_name : if defined then stop load at this short name else load whole folder!
/// \return
Smo *Smo::RefreshChildren(Smo *parentSmo, smokey smo_key, const QString smo_name)
{ //ver2 from 201704!!
    if (!(parentSmo->m_flags & SmoFlags::FlagIOk) && !Smo::Read(parentSmo)) {
        return NULL;
    }
    if (parentSmo->folderType() == SmoFolderType::LinkToDevice) //NOT TESTED!
    { //I guess it's for auto reading Net devices...
        Smo* devsmo = RefreshLinkedDevice(parentSmo);
        if (devsmo) {
            parentSmo = devsmo;
        }
    }
    parentSmo->flagsClear(SmoFlags::FlagHasInvisibleChild | SmoFlags::FlagAllChildrenRead | SmoFlags::FlagRefreshedWithError);

    const QList<Smo*> exists = parentSmo->findChildren<Smo*>(QString(), Qt::FindDirectChildrenOnly);
    int child_count;
    if (smo_key == 0) {
        smo_key = Smo::Child_key(parentSmo);
        if (smo_key == 0 && Smo::IsDevice(parentSmo)) {
            smo_key = parentSmo->m_deviceInfo->FirstSmoKey();
        }
//        child_count = parentSmo->childrenCount(); //for init the list of items to remove...
        child_count = exists.count();
    }
    else {
        child_count = 0; //no init the list of items to remove(i.e. do not remove anything!!!)...
    }
    smo_key &= smoKeyMask;

    int istart = 0;
//    static auto SmoOnKeyFinder = [&smo_key](Smo* s) { return s->m_key == smo_key;};
    QVector<int> foundlist(child_count, 0);

    Smo *smo = NULL;
    while (smo_key)
    {
        int idx = istart;
        for (; idx < exists.count(); idx++) { //NOTE: do not use foreach with QList!!!
            if (exists.at(idx)->m_key == smo_key) {
                break;
            }
        }
        const bool isSmoNotInTree = idx >= exists.count();
        if (isSmoNotInTree) {
            smo = Smo::NewChildSmo(parentSmo, smo_key);
        }
        else {
            if (idx < child_count) foundlist[idx] = 1;
            if (idx == istart) istart++;
            smo = exists.at(idx);
        }
        if (!Smo::Read(smo)) { //read then check flags...
            debug_func(QString("Smo read unsuccess key: 0x%1").arg(smo->key(),0,16));
        }
        if (!(smo->m_flags & SmoFlags::FlagIOk)) {
            if ((smo->m_flags & SmoFlags::FlagIntegrityError) || ((smo->m_flags & SmoFlags::FlagIOMessage) && smo->m_mainStatusCode == wrn_TheEnd))
            { //i guess it's the end of the folder...
                smo->deleteLater();
                smo = 0;
                parentSmo->flagsSet(SmoFlags::FlagAllChildrenRead);
                emit parentSmo->allChildrenRefreshed();
                debug_func(parentSmo << "AllChildrenRead set!!!");
                break;
            }
//            if (smo->m_flags & (SMOStatus::IOError | Smo::SystemMessage))
//            { // return without remove anything (mark the smo as error)...
                parentSmo->flagsSet(SmoFlags::FlagRefreshedWithError);
                return ViewItem_addOrUpdate(smo, isSmoNotInTree);
//            }
        }
        smo_key = Smo::nextSmo_key(smo);
        if (Smo::AccessState(smo) == SmoAccess::Invisible)
        { //TODO: check Invisible tracking...
            parentSmo->flagsSet(SmoFlags::FlagHasInvisibleChild);
            ViewItem_addOrUpdate(smo, isSmoNotInTree);
            smo->setHidden(true);
        }
        else
        { // add the child to parent if it's not in tree yet...
            ViewItem_addOrUpdate(smo, isSmoNotInTree);
            if (!smo_name.isEmpty() && Smo::ShortName(smo) == smo_name) {
                // return without remove anything...
                return smo;
            }
        }
    } //while (smoptr)
    int i = child_count;
    while (--i >= 0) { //start from last index...
        if (foundlist.value(i, -1) == 0) {
            Smo* token_smo = exists.at(i); /*parentSmo->takeChild(i);*/
            if (token_smo) {
                token_smo->deleteLater();
            }
        }
    }
    return smo;
}

//nextSmo_key() returns 0 if the end is reached!!!
smokey Smo::nextSmo_key(Smo *smo)
{
    u32 retValue;
    if (Smo::IsEndTable(smo))
    {
        return 0; //std::numeric_limits<u16>::max();
    }
    else
    {
        if (!(smo->m_flags & SmoFlags::FlagIntegrityError) && smo->IsName24())
        {
            retValue = smo->m_key + 32;
        }
        else
        {
            retValue = smo->m_key + 16;
        }
        if (retValue > 0xFFF0) return 0;
    }
    return (smokey)retValue;
}

/* DEPRECATED, use Smo's assign operator!!!
void Smo::Update(Smo *targetSmo, Smo *sourceSmo)
{
    Record_copy(targetSmo, sourceSmo);
    SetName_Parsed(targetSmo, sourceSmo->m_fullname);
    targetSmo->key = sourceSmo->key;
    targetSmo->deviceInfo = sourceSmo->deviceInfo;
    targetSmo->VirtualParent = sourceSmo->VirtualParent;

    targetSmo->MainStatusCode = sourceSmo->MainStatusCode;
    //targetSmo->ExtendedStatusCode = sourceSmo->ExtendedStatusCode;

    Smo::setcMessage(targetSmo, Smo::cMessage(sourceSmo));

    targetSmo->SmoStatus = sourceSmo->SmoStatus;

//    targetSmo->AppExtention = sourceSmo->AppExtention;

    targetSmo->VirtualKey = sourceSmo->VirtualKey;

    targetSmo->m_targetMemory = sourceSmo->m_targetMemory;
    targetSmo->m_targetValue = sourceSmo->m_targetValue;

    targetSmo->TargetAccessTime = sourceSmo->TargetAccessTime;
    targetSmo->TargetAnswerTime = sourceSmo->TargetAnswerTime;
}
*/

//returns true if ok; false for if virtual smo isn't ok or frame overload!!!
bool Smo::SmoBus_frame_make(Smo *smo, QByteArray *retv, QByteArray *data)
{
    QByteArray cVirtualPath;
    int netPathLength = smo->m_deviceInfo->NETPath().length(); //cNETPath->length();
    int dataLength = 0;
    if (data != NULL)
    {
        dataLength = data->length();
        if (!SmoBus_virtualPath_make(smo, &cVirtualPath)) {
            return false;
        }
    }
    const int packetDataLength = dataLength + cVirtualPath.length() + netPathLength;
    int totalLength = offsetof(SmoBusFrame_t, Packet.Data) + packetDataLength + sizeof(u16); //u16=FrameCRC
    if (totalLength > std::numeric_limits<byte>().max())
    { //TODO: divide by packets!!!
        //will be overwritten later: smo->MainStatusCode = G8::err_DataLength;
        return false;
    }
    retv->resize(totalLength);
    SmoBusFrame_t *frame = (SmoBusFrame_t*)retv->data();
    //fill the Frame...
    frame->FrameType         = FrameType_SmoBus;
    frame->DestAddress       = SmoBus_FirstPhAddress;
    frame->SourceAddress     = SmoBus_MasterAddress;
    frame->TotalLength       = totalLength;
    frame->Packet.packID     = smo->m_deviceInfo->HWPort()->txCounter();
//    frame->Packet.packTag    = SmoBus_offseter_get(); //    frame->Offseter          = SmoBus_offseter_get();
    frame->Packet.DataLength = packetDataLength;

    u16 packetKey = smo->m_key;
    if (packetKey == 0 && data != NULL && Smo::IsDevice(smo)) packetKey = smo->m_deviceInfo->FirstSmoKey();

    if (data != NULL) //DataFieldLength != 0 ||
    {
        if (cVirtualPath.length() > 0)
        {
            packetKey |= ExecuteGroupFlag_bm;
        }
        else
        {
            packetKey |= ExecuteFlag_bm;
        }
    }
    if (netPathLength > 0) {
        retv->replace(offsetof(SmoBusFrame_t, Packet.packKey), netPathLength, smo->m_deviceInfo->NETPath());
        retv->replace(offsetof(SmoBusFrame_t, Packet.packKey) +netPathLength, sizeof(packetKey), (const char *)&packetKey, sizeof(packetKey));
    }
    else {
        frame->Packet.packKey.Key = packetKey;
    }

    if (dataLength > 0)
    {
        retv->replace(offsetof(SmoBusFrame_t, Packet.Data) + netPathLength, dataLength, data->data());
    }
    if (cVirtualPath.length() > 0)
    {
        retv->replace(offsetof(SmoBusFrame_t, Packet.Data) + netPathLength + dataLength, cVirtualPath.length(), cVirtualPath);
    }
    SmoBus_shrinkAndCalcCRC(retv);
    return true;
}

///return number of frame bytes have been calculated!!!
void Smo::SmoBus_shrinkAndCalcCRC(QByteArray *frame)
{
    int dataOffset = frame->length() - (SmoBusCRCLength +1);
    char *frameDataPtr = frame->data();

    while (frameDataPtr[dataOffset] == 0 && dataOffset >= (int)SmoBusMinRequestLength) {--dataOffset;}
    if (frameDataPtr[offsetof(SmoBusFrame_t, TotalLength)] != dataOffset + (SmoBusCRCLength +1)) {
        frameDataPtr[offsetof(SmoBusFrame_t, TotalLength)] = dataOffset + (SmoBusCRCLength +1); //+1: offset2length;
        frame->resize(dataOffset + (SmoBusCRCLength +1)); //+1: offset2length
    }
    ::CRC16_append(CRC16_INIT, frameDataPtr, dataOffset + 1);
#if DEBUG
#else
    InOffsetData(frameDataPtr, 0, dataOffset); //PARAMS does not tested!!!
    // FrameEncryptor(ref bFrame, totalLength, Marshal.SizeOf(typeof(SmoNetFrameHeader_t)), netFrame.smoHeader.FrameOffseter);
#endif
}

///SmoBus_virtualPath_make() returns true on success!!!
bool Smo::SmoBus_virtualPath_make(Smo *smo, QByteArray *cVirtualPath)
{ //called recursively...
    if (smo->m_virtualParent)
    {
        Smo *oVirtualParent = smo->m_virtualParent;
        if (oVirtualParent->isCallSuccessful())
        {
            smoKey groupCall_Key = (smoKey)((oVirtualParent->m_key & smoKeyMask));
            if (cVirtualPath->length() > 0)
            { //ExecuteGroupFlag_bm means that this item is not last in call order!!!
                groupCall_Key |= ExecuteGroupFlag_bm;
            }
//deprecated,from 20160328
//            int nParentDataLen = oVirtualParent->m_targetMemory.length();
//            if (nParentDataLen > 0)
//            {
//                groupCall_Key |= ExecuteGroupWithDataFlag_bm;
//                cVirtualPath->append(oVirtualParent->m_targetMemory.data(), nParentDataLen);
//                cVirtualPath->append((byte)nParentDataLen);
//            }
            cVirtualPath->append(ArrayFromValue<smoKey>(&groupCall_Key)); // Array.Copy(BitConverter.GetBytes(groupCall_key), 0, cVirtualPath, nVirtualPathNewLength - sizeof(u16), sizeof(u16));
            if (!SmoBus_virtualPath_make(oVirtualParent, cVirtualPath))
            { //call recursive!!!
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

//bool Smo::SmoBus_LowLevelCall(Smo* smo, const QByteArray& netFrame)
//{
//    TDIPort* tdiPort = smo->m_deviceInfo->HWPort();

////    debug_tree_info(QString("Send %1 (length: %2) : %3").arg(tdiPort->name()).arg(netFrame.length(), 3, 10, QChar('0')).arg(Log::HexToString(netFrame)));
////    if (tdiPort->write(netFrame) != netFrame.length())
//    if (!tdiPort->dataPort(netFrame))
//    {
//        return Smo::ClearTargetData_and_setCallStatus(smo, SMOStatus::IOError, tdiPort->errorString());
//    }
////    smo->m_targetAccessTime = QDateTime::currentDateTime();

//    if (!SmoBus_waitAnswer(smo))
//    {
//        return ClearTargetData(smo);
//    }
////    smo->TargetAnswerTime = QDateTime::currentDateTime();
//    // Parse the smo...
////    enum SmoBusKeyFlags packType = smo->flags.PackType;

////    if (packType == SmoBusKey_retRecord)
////    { // for reading refill the smo...
////        bool IsParseOk = Smo::SelfParser(smo);
////        //debug.Assert(smo->ShortName == "Vset,mV");
////        debug_tree_info("Smo: " + iif(Smo::ShortName(smo).isEmpty(), "<anonymous>", Smo::ShortName(smo)) + iif(IsParseOk, "", QObject::tr("; error: %1; %2").arg(smo->SmoStatus).arg(Smo::sMessage(smo))));
////        if (!IsParseOk) return false;
////    }
////    else if (packType == SmoBusKey_retDevice)
////    {
////        bool IsDeviceInitOk = Smo::ParseAndFillDeviceInfo(smo);
////        Smo::setsMessage(smo, iif(IsDeviceInitOk,
////                                  smo->m_targetValue.value<Smo>().m_deviceInfo->DeviceName(),
////                                  "Device error: " /*+ Smo::sMessage(smo->m_targetValue.value<Smo>())*/));
////        if (!IsDeviceInitOk) return false;
////    }
////    else
////    {
//        //Log::addInfo("SmoBus_LowLevelCall(): "+smo->Text());
////        qDebug() << smo->ShortName();
////        Assert(smo->ShortName().indexOf("config") != 0);
////    }
//    //debug.Assert(smo->ShortName == "ЖурналАБ");
////    qDebug() << smo->ShortName();
//    smo->SmoStatus = SMOStatus::Ok;
//    return true;
//}

bool Smo::SmoDevice_TdiInfo_readAndCheck(Smo *smo)
{
    if (!DeviceInfo::ReadTdiInfo(smo->m_deviceInfo)) {
        return Smo::IntegrityCheckFailed(smo);
    }
    if (!DeviceInfo::IsDeviceVersionSupported(DeviceInfo::TdiInfo_version(smo->m_deviceInfo))) {
        Smo::setsMessage(smo, QStringLiteral("Device version has not supported!\n"));
        return Smo::IntegrityCheckFailed(smo);
    }
    ushort mib = DeviceInfo::TdiInfo_charset(smo->m_deviceInfo);
    QTextCodec* newCodec = iif(mib, QTextCodec::codecForMib(mib), NULL);
    if (!newCodec) {
        Smo::setsMessage(smo, QStringLiteral("Device character set undefined!\n"));
        return Smo::IntegrityCheckFailed(smo);
    }
    smo->m_deviceInfo->setTextCodec(newCodec);
    return true;
}

byte Smo::SmoBus_offseter_get()
{
    byte nOffseter = 0; //1=debug only the offseter, 0
#if DEBUG
#else
    while (nOffseter == 0)
    {
        nOffseter = (byte)(QDateTime::currentMSecsSinceEpoch() & 0xFF);
    }
#endif
    return nOffseter;
}

/// Fills: m_sMessage for msg_TextualMessage
/// return error code or -1 for undefined error!
int Smo::Smo_status_parser(Smo *smo)
{
    const int statusLength = smo->m_targetMemory.length();
    const char* data = smo->m_targetMemory.constData();
    //Parse extended status data (for Ack frames no multy status supported!!!)...
    switch ((int)smo->m_mainStatusCode)
    {
    case G8::msg_ExtendedDWordCode:
    case G8::msg_WindowsAPICode:
        if (statusLength >= MainStatus_ExtendedDWord_length) //5: mainStatusCode + WinAPI code
        {
            return (int)(*(s32*)(data +1));
        }
        break;
    case G8::msg_ExtendedByteCode:
        if (statusLength >= MainStatus_ExtendedByte_length) //2: mainStatusCode + 1byte of extended code
        {
            return (int)((ushort)(((ushort)G8::msg_ExtendedByteCode) << 8 | *(data +1)));
        }
        break;
    case G8::msg_ExtendedWordCode:
        if (statusLength >= MainStatus_ExtendedWord_length) //3: mainStatusCode + 2byte of extended code
        {
            UInt32 uCodeResult = (UInt32)*(u16*)(data +1);
            return (int)((UInt32)(uCodeResult | (UInt32)(((UInt32)G8::msg_ExtendedWordCode) << 16)));
        }
        break;
    case G8::msg_TextualMessage:
    case G8::wrn_TextualWarning:
    case G8::err_TextualError:
        Smo::setsMessage(smo, QString::fromUtf8(data +1, iif(statusLength, statusLength -1, 0)));
        return -((int)smo->m_mainStatusCode);
    default :
        break;
    }
    return -1;
}

QString Smo::StatusDescript(Smo *smo)
{
    switch (smo->m_mainStatusCode)
    {
        case G8::msg_ExtendedByteCode:
        case G8::msg_ExtendedWordCode:
        case G8::msg_ExtendedDWordCode:
        case G8::msg_WindowsAPICode:
            //TODO: seek descript of the code from a lib
            return QString("Extended code: 0x%1").arg(Smo_status_parser(smo), 0, 16);
//qt: case G8::msg_WindowsAPICode: //return Exceptions.WinAPICode2Descript(smo->ExtendedStatusCode);
        case G8::msg_TextualMessage:
        case G8::wrn_TextualWarning:
        case G8::err_TextualError:
            return Smo::sMessage(smo);
        default:
            if (smo->m_mainStatusCode == G8::msg_Success && smo->m_deviceInfo == NULL)
            {
                if (smo->parentSmo())
                {
                    return "Device info is not initilized for " + Smo::ShortName(smo);
                }
                return "parent\\" + Smo::StatusDescript(smo->parentSmo()); //recursive call!!! // return "parent\\" + Smo::StatusDescript((Smo*)smo->parent()); //recursive call!!!
            }
            return SmoLib::MainStatusCode2Descript(smo->m_mainStatusCode);
    }
    return QString(/*"empty"*/); // not used!
}

void Smo::UserGroup_set(Smo *smo, LoginedUserGroup newGroup)
{
    smo->m_deviceInfo->SetupUserInfo(newGroup);
}

//FindUserGroup() returns group number(0..3) or -1 if not found!!!
int Smo::FindUserGroup(Smo *smo, bool forVisibleOrInvisible)
{
    byte secByte = smo->SecByte().all;
    int founded_lastGroup = -1;
    for (int group = 0; group < smoTotalGroupsCount; group++)
    {
        bool isInVisible = ((SmoAccess)(secByte & SmoAccess_mask) == SmoAccess::Invisible);
        if (forVisibleOrInvisible)
        {
            if (!isInVisible) founded_lastGroup = group;
        }
        else {
            if (isInVisible) founded_lastGroup = group; //& smoLoginAccepted_bm;
        }
        secByte >>= 2;
    }
    return founded_lastGroup;
}

bool Smo::AccessState_and_userPassStatus_forecast(Smo *smo, bool IsVisibleInDevice)
{ //TODO: read it from the device instead forecast!!!
    SmoAccess smoAccessInHost = Smo::AccessState(smo);
    if (IsVisibleInDevice)
    {
        if (smoAccessInHost == SmoAccess::Invisible) // || !smo->deviceInfo->IsUserAccepted()
        { // try to solve userGroup...
            int lastVisibleGroup = Smo::FindUserGroup(smo, true);
            if (lastVisibleGroup < 0) return false;
            Smo::UserGroup_set(smo, (enum LoginedUserGroup)lastVisibleGroup); //LoginedUserGroup::NoLoginedUser
        }
    }
    else
    { //for param == InVisible ...
        if (smoAccessInHost != SmoAccess::Invisible) // || !smo->deviceInfo->IsUserAccepted()
        { // try to solve userGroup...
            int lastVisibleGroup = Smo::FindUserGroup(smo, false);
            if (lastVisibleGroup < 0) return false;
            Smo::UserGroup_set(smo, (enum LoginedUserGroup)lastVisibleGroup); //LoginedUserGroup::NoLoginedUser
        }
    }
    return true;
}

bool Smo::IntegrityCheckFailed(Smo *smo)
{
    smo->flagsSet(SmoFlags::FlagIntegrityError);
    smo->m_mainStatusCode = G8::err_IntegrityError;
    return false;
}

bool Smo::SelfParserForInvisible(Smo *smo)
{
    smo->flagsSet(SmoFlags::FlagInvisible);
    if (!AccessState_and_userPassStatus_forecast(smo, false))
    {
        return Smo::setsMessage(smo, "Access state undefined;\n");
    }
    return true;
}

bool Smo::IsDataEnabled(Smo *smo)
{
    return smo->headType() == SMOHeadTypes::SmoHeadType_Data || Smo::DataAddress(smo) != 0;
}

//may be called as: Smo::IsAccessible(Smo::AccessState(smo))
bool Smo::IsAccessible(SmoAccess smoAccess)
{
    return smoAccess == SmoAccess::ReadOnly || smoAccess == SmoAccess::FullAccess;
}

ushort Smo::DeviceCharset(Smo *smo)
{
    DeviceInfo* devinfo = smo->m_deviceInfo.data();
    return iif(devinfo, DeviceInfo::TdiInfo_charset(devinfo), 0);
}

bool Smo::IsSmoKeyRangeValid(smoKey sKey)
{
    if (IsSmoKeyToFlash(sKey)) {
        return sKey >= SmoFlashKey_address_min && sKey <= SmoFlashKey_address_max;
    }
    return sKey >= SmoRamKey_address_min && sKey <= SmoRamKey_address_max;
}

bool Smo::IntegrityCheck(Smo *smo)
{
    if (Smo::IsDevice(smo))
    {
        smoKey pKey = smo->m_record.ParentKey;
        if (!IsRootKey(pKey)) return Smo::IntegrityCheckFailed(smo);
        if (!IsSmoKeyRangeValid(Smo::Child_key(smo)))
        { //it's check a common Smo address range limits... normally it's device specific!!!
            return Smo::IntegrityCheckFailed(smo);
        }
    }
    else
    {
        smokey pkey = smo->Parent_key();
        Smo* parent_smo = smo->parentSmo();
        if (parent_smo) {
            if (parent_smo->folderType() == FolderTypes::LinkToFolder)
            {
                Smo* firstSibling = parent_smo->child(0);
                if (firstSibling) {
                    if (pkey != firstSibling->Parent_key()) return Smo::IntegrityCheckFailed(smo);
                }
                else
                {
                    if (smo->m_key != Smo::Child_key(parent_smo)) return Smo::IntegrityCheckFailed(smo);
                }
            }
            else
            {
                if (pkey != parent_smo->m_key) return Smo::IntegrityCheckFailed(smo);
            }
        }
        else
        {
            parent_smo = Smo::FindkeyLocaly(smo->m_deviceInfo->deviceItem(), pkey);
            if (parent_smo) {
                smo->setParent(parent_smo); // Smo::setParentSmo(smo, parent_smo);
            }
            else {
                parent_smo = new Smo(smo->m_deviceInfo.data(), pkey);
                if (Smo::Read(parent_smo))
                {
                    smo->setParent(parent_smo); //Smo::setParentSmo(smo, parent_smo);
                }
                else {
                    //parent_smo->deleteLater();
                    delete parent_smo;
                    return Smo::IntegrityCheckFailed(smo);
                }
            }
            //if (!Smo::IntegrityOfSmoKey(pkey)) return Smo::IntegrityCheckFailed(smo);
        }
//
//        if (smo->HeadType() == SmoHeadType::SmoHeadType_Folder)
//        {
//            if (!IntegrityOfSmoKey(Smo::Child_key(smo))) return Smo::IntegrityCheckFailed(smo);
//        }
    }
//integrity passed ok!...
    smo->flagsClear(SmoFlags::FlagIntegrityError);
    return true;
}

bool Smo::IsIntegrityOk(Smo *smo)
{
    return !(smo->m_flags & SmoFlags::FlagIntegrityError);
}

void Smo::NameOnStatus(Smo *smo) {
    SmoFlags sflags = (SmoFlags)smo->m_flags;
    if (sflags & SmoFlags::FlagIOMessage) {
        SetName_notParsed(smo, QStringLiteral("..."));
    }
    else if (sflags & SmoFlags::FlagIntegrityError) {
        SetName_notParsed(smo, QString()); //all clear!!!
        smo->m_sMessage = QStringLiteral("Integrity error");
    }
    else if (sflags & SmoFlags::FlagInvisible) {
        SetName_notParsed(smo, QStringLiteral("<invisible>"));
    }
    else if (sflags & SmoFlags::FlagEndTable) {
        SetName_notParsed(smo, QString()); //all clear!!!
    }
    else {
        SetName_notParsed(smo, QStringLiteral("<unusible>"));
        smo->m_sMessage = QString(EnumKeys<SmoFlags>(sflags));
    }
}

bool Smo::RecordParser(Smo *smo, const int recordLength)
{ //TODO: item text inside this!!!
    if (recordLength == smoAccessDenied_return_size)
    {
        return Smo::IntegrityCheck(smo) && SelfParserForInvisible(smo);
    }
    if(smo->m_record.ParentKey == smoEndTableMarker) {
        bitsSET(smo->m_flags, SmoFlags::FlagEndTable);
        smo->Record_clear();
        return false;
    }
    else {
        bitsCLEAR(smo->m_flags, SmoFlags::FlagEndTable);
    }
    //debug.Assert(smo->m_targetMemory.length() == 0);
    if (recordLength != iif(smo->IsName24(), smo32Size, smo16Size)) {
        return Smo::IntegrityCheckFailed(smo);
    }
    if (smo->m_key == AppName_address_gc || smo->m_key == Loader_address_gc)
    { // 0x0000=AppName, 0x0008=Loader; Extract key address from the record...
//        smo->m_flags.setFlag(Ok);
        u16 key = smo->Parent_key();
        if (key && smo->headType() == SmoHeadType::SmoHeadType_Folder)
        { //for a device the first smo must be a folder!!!
            smo->m_deviceInfo->setFirstSmoKey(smo->m_key = key); //the key was temporarely accomodated here!!!
            smo->setParent_key(0); //recover the ParentKey!
            smo->m_deviceInfo->setDeviceSmo(smo);
            return Smo::SmoDevice_TdiInfo_readAndCheck(smo) || Smo::IntegrityCheckFailed(smo);
        }
        else
        {
            return Smo::IntegrityCheckFailed(smo);
        }
    }
/// regular records...
    if (!Smo::IntegrityCheck(smo)) return Smo::IntegrityCheckFailed(smo);
    if (Smo::AccessState(smo) == SmoAccess::Invisible)
    { //if smo->AccessState == SmoAccess_e.Invisible then smo->deviceInfo userPass status is not equal its real value in device!!!
        if (!Smo::AccessState_and_userPassStatus_forecast(smo, true))
        {
            Smo::setsMessage(smo, Smo::sMessage(smo) + "Access state undefined;\n");
        }
    }
    smo->m_mainStatusCode = G8::msg_Success;
    return true;
}

//TODO: bool Smo::ParseAndFillDeviceInfo(Smo *smo)
//{
//    Smo *netDevice = new Smo(smo);
//    netDevice->m_targetMemory = smo->m_targetMemory;
//    netDevice->m_key = AppName_address_gc;
//    bool parserResult = Smo::SelfParser(netDevice);
//    if (parserResult)
//    {
//        netDevice->m_deviceInfo = new DeviceInfo(smo->m_deviceInfo.data(), smo, netDevice);
//        parserResult = Smo::SmoDevice_TdiInfo_readAndCheck(netDevice);
//    }
//    else
//    {
//        netDevice->m_deviceInfo = NULL;
//    }
//    netDevice->setHeadType(SmoHeadType::SmoHeadType_Folder);
//    netDevice->setFolderType(smo->FolderType());
//    smo->m_targetValue = QVariant::fromValue<Smo>(*netDevice);
//    return parserResult;
//}

QString Smo::toString() {
    return ShortName();
}

QString Smo::ShortName()
{
    return objectName();
}

//RecordName() returns: zero if result is empty;
//                      negative value if fromUtf8() was used;
//                      positive value: number of source bytes was token in account!!!
//int Smo::RecordName(QString &result, bool allchars)
//{ //ShortName(this);
//    int fieldlen = iif(IsName24(), smoFullNameMaxLength, smoShortNameMaxLength);
//    int slen = iif(allchars, fieldlen, strnlen(Record.RecordName, fieldlen));
//    if (slen) {
//        if(deviceInfo.isNull()) {
//            result = QString::fromUtf8(Record.RecordName, slen);
//            slen = -slen;
//        }
//        else {
//            QTextCodec* codec = deviceInfo->textCodec();
//            result = codec->toUnicode(Record.RecordName, slen);
//        }
//    }
//    else {
//        result = "";
//    }
//    return slen;
//}

bool Smo::IsRecordsEqual(Smo *smoL, Smo *smoR)
{
    int len = (smoL->IsName24() || smoR->IsName24()) ? smo32Size : smo16Size;
    char *recptrL = (char *)&smoL->m_record;
    char *recptrR = (char *)&smoR->m_record;
    for (int i = 0; i < len; i++)
    {
        if (*recptrL++ != *recptrR++) return false;
    }
    return true;
}

void Smo::Record_copy(Smo *destination, QByteArray* source)
{
    int leng = ::min(source->length(), smo32Size);
    memcpy((char*)&destination->m_record, source->constData(), leng);
    if (leng != smo32Size) ::memset(((char*)&destination->m_record) + leng, 0, smo32Size - leng);
}

void Smo::Record_copy(Smo *destination, Smo *source)
{
    memcpy((char*)&destination->m_record, (const char*)&source->m_record, smo32Size);
    //deprecated, do it separately:    destination->setText(0, Smo::ShortName(destination)); //update tree item!!!
}

void Smo::Record_clear(void)
{
    ::memset(&m_record, 0, smo32Size);
}

Smo *Smo::Message(Smo *parent, const QString& description, const char* icon)
{
    Smo *retv = new Smo(parent, "...");
    Smo::SetIcon(retv, icon);
    retv->setToolTip(0, description);
    return retv;
}

Smo *Smo::MessageInfo(Smo *parent, const QString& description)
{
    return Smo::Message(parent, description, "info");
}

Smo *Smo::MessageWarning(Smo *parent, const QString& description)
{
    return Smo::Message(parent, description, "warning");
}

Smo *Smo::MessageError(Smo *parent, const QString& description)
{
    return Smo::Message(parent, description, "error");
}

bool Smo::Read(Smo *smo)
{
    return Smo::Call(smo, NULL); //NULL as QByteArray* only!!!
}

/// if smo == Device then try to return a slave device AppName!!!
bool Smo::Call(Smo *smo)
{
//    QByteArray *param = new QByteArray(iif(smo->FolderType() == SmoFolderType::LinkToDevice, smoPointerSize, 0), '\0');
    QByteArray param(iif(smo->folderType() == SmoFolderType::LinkToDevice, smoPointerSize, 0), '\0');
    bool result = Smo::Call(smo, &param);
//    delete param;
    return result;
}

bool Smo::Call(Smo *smo, QVariant data)
{
    if (!data.isValid() || !Smo::calcTargetMemory(smo, &data)) {
        emit smo->dataInvalid(data);
        return false;
    }
    return Call(smo, &smo->m_targetMemory);
}

/// if data == NULL then read smo mode!!!
/// return: false if an attention required; true if everything is ok
bool Smo::Call(Smo *smo, QByteArray* data)
{
    if (!Smo::CallEnvironment_check(smo)) return false;

    //Generate frame smoBus...
    QByteArray txBuff;
    if (!Smo::SmoBus_frame_make(smo, &txBuff, data))
    {
        return Smo::ClearTargetData_and_setCallStatus(smo, SmoFlags::FlagIntegrityError, "Frame maker error");
    }
    //end Generate frame smoBus

    TDIPort* tdiPort = smo->m_deviceInfo->HWPort();

#if QT_VERSION >= 0x050800
    QDeadlineTimer deadline(tdiPort->timeout());
#define isTimeExpired() deadline.hasExpired()
#else
    QElapsedTimer deadline;
    deadline.start();
#define isTimeExpired() deadline.hasExpired(tdiPort->timeout())
#endif

//    if (tdiPort->write(netFrame) != netFrame.length())
    while (!tdiPort->dataPort(smo, txBuff))
    {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        if (isTimeExpired())
        {
            return Smo::ClearTargetData_and_setCallStatus(smo, SmoFlags::FlagIOError, tdiPort->errorString());
        }
    }
//    smo->m_targetAccessTime = QDateTime::currentDateTime();
    while (tdiPort->readFrame() != smo && tdiPort->asyncSmo() != smo) {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        if (isTimeExpired())
        {
            return Smo::ClearTargetData_and_setCallStatus(smo, SmoFlags::FlagIOError, tdiPort->errorString());
        }
    }
//DEPRECATED, it must be after value changed...    emit smo->dataChanged();
    return true;
}

//  Ret: .T. if success (result in oSmartObject.m_targetMemory)
bool Smo::calcTargetMemory(Smo *smo)
{
    return Smo::calcTargetMemory(smo, &smo->m_targetValue);
}

bool Smo::calcTargetMemory(Smo *smo, QVariant* data)
{
    SmoDataType DType = smo->dataType();
    int nDType = (int)DType;
    if (nDType >= 128)
    { //strings...
        if (data->type() == QVariant::Type::String) {
            smo->m_targetMemory = Smo::EncodeString(smo, data->value<QString>());
        }
        else
        { //another data...
            QDataStream dstr(&smo->m_targetMemory, QIODevice::Truncate);
            dstr << *data;
        }
//        QByteArray s = data->value<QString>().toLocal8Bit();
//        int dataSize = s.indexOf('\0');
//        if (dataSize == -1) dataSize = s.length();
//        int resultSize = nDType == smoDTZeroEndedAString_gc ? dataSize : (nDType & smoDTStringLengthMask);
//        if (resultSize > MaxStringLengthByApp) resultSize = MaxStringLengthByApp;
//        ArrayFillByData(&smo->m_targetMemory, s.data(), resultSize);
//        if (resultSize > dataSize) memset(smo->m_targetMemory.data()+dataSize, 0, resultSize - dataSize);
    }
    else if (nDType >= 64)
    { //Control... TODO:
        switch (DType) {
        case SmoDataType::ControlSelectItem:
            Serialize<UInt16>(&smo->m_targetMemory, data);
            break;
        case SmoDataType::ControlLogin: //NOTE: for ControlLogin add only pass, so smokey must be insert later!!!
        case SmoDataType::ControlPassLine:
            smo->m_targetMemory = QCryptographicHash::hash(data->value<QString>().toLocal8Bit(), QCryptographicHash::Algorithm::Sha1);
            break;
        default:
            smo->m_targetMemory.clear();
            return false;
        }
    }
    else if (nDType >= 32)
    { //SmoKeyFlags (always as a byte)...
        Serialize<byte>(&smo->m_targetMemory, data);
    }
    else
    {
        bool IsUnSigned = (nDType & 0x10) > 0;
        nDType &= 0x0F;
        if (nDType == 0)
        {
            if (IsUnSigned)
            { //Smo pointer...
                Serialize<smoPointerType>(&smo->m_targetMemory, data);
            } else {
                smo->m_targetMemory.clear();
                return false;
            }
        }
        else if (nDType == 1)
        {
            Serialize<byte>(&smo->m_targetMemory, data);
        }
        else if (nDType == 2)
        {
            Serialize<Int16>(&smo->m_targetMemory, data); //Int16 & UInt16 data size the same!!!
        }
        else if (nDType == 3)
        {
            ArrayFillByData(&smo->m_targetMemory, data->data(), nDType);
        }
        else if (nDType == 4)
        {
            Serialize<Int32>(&smo->m_targetMemory, data); //data size the same!!!
        }
        else if (nDType == 5)
        {
            Serialize<Int32>(&smo->m_targetMemory, data); //Serialize<float>(data)
        }
        else if (nDType == 6)
        {
            Serialize<Int32>(&smo->m_targetMemory, data);
        }
        else if (nDType == 7)
        {
            Serialize<Int64>(&smo->m_targetMemory, data);
        }
        else if (nDType == 8)
        {
            if (IsUnSigned)
            {
                Serialize<QDateTime>(&smo->m_targetMemory, data);
            }
            else
            {
                Serialize<double>(&smo->m_targetMemory, data);
            }
        }
        else if (nDType == 9)
        { //Serialize<int128>(&smo->m_targetMemory, data);
            ArrayFillByData(&smo->m_targetMemory, data->data(), 16);
        }
        else if (nDType == 10)
        { //TODO...
            return false;
        }
        else if (nDType == 11)
        { //TODO...
            return false;
        }
        else if (nDType == 12)
        { //TODO...
            return false;
        }
        else if (nDType == 13)
        { //TODO...
            return false;
        }
        else if (nDType == 14)
        { //TODO...
            return false;
        }
        else if (nDType == 15)
        { //TODO...
            return false;
        }
    }
    return true;
}

SmoUITypes Smo::UIType(Smo *smo)
{
    if (smo->m_flags & SmoFlags::FlagIntegrityError) return SmoUITypes::uiMessage;
    if (SmoDataType ctrltype = Smo::IsControlAndType(smo))
    {
        return (SmoUITypes)ctrltype;
    }
    if (Smo::IsStringAndType(smo)) return SmoUITypes::uiTextLine;
    if (Smo::IsBitAndType(smo)) return SmoUITypes::uiCheckBox;
    if (Smo::IsNumeric(smo))    return SmoUITypes::uiNumeric;
    return SmoUITypes::uiTextLine; //SmoUITypes::uiUndefined;
}

///TargetMemoryToString() used only for M2M informatives
QString Smo::TargetMemoryToString(Smo *smo)
{
    if ((smo->m_flags & SmoFlags::FlagIOk) && Smo::AccessState(smo) == SmoAccess::NoDataAccess)
    {
        return "*****";
    }
    SmoDataType smoDT = smo->dataType();
    int targetMemLen = smo->m_targetMemory.length();
    if (Smo::IsControlAndType(smo))
    { //return status...
        switch (smoDT) {
        case SmoDataType::ControlLogin: /*datasize: answer:3, request:1+ */
            if (targetMemLen == 3) {
                byte lug = ((LoginAndGroup_t*)smo->m_targetMemory.data())->LoginGroup;
                if (lug == LoginedUserGroup::NoLoginedUser) return QString();
                return QString::number((uint)lug & LoginedUserGroup_mask).repeated(20); //TODO: make same length with the hash algo (i.e. sha1,sha256,sha512)
            }
            return TargetMemoryToString_data_length_wrong(targetMemLen);
        case SmoDataType::ControlPassLine: /*datasize: answer:1, request:1+ */
            if (targetMemLen == 1) {
                byte lug = *(byte*)smo->m_targetMemory.data();
                if (lug == LoginedUserGroup::NoLoginedUser) return QString();
                return QString::number((uint)lug & LoginedUserGroup_mask).repeated(20); //TODO: make same length with the hash algo (i.e. sha1,sha256,sha512)
            }
            return TargetMemoryToString_data_length_wrong(targetMemLen);
        case SmoDataType::ControlSelectItem: /*datasize: 2; SelectTdi() is a handler for this */
            if (targetMemLen == 2) {
                return Smo::SmoKey2NameOrStatus(smo->m_deviceInfo.data(), *(smokey*)smo->m_targetMemory.data());
            }
            return TargetMemoryToString_data_length_wrong(targetMemLen);
        default:
            if (targetMemLen != 0)
            { //TODO:it
                return QString("TODO: CtrlBytes (count:%1): 0x%2").arg(targetMemLen).arg(QString(smo->m_targetMemory.toHex()));
            }
            break;
        }
    }
    else if (smoDT == SmoDataType::Pointer16) /*SmoPointer*/
    {
        if (targetMemLen == 2) {
            return Smo::SmoKey2NameOrStatus(smo->m_deviceInfo.data(), *(smokey*)smo->m_targetMemory.data());
        }
        return TargetMemoryToString_data_length_wrong(targetMemLen);
    }
    else if (smo->folderType() == SmoFolderType::LinkToDevice)
    {
        if (!smo->m_targetValue.isNull() && smo->m_targetValue.canConvert<Smo>())
        {
            const Smo &netDevice = smo->m_targetValue.value<Smo>();
            if (Smo::sMessage((Smo*)&netDevice).isEmpty())
            {
                return ((Smo*)&netDevice)->inviteString();
            }
            return Smo::sMessage((Smo*)&netDevice);
        }
        else if (targetMemLen == 1)
        {
            if (Smo::DisplayFormat(smo).isNull() || Smo::DisplayFormat(smo).indexOf('x') != -1 || Smo::DisplayFormat(smo).indexOf('X') != -1)
            { //for hex...
                return QString("0x%1").arg(QString(smo->m_targetMemory.toHex()));
            }
            return QString::number(*(byte*)smo->m_targetMemory.data());
        }
        return Smo::sMessage(smo);
    }
    else if (smoDT == SmoDataType::ControlPassLine)
    {
        if (smo->m_deviceInfo->IsUserAccepted())
        {
            return QString(16,'*');
        }
        else
        {
            return QString();
        }
    }
    else
    {
        if (Smo::IsStringAndType(smo))
        { //To do: encoding selectors!!!
            if (targetMemLen == 0) return QString();
            int resultLength = smoDT == SmoDataType::ZeString ? smo->m_targetMemory.indexOf('\0') : (int)smoDT & smoDTStringLengthMask;
            resultLength = resultLength == 0 ? targetMemLen : resultLength;
            return Smo::DecodeString(smo, smo->m_targetMemory.data(), resultLength);
        }
        else if (Smo::IsBitAndType(smo))
        {
            uint bitPosition = (uint)smo->dataType() & smoDTBitPositionInByte_bm;
            if ((uint)targetMemLen < (bitPosition + 8) / 8)
            {
                return QString();
            }
            return QString((*(byte*)smo->m_targetMemory.data()) & (uint)(1 << bitPosition) ? "true" : "false");
        }
        //next for 0..15 data types...
        switch (smoDT)
        { // commented options unused yet...
        case SmoDataType::S8:
            return SmoData_NumberToString<s8>(smo);
        case SmoDataType::U8:
            return SmoData_NumberToString<u8>(smo);
        case SmoDataType::S16:
            return SmoData_NumberToString<short>(smo);
        case SmoDataType::U16:
            return SmoData_NumberToString<ushort>(smo);
        case SmoDataType::RGB:
            if (targetMemLen < 3) return QString();
            return QString("TODO:RGB unsupported yet"); //((Int32)(BitConverter.ToInt32(smo->m_targetMemory, 0) & 0xFFFFFF)).ToString();
        case SmoDataType::U24:
            if (targetMemLen < 3) return QString();
            return QString("TODO:Unsigned24 unsupported yet"); //((UInt32)(BitConverter.ToUInt32(smo->m_targetMemory, 0) & 0xFFFFFF)).ToString();
        case SmoDataType::S32:
            return SmoData_NumberToString<Int32>(smo);
        case SmoDataType::U32:
            return SmoData_NumberToString<UInt32>(smo);
        case SmoDataType::Float32:
            return SmoData_NumberToString<float>(smo);
        case SmoDataType::DateBSD32: //TODO:
            break; //TODO   //4byte date(YYYYMMDD)
        case SmoDataType::RealTime32: //TODO:
            if (targetMemLen < 4) return QString();
            return Realtime32::ToString(smo->m_targetMemory.toUInt());
        case SmoDataType::Decimal64:
        case SmoDataType::S64:
            return SmoData_NumberToString<Int64>(smo);
        case SmoDataType::U64:
            return SmoData_NumberToString<UInt64>(smo);
        case SmoDataType::Float64:
            return SmoData_NumberToString<f64>(smo);
        case SmoDataType::DateTime64:
            if (targetMemLen < 8) return QString();
            return QDateTime::fromMSecsSinceEpoch(smo->m_targetMemory.toLongLong()).toString();
        case SmoDataType::Pointer16:
        case SmoDataType::Pointer32:
        case SmoDataType::Pointer64:
        case SmoDataType::U128:
            return QString("0x%1").arg(QString(smo->m_targetMemory.toHex()));
        case SmoDataType::SizedU8Data:
        {
            int len = *(u8*)smo->m_targetMemory.data();
            if (len > targetMemLen -1 || targetMemLen <= 1) {
                return QString();
            }
            return Smo::DecodeString(smo, smo->m_targetMemory.data()+1, (size_t)len);
        }
        case SmoDataType::SizedU32Data:
        {
            int len = *(u32*)smo->m_targetMemory.data();
            if (len > targetMemLen -1 || targetMemLen <= 5) {
                return QString();
            }
            return Smo::DecodeString(smo, smo->m_targetMemory.data()+4, (size_t)len);
        }
        case SmoDataType::Enumerator:
            return QString::number((Int32)Smo::DataAddress(smo));
        case SmoDataType::Void:
        case SmoDataType::Undefined:
            break; //return QString();
        default:
            return Smo::DecodeString(smo, smo->m_targetMemory.data(), targetMemLen);
        }
    }
    return QString(); // QString("")
}

/// DecodeString() upon the smo's device character set!!!
///  if the device info is absent then used: fromUtf8()!!!
QString Smo::DecodeString(Smo *smo, const char *data, size_t length)
{
    if (smo->m_deviceInfo.isNull()) {
        return QString::fromUtf8(data, length);
    }
    else {
        QTextCodec* codec = smo->m_deviceInfo->textCodec();
        return codec->toUnicode(data, length);
    }
}

QByteArray Smo::EncodeString(Smo *smo, const QString& data)
{
    if (smo->m_deviceInfo.isNull()) {
        return data.toUtf8();
    }
    else {
        QTextCodec* codec = smo->m_deviceInfo->textCodec();
        return codec->fromUnicode(data);
    }
}

/// \brief Smo::Path
/// \param smo
/// \param whatPath : smopath_inDevice = 0,smopath_full = 1,
/// \param delimeter: path delimeter
/// \return
QString Smo::Path(Smo *smo, PathOption whatPath, const QChar &delimeter)
{
    if (!smo) return QString();
    QString path(smo->ShortName());
    while (smo->parentSmo() && (whatPath != path_relative || smo != smo->m_deviceInfo->deviceItem()))
    {
        smo = smo->parentSmo();
        path = smo->ShortName() + delimeter + path;
    }
    return path;
}

/// SmartBackTransform() convert ONLY simple values from string to smo.m_targetValue!!!
/// return: true is result is not null!!!
bool Smo::SmartBackTransform(Smo* smo, const QString& data) {
    QVariant res = SmartBackTransformValue(smo, data);
    smo->m_targetValue.swap(res);
    return smo->m_targetValue.isValid();
}

//TODO: 2016 SmoDataType() changed!!!
/// \brief Smo::SmartBackTransformValue
/// \param smo
/// \param data string
/// \return QVariant() (invalid value) if not converted!!!
QVariant Smo::SmartBackTransformValue(Smo* smo, const QString& data)
{
    //string cFormat = oSmo.DisplayFormat;
    SmoDataType dataType = smo->dataType();
    int ntype = (int)dataType;
    if (ntype >= 128)
    { //strings...
        if (dataType == SmoDataType::ZeString)
        {
            return QVariant::fromValue(data.split(QChar('\0'),QString::SplitBehavior::SkipEmptyParts));
        }
        else
        {
            return QVariant::fromValue(data.left(ntype - (int)SmoDataType::ZeString));
        }
    }
    else if (ntype >= 64)
    {
        return QVariant::fromValue(data);
    }
    else if (ntype >= 32)
    { //Bits: as UInt32...
        return QVariant::fromValue((UInt32)(iif(data.left(1).toUpper()=="T", 1<<(ntype&0x1F), 0)));
    }
    else
    {
        bool isok = false;
        bool isFormatted = !Smo::DisplayFormat(smo).isEmpty();
        switch (dataType) {
        case SmoDataType::Void:
            return QVariant::fromValue(QString());
        case SmoDataType::Pointer16: /*SmoPointer*/
        {
            ushort result = data.toUShort(&isok, GetBaseOfNumber(data));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::U8:
        {
            ushort result = iif(isFormatted, SmoScanf<ushort>(data, smo, &isok), data.toUShort(&isok, GetBaseOfNumber(data)));
            return iif(isok && result<=0xFF, QVariant::fromValue((u8)result&0xFF), QVariant());
        }
        case SmoDataType::S8:
        {
            short result = iif(isFormatted, SmoScanf<short>(data, smo, &isok), data.toShort(&isok, GetBaseOfNumber(data)));
            return iif(isok && result>=-128 && result<=127, QVariant::fromValue((s8)result&0xFF), QVariant());
        }
        case SmoDataType::U16:
        {
            ushort result = iif(isFormatted, SmoScanf<ushort>(data, smo, &isok), data.toUShort(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::S16:
        {
            short result = iif(isFormatted, SmoScanf<short>(data, smo, &isok), data.toShort(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::U24:
        case SmoDataType::U32:
        {
            UInt32 result = iif(isFormatted, SmoScanf<UInt32>(data, smo, &isok), data.toUInt(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::RGB:
        { // TODO:
            return QVariant();
        }
        case SmoDataType::S32:
        {
            Int32 result = iif(isFormatted, SmoScanf<Int32>(data, smo, &isok), data.toInt(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::Float32:
        {
            float result = iif(isFormatted, SmoScanf<float>(data, smo, &isok), data.toFloat(&isok));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::DateBSD32:
        { //TODO personal data type (not UInt32)!!!
            UInt32 result = iif(isFormatted, SmoScanf<UInt32>(data, smo, &isok), data.toUInt(&isok, 16));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::RealTime32:
        { //TODO personal data type (not UInt32)!!!
            UInt32 result = data.toUInt(&isok, GetBaseOfNumber(data));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::SizedU8Data:
        { //TODO:
            return QVariant();
        }
        case SmoDataType::SizedU32Data:
        { //TODO:
            return QVariant();
        }
        case SmoDataType::U64:
        {
            UInt64 result = iif(isFormatted, SmoScanf<UInt64>(data, smo, &isok), data.toULongLong(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::S64:
        {
            Int64 result = iif(isFormatted, SmoScanf<Int64>(data, smo, &isok), data.toLongLong(&isok, GetBaseOfNumber(data)));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::Float64:
        {
            double result = iif(isFormatted, SmoScanf<double>(data, smo, &isok), data.toDouble(&isok));
            return iif(isok, QVariant::fromValue(result), QVariant());
        }
        case SmoDataType::DateTime64:
        { //TODO: define a string format
            QDateTime result = QDateTime::fromString(data);
            return QVariant::fromValue(result);
        }
        default:
            break;
        }
    }
    return QVariant();
}

const char* Smo::StatusToString(Smo::StatusFlags value)
{
    return EnumKeys<SmoFlags>(value); // QMetaEnum::fromType<SMOStatus>().valueToKeys((int)value).data();
}

static QString TargetMemoryToString_data_length_wrong(int len)
{
    return QString("the data length wrong: %1").arg(len);
}

