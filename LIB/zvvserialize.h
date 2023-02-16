// serialization...
#ifndef ZVVSERIALIZE_H
#define ZVVSERIALIZE_H

using namespace std;

#include <QByteArray>
#include <QVariant>

template<class T> void* Serialize(QByteArray* targetArray, T* valuePtr) {
    targetArray->resize(sizeof(T));
    return memcpy(targetArray->data(), (const void*)valuePtr, sizeof(T));
}
template<class T> void* Serialize(QByteArray* targetArray, QVariant* qVariantPtr) {
    return Serialize<T>(targetArray, (T*)qVariantPtr->data());
}

template<class T> QByteArray Serialize(T *ref) {
    return ArrayFromValue<T>(ref);
}
template<class T> QByteArray Serialize(QVariant *ref) {
    T value = ref->value<T>();
    return ArrayFromValue<T>(&value);
}

template<class T> T Deserialize(QByteArray *ref) {
    return ArrayToValue<T>(ref);
}
template<class T> T Deserialize(QByteArray *ref, int offset) {
    return ArrayToValue<T>(ref, offset);
}

template<class T>
T DeserializeUncompletedNumber(const char* data, const int dataLength) {
    T value = 0;
    memcpy(&value, data, ::min(sizeof(T), (/*win32 long *//*unsigned int*/ decltype(sizeof(T)))dataLength));
    return value;
}



template<typename T>
T sscanf_to_value(const char* data_str, const char* format, bool* isok) {
    QByteArray result_data(sizeof (T), '\0');
    *isok = std::sscanf(data_str, format, result_data.data()) == 1;
    return *(T*)result_data.constData();
}

#endif // ZVVSERIALIZE_H
