/****************************************************************************
** Meta object code from reading C++ file 'TestClass.h'
**
** Created: Sun 12. Jun 19:21:58 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tests/TestClass.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TestClass.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestClass[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      27,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TestClass[] = {
    "TestClass\0\0loginFinished()\0"
    "receivedPlayerInformation(Player*)\0"
};

const QMetaObject TestClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestClass,
      qt_meta_data_TestClass, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestClass))
        return static_cast<void*>(const_cast< TestClass*>(this));
    return QObject::qt_metacast(_clname);
}

int TestClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loginFinished(); break;
        case 1: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
