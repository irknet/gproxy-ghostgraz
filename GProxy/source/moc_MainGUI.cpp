/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Sun 14. Aug 10:44:31 2011
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
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      41,   29,    8,    8, 0x0a,
      74,   66,    8,    8, 0x2a,
     102,   94,    8,    8, 0x0a,
     142,  125,    8,    8, 0x0a,
     183,  174,    8,    8, 0x0a,
     210,    8,    8,    8, 0x0a,
     253,  228,    8,    8, 0x0a,
     285,    8,    8,    8, 0x0a,
     328,  301,    8,    8, 0x0a,
     370,  361,    8,    8, 0x0a,
     410,  397,    8,    8, 0x0a,
     449,  436,    8,    8, 0x0a,
     472,    8,    8,    8, 0x2a,
     502,  491,    8,    8, 0x0a,
     524,    8,    8,    8, 0x0a,
     545,    8,    8,    8, 0x0a,
     570,  559,    8,    8, 0x0a,
     605,  595,    8,    8, 0x0a,
     628,    8,    8,    8, 0x08,
     638,    8,    8,    8, 0x08,
     650,    8,    8,    8, 0x08,
     679,    8,    8,    8, 0x08,
     705,    8,    8,    8, 0x08,
     734,    8,    8,    8, 0x08,
     763,    8,    8,    8, 0x08,
     799,    8,    8,    8, 0x08,
     826,    8,    8,    8, 0x08,
     851,    8,    8,    8, 0x08,
     873,    8,    8,    8, 0x08,
     889,    8,    8,    8, 0x08,
     928,    8,    8,    8, 0x08,
     966,    8,    8,    8, 0x08,
     991,    8,    8,    8, 0x08,
    1026,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainGUI[] = {
    "MainGUI\0\0startUpdateThread()\0message,log\0"
    "addMessage(QString,bool)\0message\0"
    "addMessage(QString)\0channel\0"
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
    "playerJoined(QString)\0initConfigurations()\0"
    "applyConfig()\0area,color\0"
    "setColor(QString,QColor)\0area,font\0"
    "setFont(QString,QFont)\0onClose()\0"
    "onRestart()\0onInputTextAreaTextChanged()\0"
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
        case 0: startUpdateThread(); break;
        case 1: addMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: addMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: changeChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: addChannelUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: removeChannelUser((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: clearFriendlist(); break;
        case 7: addFriend((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 8: clearGamelist(); break;
        case 9: addGame((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 10: setGameslots((*reinterpret_cast< QList<Slot*>(*)>(_a[1]))); break;
        case 11: showErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: showConfigDialog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: showConfigDialog(); break;
        case 14: playerJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: initConfigurations(); break;
        case 16: applyConfig(); break;
        case 17: setColor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 18: setFont((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 19: onClose(); break;
        case 20: onRestart(); break;
        case 21: onInputTextAreaTextChanged(); break;
        case 22: onTitleLabelTextChanged(); break;
        case 23: onChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 24: onFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 25: onGameListItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 26: onOutputFieldSliderMoved(); break;
        case 27: onRefreshButtonClicked(); break;
        case 28: updateRefreshButton(); break;
        case 29: startWarcraft(); break;
        case 30: onChannellistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 31: onFriendlistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 32: statspageLoginFinished(); break;
        case 33: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 34: onAdminlistReceived((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 35;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
