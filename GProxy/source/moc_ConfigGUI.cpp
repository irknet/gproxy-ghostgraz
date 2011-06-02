/****************************************************************************
** Meta object code from reading C++ file 'ConfigGUI.h'
**
** Created: Thu 2. Jun 13:05:15 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConfigGUI.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConfigGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConfigGUI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      20,   10,   10,   10, 0x08,
      34,   29,   10,   10, 0x08,
      61,   29,   10,   10, 0x08,
      88,   29,   10,   10, 0x08,
     115,   29,   10,   10, 0x08,
     142,   29,   10,   10, 0x08,
     168,   10,   10,   10, 0x08,
     194,   29,   10,   10, 0x08,
     231,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConfigGUI[] = {
    "ConfigGUI\0\0accept()\0reject()\0text\0"
    "onCDKeyROCChanged(QString)\0"
    "onCDKeyTFTChanged(QString)\0"
    "onUsernameChanged(QString)\0"
    "onPasswordChanged(QString)\0"
    "onChannelChanged(QString)\0"
    "onWar3pathChangeRequest()\0"
    "onServerComboboxItemChanged(QString)\0"
    "onForegroundcolorButtonClicked()\0"
};

const QMetaObject ConfigGUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ConfigGUI,
      qt_meta_data_ConfigGUI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConfigGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConfigGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConfigGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigGUI))
        return static_cast<void*>(const_cast< ConfigGUI*>(this));
    return QDialog::qt_metacast(_clname);
}

int ConfigGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: reject(); break;
        case 2: onCDKeyROCChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: onCDKeyTFTChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: onUsernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: onPasswordChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: onChannelChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: onWar3pathChangeRequest(); break;
        case 8: onServerComboboxItemChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: onForegroundcolorButtonClicked(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
