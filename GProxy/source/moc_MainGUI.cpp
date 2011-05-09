/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Mon 9. May 19:21:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainGUI.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGUI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      19,    8,    8,    8, 0x08,
      43,    8,    8,    8, 0x08,
      56,    8,    8,    8, 0x08,
      87,    8,    8,    8, 0x08,
     118,    8,    8,    8, 0x08,
     156,    8,    8,    8, 0x08,
     181,    8,    8,    8, 0x08,
     215,  203,    8,    8, 0x0a,
     248,  240,    8,    8, 0x2a,
     276,  268,    8,    8, 0x0a,
     316,  299,    8,    8, 0x0a,
     357,  348,    8,    8, 0x0a,
     384,    8,    8,    8, 0x0a,
     418,  402,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainGUI[] = {
    "MainGUI\0\0onClose()\0inputFieldTextChanged()\0"
    "changeFont()\0showChannelContextMenu(QPoint)\0"
    "showFriendsContextMenu(QPoint)\0"
    "gameListItemClicked(QListWidgetItem*)\0"
    "outputFieldSliderMoved()\0actionConfigClicked()\0"
    "message,log\0addMessage(QString,bool)\0"
    "message\0addMessage(QString)\0channel\0"
    "changeChannel(QString)\0username,clanTag\0"
    "addChannelUser(QString,QString)\0"
    "username\0removeChannelUser(QString)\0"
    "clearFriendlist()\0username,online\0"
    "addFriend(QString,bool)\0"
};

const QMetaObject MainGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainGUI,
      qt_meta_data_MainGUI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGUI))
        return static_cast<void*>(const_cast< MainGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onClose(); break;
        case 1: inputFieldTextChanged(); break;
        case 2: changeFont(); break;
        case 3: showChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: showFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: gameListItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: outputFieldSliderMoved(); break;
        case 7: actionConfigClicked(); break;
        case 8: addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: addMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 12: removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: clearFriendlist(); break;
        case 14: addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
