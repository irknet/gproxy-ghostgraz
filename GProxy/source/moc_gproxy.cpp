/****************************************************************************
** Meta object code from reading C++ file 'gproxy.h'
**
** Created: Wed 11. May 20:10:17 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gproxy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGProxy[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x05,
      43,    8,    8,    8, 0x05,
      73,    9,    8,    8, 0x05,
     112,    8,    8,    8, 0x05,
     146,    8,    8,    8, 0x05,
     171,    9,    8,    8, 0x05,
     202,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CGProxy[] = {
    "CGProxy\0\0,\0signal_addMessage(QString,bool)\0"
    "signal_changeChannel(QString)\0"
    "signal_addChannelUser(QString,QString)\0"
    "signal_removeChannelUser(QString)\0"
    "signal_clearFriendlist()\0"
    "signal_addFriend(QString,bool)\0"
    "signal_setGameslots(vector<CIncomingSlots*>)\0"
};

const QMetaObject CGProxy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CGProxy,
      qt_meta_data_CGProxy, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGProxy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGProxy))
        return static_cast<void*>(const_cast< CGProxy*>(this));
    return QObject::qt_metacast(_clname);
}

int CGProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: signal_changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: signal_addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: signal_removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: signal_clearFriendlist(); break;
        case 5: signal_addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: signal_setGameslots((*reinterpret_cast< vector<CIncomingSlots*>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CGProxy::signal_addMessage(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CGProxy::signal_changeChannel(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CGProxy::signal_addChannelUser(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CGProxy::signal_removeChannelUser(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CGProxy::signal_clearFriendlist()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CGProxy::signal_addFriend(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CGProxy::signal_setGameslots(vector<CIncomingSlots*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
