/****************************************************************************
** Meta object code from reading C++ file 'Statspage.h'
**
** Created: Thu 30. Jun 19:59:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "statspage/Statspage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Statspage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Statspage[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      27,   10,   10,   10, 0x05,
      67,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,   10,   10,   10, 0x08,
     141,   10,   10,   10, 0x08,
     176,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Statspage[] = {
    "Statspage\0\0loginFinished()\0"
    "playerInformationReplyFinished(Player*)\0"
    "adminlistReplyFinished(QList<QString>)\0"
    "forumReplyFinished(QNetworkReply*)\0"
    "loginReplyFinished(QNetworkReply*)\0"
    "replyFinished(QNetworkReply*)\0"
};

const QMetaObject Statspage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Statspage,
      qt_meta_data_Statspage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Statspage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Statspage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Statspage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Statspage))
        return static_cast<void*>(const_cast< Statspage*>(this));
    return QObject::qt_metacast(_clname);
}

int Statspage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loginFinished(); break;
        case 1: playerInformationReplyFinished((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 2: adminlistReplyFinished((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        case 3: forumReplyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: loginReplyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 5: replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Statspage::loginFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Statspage::playerInformationReplyFinished(Player * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Statspage::adminlistReplyFinished(QList<QString> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
