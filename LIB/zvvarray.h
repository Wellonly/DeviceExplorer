#ifndef ZVVARRAY_H
#define ZVVARRAY_H

using namespace std;

#include <QByteArray>
#include <QDataStream>

inline void ArrayFillByData(QByteArray* array, const void* dataPtr, int size) {
    array->resize(size);
    memcpy(array->data(), dataPtr, size);
}

///ret target array size
template<class T> size_t ArrayFillByValue(QByteArray* array, T* ref) {
    size_t ret = sizeof(T);
    array->resize(ret);
    memcpy(array->data(), (const void*)ref, ret);
    return ret;
}

template<class T> QByteArray ArrayFromValue(T *ref) {
    return QByteArray::fromRawData((const char *)ref, sizeof(T));
}

template<class T> T ArrayToValue(QByteArray *ref) {
    T retv;
    QDataStream strm(ref, QIODevice::ReadOnly);
    strm >> retv;
    return retv;
}

template<class T> T ArrayToValue(QByteArray *ref, int offset) {
    T retv;
    QDataStream strm(ref, QIODevice::ReadOnly);
    if (strm.skipRawData(offset) == offset) {
        strm >> retv;
    }
    return retv;
}

#endif // ZVVARRAY_H
