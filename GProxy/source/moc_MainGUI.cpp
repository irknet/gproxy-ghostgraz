/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Mon 23. Jan 18:53:58 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainGUI.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGUI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      73,   29,    8,    8, 0x0a,
     149,  115,    8,    8, 0x2a,
     205,  186,    8,    8, 0x2a,
     252,  237,    8,    8, 0x2a,
     287,  279,    8,    8, 0x0a,
     327,  310,    8,    8, 0x0a,
     368,  359,    8,    8, 0x0a,
     403,  395,    8,    8, 0x0a,
     438,  436,    8,    8, 0x0a,
     460,  436,    8,    8, 0x0a,
     491,  479,    8,    8, 0x0a,
     519,    8,    8,    8, 0x0a,
     562,  535,    8,    8, 0x0a,
     604,  595,    8,    8, 0x0a,
     644,  631,    8,    8, 0x0a,
     683,  670,    8,    8, 0x0a,
     706,    8,    8,    8, 0x2a,
     736,  725,    8,    8, 0x0a,
     765,    8,    8,    8, 0x0a,
     786,    8,    8,    8, 0x0a,
     811,  800,    8,    8, 0x0a,
     846,  836,    8,    8, 0x0a,
     869,    8,    8,    8, 0x08,
     879,    8,    8,    8, 0x08,
     891,    8,    8,    8, 0x08,
     920,    8,    8,    8, 0x08,
     946,    8,    8,    8, 0x08,
     975,    8,    8,    8, 0x08,
    1004,    8,    8,    8, 0x08,
    1040,    8,    8,    8, 0x08,
    1065,    8,    8,    8, 0x08,
    1087,    8,    8,    8, 0x08,
    1103,    8,    8,    8, 0x08,
    1142,    8,    8,    8, 0x08,
    1180,    8,    8,    8, 0x08,
    1205,    8,    8,    8, 0x08,
    1240,    8,    8,    8, 0x08,
    1285, 1273,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainGUI[] = {
    "MainGUI\0\0startUpdateThread()\0"
    "coloredMessage,log,printTimestamp,lineBreak\0"
    "addMessage(ColoredMessage,bool,bool,bool)\0"
    "coloredMessage,log,printTimestamp\0"
    "addMessage(ColoredMessage,bool,bool)\0"
    "coloredMessage,log\0addMessage(ColoredMessage,bool)\0"
    "coloredMessage\0addMessage(ColoredMessage)\0"
    "channel\0changeChannel(QString)\0"
    "username,clanTag\0addChannelUser(QString,QString)\0"
    "username\0removeChannelUser(QString)\0"
    "friends\0updateFriendlist(QList<Friend*>)\0"
    "f\0updateFriend(Friend*)\0addFriend(Friend*)\0"
    "entryNumber\0removeFriend(unsigned char)\0"
    "clearGamelist()\0botname,gamename,openSlots\0"
    "addGame(QString,QString,QString)\0"
    "slotList\0setGameslots(QList<Slot*>)\0"
    "errorMessage\0showErrorMessage(QString)\0"
    "exitOnReject\0showConfigDialog(bool)\0"
    "showConfigDialog()\0playername\0"
    "playerJoined(ColoredMessage)\0"
    "initConfigurations()\0applyConfig()\0"
    "area,color\0setColor(QString,QColor)\0"
    "area,font\0setFont(QString,QFont)\0"
    "onClose()\0onRestart()\0"
    "onInputTextAreaTextChanged()\0"
    "onTitleLabelTextChanged()\0"
    "onChannelContextMenu(QPoint)\0"
    "onFriendsContextMenu(QPoint)\0"
    "onGameListItemClicked(QMouseEvent*)\0"
    "onRefreshButtonClicked()\0updateRefreshButton()\0"
    "startWarcraft()\0onChannellistItemClicked(QMouseEvent*)\0"
    "onFriendlistItemClicked(QMouseEvent*)\0"
    "statspageLoginFinished()\0"
    "receivedPlayerInformation(Player*)\0"
    "onAdminlistReceived(QStringList)\0"
    "colorDialog\0userColorChanged(MColorDialog*)\0"
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
        case 0: startUpdateThread(); break;
        case 1: addMessage((*reinterpret_cast< const ColoredMessage(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 2: addMessage((*reinterpret_cast< const ColoredMessage(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: addMessage((*reinterpret_cast< const ColoredMessage(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: addMessage((*reinterpret_cast< const ColoredMessage(*)>(_a[1]))); break;
        case 5: changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: updateFriendlist((*reinterpret_cast< QList<Friend*>(*)>(_a[1]))); break;
        case 9: updateFriend((*reinterpret_cast< Friend*(*)>(_a[1]))); break;
        case 10: addFriend((*reinterpret_cast< Friend*(*)>(_a[1]))); break;
        case 11: removeFriend((*reinterpret_cast< unsigned char(*)>(_a[1]))); break;
        case 12: clearGamelist(); break;
        case 13: addGame((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 14: setGameslots((*reinterpret_cast< QList<Slot*>(*)>(_a[1]))); break;
        case 15: showErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: showConfigDialog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: showConfigDialog(); break;
        case 18: playerJoined((*reinterpret_cast< const ColoredMessage(*)>(_a[1]))); break;
        case 19: initConfigurations(); break;
        case 20: applyConfig(); break;
        case 21: setColor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 22: setFont((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 23: onClose(); break;
        case 24: onRestart(); break;
        case 25: onInputTextAreaTextChanged(); break;
        case 26: onTitleLabelTextChanged(); break;
        case 27: onChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 28: onFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 29: onGameListItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 30: onRefreshButtonClicked(); break;
        case 31: updateRefreshButton(); break;
        case 32: startWarcraft(); break;
        case 33: onChannellistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 34: onFriendlistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 35: statspageLoginFinished(); break;
        case 36: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 37: onAdminlistReceived((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 38: userColorChanged((*reinterpret_cast< MColorDialog*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 39;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
