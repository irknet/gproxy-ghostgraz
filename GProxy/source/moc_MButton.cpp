/****************************************************************************
** Meta object code from reading C++ file 'MButton.h'
**
** Created: Sun 21. Aug 10:51:57 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget/MButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MButton[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MButton[] = {
    "MButton\0\0button\0clicked(MButton*)\0"
    "onButtonClicked()\0"
};

const QMetaObject MButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_MButton,
      qt_meta_data_MButton, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MButton))
        return static_cast<void*>(const_cast< MButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int MButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< MButton*(*)>(_a[1]))); break;
        case 1: onButtonClicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MButton::clicked(MButton * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE