/*
 ** This file is a part of the DE project.
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

/* USAGE EXAMPLE:
 #include <QDebug>
 #include "jconfig.h"

    QString configFName = QApplication::applicationFilePath().append(".config");
    JConfig* config = new JConfig(this, configFName);
    QVariantMap configmap;
    if (!config->readToMap(configmap)) {
        qDebug() << config->exceptionString;
    }
    QString drvName = configmap.value("DriverName", QString()).toString();

config file content:
{
    "DriverName" : "QPSQL"
}

*/

/* WARNING: Qt5.9.1 JSON Parser does not work properly with keys in russian!!!
 * Read*() routines read directly from JConfig file!!!
 *
*/

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextCodec>
#include <QTextStream>
#include <QVariantMap>

#include "jconfig.h"

#ifndef NO_MACROS
#include "c_cpp_macros.h"
#else
#define iif(a,b,c) ((a) ? (b) : (c))
#endif

#ifndef UTF_8
#define UTF_8 106
#endif

JConfig::JConfig(QObject *parent, const char *FName, bool isCreateIfNotExist)
: QObject(parent)
, except(0)
{
    file = new QFile(QString::fromLocal8Bit(iif(FName == NULL, DEFAULT_CONFIG_FNAME, FName)), this);
    if (isCreateIfNotExist && !file->exists()) {
        file_create();
    }
}

JConfig::JConfig(QObject *parent, const QString& fname, bool isCreateIfNotExist)
: QObject(parent)
, except(0)
{
    file = new QFile(fname, this);
    if (isCreateIfNotExist && !file->exists()) {
        file_create();
    }
}

void JConfig::file_create()
{
    if (file->open(QIODevice::ReadWrite)) {
        file->write("", 1);
        file->close();
    }
}

/// \brief JConfig::openFile & set exception if something went wrong!
/// \param opmod
/// \return
bool JConfig::openFile(QIODevice::OpenMode opmod)
{
    if (!file->exists()) return setException("file not exist", QString());
    if (file->isOpen()) file->close();
    if (!file->open(opmod))
    {
        return setException("opening", file->errorString());
    }
    return true;
}

/// \brief JConfig::readToMap
/// \param tomap
/// \param folder
/// \return document map if folder empty
bool JConfig::readToMap(QVariantMap &tomap, const QString& folder)
{
    QJsonObject jobj;
    if (readToJsonObject(jobj, folder)) {
        tomap = jobj.toVariantMap();
        return true;
    }
    return false;
}

/// \brief JConfig::readToJsonDocument
/// \param jdoc
/// \return true if success!!!
bool JConfig::readToJsonDocument(QJsonDocument &jdoc)
{
    bool retv = false;
    if (openFile(QIODevice::ReadOnly)) {
        QJsonParseError *errors;
        errors = new QJsonParseError();
        jdoc = QJsonDocument::fromJson(file->readAll(), errors);
        retv = errors->error == QJsonParseError::NoError;
        if (!retv) {
            setException("parsing", errors->errorString(), errors->offset);
        }
        file->close();
    }
    return retv;
}

///
/// \brief JConfig::readToIndexedMap
/// \param tomap
/// \param folder
/// \return
/// Usage example:
///   QMapIterator<int, QPair<QString, QVariant>> imap(paramMap);
///   while (imap.hasNext()) {
///     imap.next();
///     int key = imap.key();
///     QString key = imap.value().first;
///     QVariant var = imap.value().second;
///   }
bool JConfig::readToIndexedMap(IndexedMap &tomap, const QString &folder)
{
    if (openFile(QIODevice::ReadOnly)) {
        QTextStream in(file);
        in.setCodec(QTextCodec::codecForMib(UTF_8));
        int linenum = -1;
        bool isInFolder = false;
        while (!in.atEnd()) {
            linenum++;
            QString linestr(in.readLine(222).toUtf8());
            int posf = linestr.indexOf('"');
            if (posf >=0) {
                int posl = linestr.indexOf('"', posf + 1);
                if (posl > posf) {
                    QString jkey(QStringRef(&linestr, posf+1, posl - posf-1).toString());
                    int posdata = linestr.indexOf(':', posl + 1);
                    if (posdata) {
                        if (isInFolder) {
                            int posstr = linestr.indexOf('"', posdata + 1);
                            if (posstr > posdata)
                            { //parse string...
                                QString vs(QStringRef(&linestr, posstr, posstr - posdata).toString());
                                tomap.insert(linenum, QPair<QString, QVariant>(jkey, QVariant::fromValue(vs)));
                            }
                            else
                            { //parse int, double, bool...
                                bool ok;
                                int poscomma = linestr.indexOf(',', posdata + 1);
                                QStringRef data(&linestr, posdata + 1, iif(poscomma >0, poscomma - posdata - 1,linestr.length() - posdata -1));
                                int vi = data.toInt(&ok);
                                if (ok) {
                                    tomap.insert(linenum, QPair<QString, QVariant>(jkey, QVariant::fromValue(vi)));
                                }
                                else
                                {
                                    double vd = data.toDouble(&ok);
                                    if (ok) {
                                        tomap.insert(linenum, QPair<QString, QVariant>(jkey, QVariant::fromValue(vd)));
                                    }
                                    else
                                    {
                                        if (linestr.indexOf("true", posdata + 1) > 0) {
                                            ok = true;
                                            tomap.insert(linenum, QPair<QString, QVariant>(jkey, QVariant::fromValue(ok)));
                                        }
                                        else if (linestr.indexOf("false", posdata + 1) > 0) {
                                            tomap.insert(linenum, QPair<QString, QVariant>(jkey, QVariant::fromValue(ok)));
                                        }
                                    }
                                }
                            }
                        }
                        else
                        { //check for folder...
                            isInFolder = linestr.indexOf('{', posdata + 1) >0 && folder == jkey;
                        }
                    }
                }
            }
        }
        file->close();
        return true;
    }
    return false;
}

/// \brief JConfig::readToJsonObject
/// \param jobject
/// \param folder
/// \return if folder is empty then doc.object else child object with key!!!
/// Usage example:
/// for (QJsonObject::iterator i = jobj.begin(); i!= jobj.end(); i++) {
///    qDebug() << i.key() << i.value();
/// }
bool JConfig::readToJsonObject(QJsonObject &jobject, const QString &folder)
{
    QJsonDocument jdoc;
    if (readToJsonDocument(jdoc)) {
        if (folder.isEmpty()) {
            jobject = jdoc.object();
            return true;
        }
        else {
            QJsonObject jobj = jdoc.object();
            if (jobj.contains(folder) && jobj.value(folder).isObject()) {
                jobject = jobj.value(folder).toObject();
                return true;
            }
        }
    }
    return false;
}

bool JConfig::saveFile(QVariantMap &vmap)
{
    if (!openFile(QIODevice::WriteOnly)) return false;
    file->resize(0); //truncate!!!
    if (file->write(QJsonDocument::fromVariant(vmap).toJson()) == -1) { //mainmap
        if (file->isOpen()) file->close();
        return setException("writing", file->errorString());
    }
    file->close();
    return clearException();
}

///Overwrite file by target data
bool JConfig::save(QVariant target)
{
    return save(target, QString());
}

///Insert target data to file if folder != empty
bool JConfig::save(QVariant fromvar, const QString& folder)
{
    QVariantMap mainmap;
    if (!fromvar.isValid() || fromvar.isNull()) return false;
    if (!folder.isEmpty() && file->exists() && openFile(QIODevice::ReadOnly)) {
        if (!readToMap(mainmap)) {
#ifndef NO_LIB
            zvvlib::BackupFileWithSuffix(file);
#endif
        }
    }
    if (folder.isEmpty()) {
        QVariant::Type tartype = fromvar.type();
        if (tartype == QVariant::Map) {
            mainmap = fromvar.value<QVariantMap>();
        }
        else if (tartype == QVariant::UserType && fromvar.userType() >= (int)QVariant::UserType) {
            saveObjectToMap(&mainmap, fromvar.value<QObject*>());
        }
    }
    else {
        saveObjectToMapFolder(&mainmap, &fromvar, QStringRef(&folder));
    }
    return saveFile(mainmap);
}

QVariantMap JConfig::saveObjectToMap(QVariantMap *tomap, QObject *fromvar) {
    const QMetaObject *metaobject = fromvar->metaObject();
    int count = metaobject->propertyCount();
    for (int i = metaobject->propertyOffset(); i<count; ++i) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        QVariant propertyValue = fromvar->property(name);
        if (propertyValue.type() == QVariant::UserType && (int)propertyValue.userType() >= (int)QVariant::UserType) {
            QVariantMap newmap;
            (*tomap)[name] = saveObjectToMap(&newmap, propertyValue.value<QObject*>());
        }
        else {
            (*tomap)[name] = propertyValue;
        }
    }
    return *tomap;
}

QVariantMap JConfig::saveObjectToMapFolder(QVariantMap *tomap, QVariant *fromvar, QStringRef folder)
{
    int dotpos = folder.indexOf('.');
    if (dotpos >= 0) { //go to next folder... recursively! if folder is not exist then create it!!!
        QString name = folder.left(dotpos).toString();
        QVariantMap newmap;
        if (tomap->contains(name) && (*tomap)[name].type() == QVariant::Map) {
            newmap = (*tomap)[name].value<QVariantMap>();
        }
        (*tomap)[name] = saveObjectToMapFolder(&newmap, fromvar, folder.mid(dotpos+1));
    }
    else
    { //write to destination folder...
        QString name = folder.toString();
        if (fromvar->type() == QVariant::UserType && fromvar->userType() >= (int)QVariant::UserType) {
            QVariantMap newmap;
            (*tomap)[name] = saveObjectToMap(&newmap, fromvar->value<QObject*>());
        }
        else {
            (*tomap)[name] = *fromvar;
        }
    }
    return *tomap;
}

/// \brief JConfig::saveObjectProperties
/// \param object_list
/// \param property_list
/// \return count of total saved properties or -1 if not saved
int JConfig::saveObjectPropertyList(const QObjectList &object_list, const QStringList &property_list)
{
    int retv = 0;
    foreach (QObject* object, object_list) {
        if (object) {
            QVariantMap vmap;
            foreach (QString pname, property_list) {
                QVariant pvalue = object->property(pname.toUtf8().constData());
                if (pvalue.isValid()) {
                    vmap.insert(pname, pvalue);
                    retv++;
                }
            }
            if (!save(vmap, object->objectName())) {
                retv = -1;
                break;
            }
        }
    }
    return retv;
}

/// \brief JConfig::readToObjectProperties read only property_list
/// \param object_list
/// \param property_list
/// \return count of total successful loaded properties
int JConfig::readToObjectPropertyList(const QObjectList &object_list, const QStringList &property_list)
{
    int retv = 0;
    QVariantMap mainmap;
    if (readToMap(mainmap)) {
        QVariantMap perObjectMap;
        foreach (QObject* object, object_list) {
            if (object) {
                if (mainmap.contains(object->objectName()) && mainmap[object->objectName()].type() == QVariant::Map) {
                    perObjectMap = mainmap[object->objectName()].value<QVariantMap>();
                    foreach (QString pname, property_list) {
                        QVariant pvalue = object->property(pname.toUtf8().constData());
                        if (pvalue.isValid() && perObjectMap.contains(pname)) {
                            if (object->setProperty(pname.toUtf8().constData(), perObjectMap[pname])) {
                                retv++;
                            }
                        }
                    }
                }
            }
        }
        clearException();
    }
    return retv;
}

bool JConfig::readToDynamicProperties(QVariantMap & tomap, const QString& folder)
{
    QVariant param(tomap);
    bool isok = readToDynamicProperties(param, folder);
    tomap = param.value<QVariantMap>();
    return isok;
}
//return false meaning: not all properties of the target object exists in config file
//                      or the folder is absent!!!
bool JConfig::readToDynamicProperties(QVariant & tovar, const QString& folder)
{
    QVariantMap mainmap;
    bool retv = readToMap(mainmap);
    if (folder.isEmpty()) {
        QVariant::Type tartype = tovar.type();
        if (tartype == QVariant::UserType && tovar.userType() >= (int)QVariant::UserType) {
            QObject* toobj = qobject_cast<QObject*>(tovar.value<QObject*>());
            retv = iif(toobj, mapToObjectDynamicProperties(&mainmap, toobj), false);
        }
        else if (tartype == QVariant::Map) {
            tovar = mainmap;
            retv = true;
        }
        else {
            retv = false;
        }
    }
    else {
        retv = mapToObjectDynamicProperties_fromFolder(&mainmap, &tovar, folder);
    }
    if (retv) clearException();
    return retv;
}

//return false meaning: not all properties of the target object exists in config file!!!
bool JConfig::mapToObjectDynamicProperties(QVariantMap *sourcemap, QObject *tovar)
{
    if (!tovar) return false;
    bool retv = true;
    const QMetaObject *metaobject = tovar->metaObject();
    int count = metaobject->propertyCount();
    for (int i =metaobject->propertyOffset(); i < count; ++i) {
        const QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        QVariant propertyValue = tovar->property(name);
        if (propertyValue.type() == QVariant::UserType && (int)propertyValue.userType() >= (int)QVariant::UserType) {
            retv = namedMapToObjectDynamicProperties(sourcemap, qobject_cast<QObject*>(propertyValue.value<QObject*>()), name);
        }
        else {
            if (sourcemap->contains(name)) {
                tovar->setProperty(name, (*sourcemap)[name]);
            }
            else
            { //the name absent in config file...
                if (!(metaproperty.isResettable() && metaproperty.reset(tovar))) {
                    tovar->setProperty(name, QVariant(propertyValue.type())); //Constructs a NULL variant of property type!!!
                }
                retv = false;
            }
        }
    }
    return retv;
}

bool JConfig::namedMapToObjectDynamicProperties(QVariantMap *sourcemap, QObject *toobj, const QString& name)
{
    if(!toobj) return false;
    QVariantMap newmap;
    if (sourcemap->contains(name) && (*sourcemap)[name].type() == QVariant::Map) {
        newmap = (*sourcemap)[name].value<QVariantMap>();
    }
    return mapToObjectDynamicProperties(&newmap, toobj);
}

//return false meaning: not all properties of the target object exists in config file!!!
///Param: folder: "top.child.grandchild" and so on...
bool JConfig::mapToObjectDynamicProperties_fromFolder(QVariantMap *sourcemap, QVariant *tovar, const QString& folder)
{
    bool retv = true;
    int dotpos = folder.indexOf('.');
    if (dotpos >= 0) { //go to next folder... recursively!
        const QString name = folder.left(dotpos);
        QVariantMap newmap;
        if (sourcemap->contains(name) && (*sourcemap)[name].type() == QVariant::Map) {
            newmap = (*sourcemap)[name].value<QVariantMap>();
            retv = mapToObjectDynamicProperties_fromFolder(&newmap, tovar, folder.mid(dotpos + 1));
        }
        else {
            mapToObjectDynamicProperties(&newmap, tovar->value<QObject*>());
            retv = false;
        }
    }
    else
    { //read from last child folder...
        if (tovar->type() == QVariant::UserType && tovar->userType() >= (int)QVariant::UserType)
        { //handle user objects...
            retv = namedMapToObjectDynamicProperties(sourcemap, qobject_cast<QObject*>(tovar->value<QObject*>()), folder);
        }
        else
        { //handle QT types...
            if (sourcemap->contains(folder)) {
                *tovar = (*sourcemap)[folder];
            }
            else
            {
                *tovar = QVariant(tovar->type()); //a reset value
                retv = false;
            }
        }
    }
    return retv;
}

/// \brief JConfig::readToUserProperties based on properties list from file!!!
/// \param toobj
/// \param metaobject
/// \param folder
/// \return
bool JConfig::readToUserProperties(QObject* toobj, const QMetaObject *metaobject, const QString &folder)
{
    QVariantMap mainmap;
    bool retv = readToMap(mainmap);
    retv = retv && mapToObjectUserProperties(&mainmap, toobj, metaobject, folder);
    if (retv) clearException();
    return retv;
}

/// \brief JConfig::ReadToObjectUserProperties()
/// \param sourcemap
/// \param toobj
/// \return false means: not all properties exist in the config file/folder!!!
bool JConfig::mapToObjectUserProperties(QVariantMap *sourcemap, QObject* toobj, const QMetaObject *metaobject, const QString& folder)
{
    if (!folder.isEmpty()) {
        QVariantMap newmap;
        int dotpos = folder.indexOf('.');
        if (dotpos >= 0) { //go to next folder in the map... recursively!
            const QString name = folder.left(dotpos);
            if (sourcemap->contains(name) && (*sourcemap)[name].type() == QVariant::Map) {
                newmap = (*sourcemap)[name].value<QVariantMap>();
                return mapToObjectUserProperties(&newmap, toobj, metaobject, folder.mid(dotpos + 1));
            }
        }
        else {
            if (sourcemap->contains(folder) && (*sourcemap)[folder].type() == QVariant::Map) {
                newmap = (*sourcemap)[folder].value<QVariantMap>();
                return mapToObjectUserProperties(&newmap, toobj, metaobject);
            }
        }
        return false;
    }
    //now we reach the target folder...
    if (!sourcemap || !toobj) return false;
    bool retv = true;
    int count = metaobject->propertyCount();
    for (int i = 0; i < count; ++i) {
        const QMetaProperty p = metaobject->property(i);
        if (p.isUser()) {
            const QString name = p.name();
            if (sourcemap->contains(name)) {
                QVariant propertyv = p.read(toobj); // tovar->property(p.name());
                QVariant sourcev = sourcemap->value(name, QVariant(p.type()));
                bool isok = false;
                if (propertyv.type() == sourcev.type()) {
                    isok = p.write(toobj, sourcev);
                }
                else if (p.isEnumType() && (int)sourcev.type() == QMetaType::QString) {
                    const QMetaEnum e = p.enumerator();
                    const QString estr = sourcev.value<QString>();
                    int enumv;
                    if (e.isFlag()) {
                        enumv = e.keysToValue(estr.toLatin1().constData(), &isok);
                    }
                    else {
                        enumv = e.keyToValue(estr.toLatin1().constData(), &isok);
                    }
                    if (isok) {
                        isok = p.write(toobj, QVariant::fromValue(enumv));
                    }
                }
                else {
                    if ((isok = sourcev.convert(propertyv.type()))) {
                        isok = p.write(toobj, sourcev);
                    }
                }
                if (!isok) {
                    p.write(toobj, QVariant(p.type())); //write a type default value!!!
                    retv = false;
                }
            }
            else
            { // the sourcemap does not have the name!!!
                retv = false; // means not all properties exist in the config file/folder!!!
                //p.write(toobj, QVariant(p.type()));     //write a type default value!!!
            }
        }
    }
    return retv;
}

#ifndef NO_EXCEPTIONS
zvvlib::Except *JConfig::lastException() const
{
    if (except && except->eMessage.isEmpty()) return 0;
    return except;
}
#endif

QString JConfig::lastErrorString()
{
#ifndef NO_EXCEPTIONS
    if (except) {
        return except->eMessage;
    }
#else
    return exceptionString;
#endif
}

QString JConfig::fileName()
{
    return file->fileName();
}

bool JConfig::setException(const QString& circumstance, const QString& errorStr)
{
    return setException(tr("JConfig: %1 error: %2; file: %3").arg(circumstance).arg(errorStr).arg(file->fileName()));
}

bool JConfig::setException(const QString& circumstance, const QString& errorStr, int offset)
{
    return setException(tr("JConfig: %1 error: %2; file: %3; offset: %4").arg(circumstance).arg(errorStr).arg(file->fileName()).arg(offset));
}

bool JConfig::setException(const QString& mess)
{
#ifndef NO_EXCEPTIONS
    if (except == 0) {
        except = new zvvlib::Except(mess);
    }
    else {
        except->eMessage = mess;
    }
#else
    exceptionString = mess;
#endif
    return false;
}

bool JConfig::clearException()
{
#ifndef NO_EXCEPTIONS
    if (except) except->eMessage.clear();
#else
    exceptionString.clear();
#endif
    return true;
}

