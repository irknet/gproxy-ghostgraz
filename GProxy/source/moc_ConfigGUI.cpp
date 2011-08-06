/****************************************************************************
** Meta object code from reading C++ file 'ConfigGUI.h'
**
** Created: Fri 5. Aug 20:11:30 2011
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
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      28,   25,   10,   10, 0x05,
      79,   77,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,   10,   10,   10, 0x08,
     115,   10,   10,   10, 0x08,
     129,  124,   10,   10, 0x08,
     156,  124,   10,   10, 0x08,
     183,  124,   10,   10, 0x08,
     210,  124,   10,   10, 0x08,
     237,  124,   10,   10, 0x08,
     263,   10,   10,   10, 0x08,
     289,  124,   10,   10, 0x08,
     326,   10,   10,   10, 0x08,
     365,  359,   10,   10, 0x08,
     398,   10,   10,   10, 0x08,
     435,  430,   10,   10, 0x08,
     466,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConfigGUI[] = {
    "ConfigGUI\0\0applyConfig()\0,,\0"
    "colorChanged(QString,QPalette::ColorRole,QColor)\0"
    ",\0fontChanged(QString,QFont)\0accept()\0"
    "reject()\0text\0onCDKeyROCChanged(QString)\0"
    "onCDKeyTFTChanged(QString)\0"
    "onUsernameChanged(QString)\0"
    "onPasswordChanged(QString)\0"
    "onChannelChanged(QString)\0"
    "onWar3pathChangeRequest()\0"
    "onServerComboboxItemChanged(QString)\0"
    "onBackgroundcolorButtonClicked()\0color\0"
    "onBackgroundColorChanged(QColor)\0"
    "onOutputareaFontButtonClicked()\0font\0"
    "onOutputareaFontChanged(QFont)\0"
    "onOutputareaForegroundcolorButtonClicked()\0"
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
        case 0: applyConfig(); break;
        case 1: colorChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPalette::ColorRole(*)>(_a[2])),(*reinterpret_cast< const QColor(*)>(_a[3]))); break;
        case 2: fontChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 3: accept(); break;
        case 4: reject(); break;
        case 5: onCDKeyROCChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: onCDKeyTFTChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: onUsernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: onPasswordChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: onChannelChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: onWar3pathChangeRequest(); break;
        case 11: onServerComboboxItemChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: onBackgroundcolorButtonClicked(); break;
        case 13: onBackgroundColorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 14: onOutputareaFontButtonClicked(); break;
        case 15: onOutputareaFontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 16: onOutputareaForegroundcolorButtonClicked(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void ConfigGUI::applyConfig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ConfigGUI::colorChanged(const QString & _t1, const QPalette::ColorRole & _t2, const QColor & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConfigGUI::fontChanged(const QString & _t1, const QFont & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
