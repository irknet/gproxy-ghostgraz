/****************************************************************************
** Meta object code from reading C++ file 'Slot.h'
**
** Created: Fri 19. Aug 17:28:42 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Slot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Slot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Slot[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       4,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       5, 0x0,    2,   30,
      20, 0x0,    3,   34,
      33, 0x0,   12,   40,
      43, 0x0,    5,   64,

 // enum data: key, value
      48, uint(Slot::NO_COMPUTER),
      60, uint(Slot::COMPUTER),
      69, uint(Slot::EASY),
      74, uint(Slot::NORMAL),
      81, uint(Slot::HARD),
      86, uint(Slot::RED),
      90, uint(Slot::BLUE),
      95, uint(Slot::TEAL),
     100, uint(Slot::PURPLE),
     107, uint(Slot::YELLOW),
     114, uint(Slot::ORANGE),
     121, uint(Slot::GREEN),
     127, uint(Slot::PINK),
     132, uint(Slot::GRAY),
     137, uint(Slot::LIGHT_BLUE),
     148, uint(Slot::DARK_GREEN),
     159, uint(Slot::BROWN),
     165, uint(Slot::HUMAN),
     171, uint(Slot::ORC),
     175, uint(Slot::NIGHT_ELF),
     185, uint(Slot::UNDEAD),
     192, uint(Slot::RANDOM),

       0        // eod
};

static const char qt_meta_stringdata_Slot[] = {
    "Slot\0ComputerStatus\0ComputerType\0"
    "SlotColor\0Race\0NO_COMPUTER\0COMPUTER\0"
    "EASY\0NORMAL\0HARD\0RED\0BLUE\0TEAL\0PURPLE\0"
    "YELLOW\0ORANGE\0GREEN\0PINK\0GRAY\0LIGHT_BLUE\0"
    "DARK_GREEN\0BROWN\0HUMAN\0ORC\0NIGHT_ELF\0"
    "UNDEAD\0RANDOM\0"
};

const QMetaObject Slot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Slot,
      qt_meta_data_Slot, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Slot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Slot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Slot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Slot))
        return static_cast<void*>(const_cast< Slot*>(this));
    return QObject::qt_metacast(_clname);
}

int Slot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
