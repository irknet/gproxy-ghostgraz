/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Mon 13. Jun 10:19:06 2011
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
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      19,    8,    8,    8, 0x08,
      45,    8,    8,    8, 0x08,
      64,    8,    8,    8, 0x08,
      93,    8,    8,    8, 0x08,
     122,    8,    8,    8, 0x08,
     158,    8,    8,    8, 0x08,
     185,    8,    8,    8, 0x08,
     210,    8,    8,    8, 0x08,
     232,    8,    8,    8, 0x08,
     248,    8,    8,    8, 0x08,
     287,    8,    8,    8, 0x08,
     325,    8,    8,    8, 0x08,
     350,    8,    8,    8, 0x08,
     397,  385,    8,    8, 0x0a,
     430,  422,    8,    8, 0x2a,
     458,  450,    8,    8, 0x0a,
     498,  481,    8,    8, 0x0a,
     539,  530,    8,    8, 0x0a,
     566,    8,    8,    8, 0x0a,
     609,  584,    8,    8, 0x0a,
     641,    8,    8,    8, 0x0a,
     684,  657,    8,    8, 0x0a,
     726,  717,    8,    8, 0x0a,
     777,  764,    8,    8, 0x0a,
     803,    8,    8,    8, 0x0a,
     833,  822,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainGUI[] = {
    "MainGUI\0\0onClose()\0onInputFieldTextChanged()\0"
    "onChannelChanged()\0onChannelContextMenu(QPoint)\0"
    "onFriendsContextMenu(QPoint)\0"
    "onGameListItemClicked(QMouseEvent*)\0"
    "onOutputFieldSliderMoved()\0"
    "onRefreshButtonClicked()\0updateRefreshButton()\0"
    "startWarcraft()\0onChannellistItemClicked(QMouseEvent*)\0"
    "onFriendlistItemClicked(QMouseEvent*)\0"
    "statspageLoginFinished()\0"
    "receivedPlayerInformation(Player*)\0"
    "message,log\0addMessage(QString,bool)\0"
    "message\0addMessage(QString)\0channel\0"
    "changeChannel(QString)\0username,clanTag\0"
    "addChannelUser(QString,QString)\0"
    "username\0removeChannelUser(QString)\0"
    "clearFriendlist()\0username,online,location\0"
    "addFriend(QString,bool,QString)\0"
    "clearGamelist()\0botname,gamename,openSlots\0"
    "addGame(QString,QString,QString)\0"
    "slotList\0setGameslots(vector<CIncomingSlots*>)\0"
    "errorMessage\0showErrorMessage(QString)\0"
    "showConfigDialog()\0playerName\0"
    "playerJoined(QString)\0"
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
        case 1: onInputFieldTextChanged(); break;
        case 2: onChannelChanged(); break;
        case 3: onChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: onFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: onGameListItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: onOutputFieldSliderMoved(); break;
        case 7: onRefreshButtonClicked(); break;
        case 8: updateRefreshButton(); break;
        case 9: startWarcraft(); break;
        case 10: onChannellistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 11: onFriendlistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 12: statspageLoginFinished(); break;
        case 13: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 14: addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: addMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 18: removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: clearFriendlist(); break;
        case 20: addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 21: clearGamelist(); break;
        case 22: addGame((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 23: setGameslots((*reinterpret_cast< vector<CIncomingSlots*>(*)>(_a[1]))); break;
        case 24: showErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: showConfigDialog(); break;
        case 26: playerJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
