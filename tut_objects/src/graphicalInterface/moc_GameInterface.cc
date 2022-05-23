/****************************************************************************
** Meta object code from reading C++ file 'GameInterface.hxx'
**
** Created: Wed Jun 13 03:43:34 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GameInterface.hxx"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameInterface.hxx' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GameInterface[] = {

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
      15,   14,   14,   14, 0x0a,
      25,   14,   14,   14, 0x0a,
      35,   14,   14,   14, 0x0a,
      44,   14,   14,   14, 0x0a,
      53,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GameInterface[] = {
    "GameInterface\0\0goSouth()\0goNorth()\0"
    "goEast()\0goWest()\0use()\0"
};

const QMetaObject GameInterface::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GameInterface,
      qt_meta_data_GameInterface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GameInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GameInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GameInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameInterface))
        return static_cast<void*>(const_cast< GameInterface*>(this));
    return QWidget::qt_metacast(_clname);
}

int GameInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goSouth(); break;
        case 1: goNorth(); break;
        case 2: goEast(); break;
        case 3: goWest(); break;
        case 4: use(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
