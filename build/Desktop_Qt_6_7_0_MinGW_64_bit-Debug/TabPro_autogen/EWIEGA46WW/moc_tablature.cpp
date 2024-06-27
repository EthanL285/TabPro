/****************************************************************************
** Meta object code from reading C++ file 'tablature.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../tablature.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tablature.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSTablatureENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSTablatureENDCLASS = QtMocHelpers::stringData(
    "Tablature",
    "addFretNumber",
    "",
    "goLeft",
    "goRight",
    "playTab",
    "ms",
    "playColumn",
    "getColumnInfo",
    "getSelectedColumnIndex",
    "selectColumn",
    "checked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTablatureENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x0a,    1 /* Public */,
       3,    0,   63,    2, 0x0a,    2 /* Public */,
       4,    0,   64,    2, 0x0a,    3 /* Public */,
       5,    1,   65,    2, 0x0a,    4 /* Public */,
       7,    0,   68,    2, 0x0a,    6 /* Public */,
       8,    0,   69,    2, 0x0a,    7 /* Public */,
       9,    0,   70,    2, 0x0a,    8 /* Public */,
      10,    1,   71,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject Tablature::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSTablatureENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTablatureENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTablatureENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Tablature, std::true_type>,
        // method 'addFretNumber'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goLeft'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goRight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'playColumn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getColumnInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getSelectedColumnIndex'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'selectColumn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void Tablature::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Tablature *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addFretNumber(); break;
        case 1: _t->goLeft(); break;
        case 2: _t->goRight(); break;
        case 3: _t->playTab((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 4: _t->playColumn(); break;
        case 5: _t->getColumnInfo(); break;
        case 6: { int _r = _t->getSelectedColumnIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->selectColumn((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *Tablature::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tablature::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTablatureENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Tablature::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
