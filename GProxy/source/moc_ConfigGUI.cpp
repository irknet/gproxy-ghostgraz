/****************************************************************************
** Meta object code from reading C++ file 'ConfigGUI.h'
**
** Created: Sat 14. May 20:50:34 2011
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
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      20,   10,   10,   10, 0x08,
      29,   10,   10,   10, 0x08,
      57,   10,   10,   10, 0x08,
      90,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConfigGUI[] = {
    "ConfigGUI\0\0accept()\0reject()\0"
    "onServerCBChanged(QWidget*)\0"
    "onTextfieldTextChanged(QWidget*)\0"
    "onOpenFileDialog(QWidget*)\0"
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
        case 2: onServerCBChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 3: onTextfieldTextChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 4: onOpenFileDialog((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_FileDialogLineEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_FileDialogLineEdit[] = {
    "FileDialogLineEdit\0\0clicked()\0"
};

const QMetaObject FileDialogLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_FileDialogLineEdit,
      qt_meta_data_FileDialogLineEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileDialogLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileDialogLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileDialogLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileDialogLineEdit))
        return static_cast<void*>(const_cast< FileDialogLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int FileDialogLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FileDialogLineEdit::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
