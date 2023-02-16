/*TODO: find out why does it wrong!
*/

#ifndef SMOLISTPROPERTY_H
#define SMOLISTPROPERTY_H

#include <QQmlListProperty>
#include <QQmlEngine>
#include <QMetaObject>

QT_BEGIN_NAMESPACE

class QObject;
struct QMetaObject;
class Smo;

//template<typename T>
//class SmoListProperty : public QQmlListProperty<T> {
////    Q_OBJECT
//public:
//    SmoListProperty() {}
//    SmoListProperty(QObject *o, void *d, void* c, void* t) /*CountFunction c, AtFunction t*/
//        : QQmlListProperty<T>(o, d, c, t) {}

////    Q_INVOKABLE int count() const {
////        return qobject_cast<Smo*>(QQmlListProperty<T>::object)->childCountQml();
////    }

//private:

//};


//NEXT: clone of QQmlListProperty...
template<typename T>
class SmoListProperty {
public:
    typedef void (*AppendFunction)(SmoListProperty<T> *, T*);
    typedef int (*CountFunction)(SmoListProperty<T> *);
    typedef T *(*AtFunction)(SmoListProperty<T> *, int);
    typedef void (*ClearFunction)(SmoListProperty<T> *);

    SmoListProperty()
        : object(Q_NULLPTR),
          data(Q_NULLPTR),
          append(Q_NULLPTR),
          count(Q_NULLPTR),
          at(Q_NULLPTR),
          clear(Q_NULLPTR),
          dummy1(Q_NULLPTR),
          dummy2(Q_NULLPTR)
    {}
    SmoListProperty(QObject *o, QList<T *> &list)
        : object(o), data(&list), append(qlist_append), count(qlist_count), at(qlist_at),
          clear(qlist_clear),
          dummy1(Q_NULLPTR),
          dummy2(Q_NULLPTR)
    {}
    SmoListProperty(QObject *o, void *d, AppendFunction a, CountFunction c, AtFunction t,
                    ClearFunction r )
        : object(o),
          data(d),
          append(a),
          count(c),
          at(t),
          clear(r),
          dummy1(Q_NULLPTR),
          dummy2(Q_NULLPTR)
    {}
    SmoListProperty(QObject *o, void *d, CountFunction c, AtFunction t)
        : object(o),
          data(d),
          append(Q_NULLPTR),
          count(c), at(t),
          clear(Q_NULLPTR),
          dummy1(Q_NULLPTR),
          dummy2(Q_NULLPTR)
    {}
    bool operator==(const SmoListProperty &o) const {
        return object == o.object &&
               data == o.data &&
               append == o.append &&
               count == o.count &&
               at == o.at &&
               clear == o.clear;
    }

    Q_INVOKABLE int folderItemsCount() const { return qobject_cast<T*>(object)->folderItemsCount();}

    QObject *object;
    void *data;

    AppendFunction append;

    CountFunction count;
    AtFunction at;

    ClearFunction clear;

    void *dummy1;
    void *dummy2;

private:
    static void qlist_append(SmoListProperty *p, T *v) {
        reinterpret_cast<QList<T *> *>(p->data)->append(v);
    }
    static int qlist_count(SmoListProperty *p) {
        return reinterpret_cast<QList<T *> *>(p->data)->count();
    }
    static T *qlist_at(SmoListProperty *p, int idx) {
        return reinterpret_cast<QList<T *> *>(p->data)->at(idx);
    }
    static void qlist_clear(SmoListProperty *p) {
        return reinterpret_cast<QList<T *> *>(p->data)->clear();
    }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(SmoListProperty<Smo>)

//Q_DECLARE_METATYPE(SmoListProperty)


#endif // SMOLISTPROPERTY_H
