/****************************************************************************
** Meta object code from reading C++ file 'ClickableListWidget.h'
**
** Created: Sun 12. Jun 19:22:07 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget/ClickableListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClickableListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClickableListWidget[] = {

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
      32,   21,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ClickableListWidget[] = {
    "ClickableListWidget\0\0mouseEvent\0"
    "clicked(QMouseEvent*)\0"
};

const QMetaObject ClickableListWidget::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_ClickableListWidget,
      qt_meta_data_ClickableListWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClickableListWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClickableListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClickableListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClickableListWidget))
        return static_cast<void*>(const_cast< ClickableListWidget*>(this));
    return QListWidget::qt_metacast(_clname);
}

int ClickableListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableListWidget::clicked(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
