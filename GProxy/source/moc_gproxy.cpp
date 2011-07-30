/****************************************************************************
** Meta object code from reading C++ file 'gproxy.h'
**
** Created: Sat 30. Jul 10:35:01 2011
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
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      38,   36,    8,    8, 0x05,
      70,    8,    8,    8, 0x05,
     100,   36,    8,    8, 0x05,
     139,    8,    8,    8, 0x05,
     173,    8,    8,    8, 0x05,
     201,  198,    8,    8, 0x05,
     240,    8,    8,    8, 0x05,
     274,    8,    8,    8, 0x05,
     307,    8,    8,    8, 0x05,
     336,    8,    8,    8, 0x05,
     364,    8,    8,    8, 0x05,
     394,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     415,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CGProxy[] = {
    "CGProxy\0\0signal_startUpdateThread()\0"
    ",\0signal_addMessage(QString,bool)\0"
    "signal_changeChannel(QString)\0"
    "signal_addChannelUser(QString,QString)\0"
    "signal_removeChannelUser(QString)\0"
    "signal_clearFriendlist()\0,,\0"
    "signal_addFriend(QString,bool,QString)\0"
    "signal_setGameslots(QList<Slot*>)\0"
    "signal_showErrorMessage(QString)\0"
    "signal_playerJoined(QString)\0"
    "signal_stopDownloadThread()\0"
    "signal_showConfigDialog(bool)\0"
    "signal_applyConfig()\0applyConfig()\0"
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
        case 0: signal_startUpdateThread(); break;
        case 1: signal_addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: signal_changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: signal_addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: signal_removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: signal_clearFriendlist(); break;
        case 6: signal_addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: signal_setGameslots((*reinterpret_cast< QList<Slot*>(*)>(_a[1]))); break;
        case 8: signal_showErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: signal_playerJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: signal_stopDownloadThread(); break;
        case 11: signal_showConfigDialog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: signal_applyConfig(); break;
        case 13: applyConfig(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CGProxy::signal_startUpdateThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CGProxy::signal_addMessage(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CGProxy::signal_changeChannel(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CGProxy::signal_addChannelUser(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CGProxy::signal_removeChannelUser(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CGProxy::signal_clearFriendlist()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CGProxy::signal_addFriend(QString _t1, bool _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CGProxy::signal_setGameslots(QList<Slot*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CGProxy::signal_showErrorMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CGProxy::signal_playerJoined(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CGProxy::signal_stopDownloadThread()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void CGProxy::signal_showConfigDialog(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CGProxy::signal_applyConfig()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}
QT_END_MOC_NAMESPACE
