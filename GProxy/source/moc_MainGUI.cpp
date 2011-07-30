/****************************************************************************
** Meta object code from reading C++ file 'MainGUI.h'
**
** Created: Sat 30. Jul 10:35:10 2011
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
      33,   14, // methods
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
     559,  538,    8,    8, 0x0a,
     614,  604,    8,    8, 0x0a,
     637,    8,    8,    8, 0x08,
     647,    8,    8,    8, 0x08,
     676,    8,    8,    8, 0x08,
     702,    8,    8,    8, 0x08,
     731,    8,    8,    8, 0x08,
     760,    8,    8,    8, 0x08,
     796,    8,    8,    8, 0x08,
     823,    8,    8,    8, 0x08,
     848,    8,    8,    8, 0x08,
     870,    8,    8,    8, 0x08,
     886,    8,    8,    8, 0x08,
     925,    8,    8,    8, 0x08,
     963,    8,    8,    8, 0x08,
     988,    8,    8,    8, 0x08,
    1023,    8,    8,    8, 0x08,

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
        case 15: applyConfig(); break;
        case 16: setColor((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPalette::ColorRole(*)>(_a[2])),(*reinterpret_cast< const QColor(*)>(_a[3]))); break;
        case 17: setFont((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 18: onClose(); break;
        case 19: onInputTextAreaTextChanged(); break;
        case 20: onTitleLabelTextChanged(); break;
        case 21: onChannelContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 22: onFriendsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 23: onGameListItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 24: onOutputFieldSliderMoved(); break;
        case 25: onRefreshButtonClicked(); break;
        case 26: updateRefreshButton(); break;
        case 27: startWarcraft(); break;
        case 28: onChannellistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 29: onFriendlistItemClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 30: statspageLoginFinished(); break;
        case 31: receivedPlayerInformation((*reinterpret_cast< Player*(*)>(_a[1]))); break;
        case 32: onAdminlistReceived((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 33;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
