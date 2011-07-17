/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Sat 16. Jul 11:22:13 2011
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
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,    9,    8,    8, 0x0a,
      54,   46,    8,    8, 0x2a,
      82,   74,    8,    8, 0x0a,
     122,  105,    8,    8, 0x0a,
     163,  154,    8,    8, 0x0a,
     190,    8,    8,    8, 0x0a,
     233,  208,    8,    8, 0x0a,
     265,    8,    8,    8, 0x0a,
     308,  281,    8,    8, 0x0a,
     350,  341,    8,    8, 0x0a,
     390,  377,    8,    8, 0x0a,
     429,  416,    8,    8, 0x0a,
     452,    8,    8,    8, 0x2a,
     482,  471,    8,    8, 0x0a,
     504,    8,    8,    8, 0x0a,
     539,  518,    8,    8, 0x0a,
     594,  584,    8,    8, 0x0a,
     617,    8,    8,    8, 0x08,
     627,    8,    8,    8, 0x08,
     656,    8,    8,    8, 0x08,
     682,    8,    8,    8, 0x08,
     711,    8,    8,    8, 0x08,
     740,    8,    8,    8, 0x08,
     776,    8,    8,    8, 0x08,
     803,    8,    8,    8, 0x08,
     828,    8,    8,    8, 0x08,
     850,    8,    8,    8, 0x08,
     866,    8,    8,    8, 0x08,
     905,    8,    8,    8, 0x08,
     943,    8,    8,    8, 0x08,
     968,    8,    8,    8, 0x08,
    1003,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainGUI[] = {
    "MainGUI\0\0message,log\0addMessage(QString,bool)\0"
    "message\0addMessage(QString)\0channel\0"
    "changeChannel(QString)\0username,clanTag\0"
    "addChannelUser(QString,QString)\0"
    "username\0removeChannelUser(QString)\0"
    "clearFriendlist()\0username,online,location\0"
    "addFriend(QString,bool,QString)\0"
    "clearGamelist()\0botname,gamename,openSlots\0"
    "addGame(QString,QString,QString)\0"
    "slotList\0setGameslots(QList<Slot*>)\0"
    "errorMessage\0showErrorMessage(QString)\0"
    "exitOnReject\0showConfigDialog(bool)\0"
    "showConfigDialog()\0playerName\0"
    "playerJoined(QString)\0applyConfig()\0"
    "area,colorRole,color\0"
    "setColor(QString,QPalette::ColorRole,QColor)\0"
    "area,font\0setFont(QString,QFont)\0"
    "onClose()\0onInputTextAreaTextChanged()\0"
    "onTitleLabelTextChanged()\0"
    "onChannelContextMenu(QPoint)\0"
    "onFriendsContextMenu(QPoint)\0"
    "onGameListItemClicked(QMouseEvent*)\0"
    "onOutputFieldSliderMoved()\0"
    "onRefreshButtonClicked()\0updateRefreshButton()\0"
    "startWarcraft()\0onChannellistItemClicked(QMouseEvent*)\0"
    "onFriendlistItemClicked(QMouseEvent*)\0"
    "statspageLoginFinished()\0"
    "receivedPlayerInformation(Player*)\0"
    "onAdminlistReceived(QList<QString>)\0"
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
        case 0: addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: addMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: clearFriendlist(); break;
        case 6: addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: clearGamelist(); break;
        case 8: addGame((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 9: setGameslots((*reinterpret_cast< QList<Slot*>(*)>(_a[1]))); break;
        case 10: showErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: showConfigDialog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: showConfigDialog(); break;
        case 13: playerJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: applyConfig(); break;
        case 15: setColor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPalette::ColorRole(*)>(_a[2])),(*reinterpret_cast< const QColor(*)>(_a[3]))); break;
        case 16: setFont((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 17: onClose(); break;
        case 18: onInputTextAreaTextChanged(); break;
        case 19: onTitleLabelTextChanged(); break;
        case 20: onChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 21: onFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 22: onGameListItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 23: onOutputFieldSliderMoved(); break;
        case 24: onRefreshButtonClicked(); break;
        case 25: updateRefreshButton(); break;
        case 26: startWarcraft(); break;
        case 27: onChannellistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 28: onFriendlistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 29: statspageLoginFinished(); break;
        case 30: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 31: onAdminlistReceived((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 32;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
