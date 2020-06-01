/****************************************************************************
** Meta object code from reading C++ file 'MainGazeTracker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainGazeTracker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainGazeTracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainGazeTracker_t {
    QByteArrayData data[7];
    char stringdata[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainGazeTracker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainGazeTracker_t qt_meta_stringdata_MainGazeTracker = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 7),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 16),
QT_MOC_LITERAL(4, 42, 12),
QT_MOC_LITERAL(5, 55, 12),
QT_MOC_LITERAL(6, 68, 11)
    },
    "MainGazeTracker\0process\0\0startCalibration\0"
    "startTesting\0choosePoints\0clearPoints"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainGazeTracker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainGazeTracker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainGazeTracker *_t = static_cast<MainGazeTracker *>(_o);
        switch (_id) {
        case 0: _t->process(); break;
        case 1: _t->startCalibration(); break;
        case 2: _t->startTesting(); break;
        case 3: _t->choosePoints(); break;
        case 4: _t->clearPoints(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MainGazeTracker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainGazeTracker.data,
      qt_meta_data_MainGazeTracker,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainGazeTracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainGazeTracker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGazeTracker.stringdata))
        return static_cast<void*>(const_cast< MainGazeTracker*>(this));
    return QObject::qt_metacast(_clname);
}

int MainGazeTracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
