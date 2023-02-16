///zvvlib enum system !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///
#ifndef ZVVENUM_H
#define ZVVENUM_H

using namespace std;

#include <QString>
#include <QMetaEnum>
#include <QMetaObject>
#include <QByteArray>
#include <QMap>

template<class T> QMetaEnum Enum() {
    return T::staticMetaObject.enumerator(T::staticMetaObject.enumeratorOffset());
}
template<class T> QMetaEnum Enum(const char * enumName) {
    return T::staticMetaObject.enumerator(T::staticMetaObject.indexOfEnumerator(enumName));
}

template<class T, typename V> QList<V> EnumValuesList() {
    QList<V> retv;
    QMetaEnum enm = Enum<T>();
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv.append((V)enm.value(i));
    }
    return retv;
}
template<class T, typename V> QList<V> EnumValuesList(const char * enumName) {
    QList<V> retv;
    QMetaEnum enm = Enum<T>(enumName);
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv.append((V)enm.value(i));
    }
    return retv;
}

template<class T> QStringList EnumItemsList() {
    QStringList retv;
    QMetaEnum enm = Enum<T>();
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv.append(enm.key(i));
    }
    return retv;
}
template<class T> QStringList EnumItemsList(const char * enumName) {
    QStringList retv;
    QMetaEnum enm = Enum<T>(enumName);
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv.append(enm.key(i));
    }
    return retv;
}
template<class T, typename V> QMap<QString, V> EnumItemsMap() {
    QMap<QString, V> retv;
    QMetaEnum enm = Enum<T>();
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv[enm.key(i)] = (V)enm.value(i);
    }
    return retv;
}
template<class T, typename V> QMap<QString, V> EnumItemsMap(const char * enumName) {
    QMap<QString, V> retv;
    QMetaEnum enm = Enum<T>(enumName);
    int cou = enm.keyCount();
    for(int i = T::staticMetaObject.enumeratorOffset(); i<cou; ++i) {
        retv[enm.key(i)] = (V)enm.value(i);
    }
    return retv;
}

///ret 0 if value is not found!
template<class T> const char* EnumKey(const char *enumName, int value) {
    QMetaEnum metaenum = T::staticMetaObject.enumerator(T::staticMetaObject.indexOfEnumerator(enumName));
    return metaenum.valueToKey(value);
}

template<class T> const char* EnumKey(int value) {
#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
    //use Enum<T>()
    return "UNSUPPORTED BY QT BEFORE v5.5.0";
#else
    return QMetaEnum::fromType<T>().valueToKey(value);
#endif
}
template<class T> QByteArray EnumKeys(int value) {
#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
    //use Enum<T>()
    return "UNSUPPORTED BY QT BEFORE v5.5.0";
#else
//    QString ret(QMetaEnum::fromType<T>().valueToKeys(value).data());
    return QMetaEnum::fromType<T>().valueToKeys(value);
#endif
}

///ret -1 if key is not found!
template<class T> int EnumValue(const char *enumName, const char * key) {
    QMetaEnum metaenum = T::staticMetaObject.enumerator(T::staticMetaObject.indexOfEnumerator(enumName));
    return metaenum.keyToValue(key);
}

// EnumItemsList usage example: EnumItemsList<SerialParity>()
//class SerialParity : public QObject
//{
//    Q_OBJECT
//    Q_ENUM(serialParity)
//public:
//    typedef enum serialParity
//    {
//        None = QSerialPort::Parity::NoParity,
//        Even = QSerialPort::Parity::EvenParity,
//        Odd = QSerialPort::Parity::OddParity,
//        Space = QSerialPort::Parity::SpaceParity,
//        Mark = QSerialPort::Parity::MarkParity,
//    } serialParity;
//};

#endif // ZVVENUM_H
