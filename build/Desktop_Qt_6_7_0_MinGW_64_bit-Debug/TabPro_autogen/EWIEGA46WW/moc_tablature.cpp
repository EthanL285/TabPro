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
    "getSelectedColumnIndex",
    "",
    "addFretNumber",
    "goLeft",
    "goRight",
    "playTab",
    "QPushButton*",
    "play",
    "playColumn",
    "getColumnInfo",
    "stopTempoTimer",
    "changeTempoButton",
    "QLineEdit*",
    "field",
    "increase",
    "decrease",
    "changeTempoEdit",
    "insertSlideUp",
    "insertSlideDown",
    "insertHammerOn",
    "insertPullOff",
    "insertBend",
    "insertRelease",
    "insertVibrato",
    "insertMutedHit",
    "insertRest",
    "insertBarLine",
    "undo",
    "remove",
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
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  152,    2, 0x0a,    1 /* Public */,
       3,    0,  153,    2, 0x0a,    2 /* Public */,
       4,    0,  154,    2, 0x0a,    3 /* Public */,
       5,    0,  155,    2, 0x0a,    4 /* Public */,
       6,    1,  156,    2, 0x0a,    5 /* Public */,
       9,    0,  159,    2, 0x0a,    7 /* Public */,
      10,    0,  160,    2, 0x0a,    8 /* Public */,
      11,    0,  161,    2, 0x0a,    9 /* Public */,
      12,    3,  162,    2, 0x0a,   10 /* Public */,
      17,    3,  169,    2, 0x0a,   14 /* Public */,
      18,    0,  176,    2, 0x0a,   18 /* Public */,
      19,    0,  177,    2, 0x0a,   19 /* Public */,
      20,    0,  178,    2, 0x0a,   20 /* Public */,
      21,    0,  179,    2, 0x0a,   21 /* Public */,
      22,    0,  180,    2, 0x0a,   22 /* Public */,
      23,    0,  181,    2, 0x0a,   23 /* Public */,
      24,    0,  182,    2, 0x0a,   24 /* Public */,
      25,    0,  183,    2, 0x0a,   25 /* Public */,
      26,    0,  184,    2, 0x0a,   26 /* Public */,
      27,    0,  185,    2, 0x0a,   27 /* Public */,
      28,    0,  186,    2, 0x0a,   28 /* Public */,
      29,    0,  187,    2, 0x0a,   29 /* Public */,
      30,    1,  188,    2, 0x08,   30 /* Private */,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 7, 0x80000000 | 7,   14,   15,   16,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 7, 0x80000000 | 7,   14,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   31,

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
        // method 'getSelectedColumnIndex'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'addFretNumber'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goLeft'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goRight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'playColumn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getColumnInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopTempoTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changeTempoButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'changeTempoEdit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'insertSlideUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertSlideDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertHammerOn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertPullOff'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertBend'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertRelease'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertVibrato'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertMutedHit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertRest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertBarLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'undo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'remove'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
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
        case 0: { int _r = _t->getSelectedColumnIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->addFretNumber(); break;
        case 2: _t->goLeft(); break;
        case 3: _t->goRight(); break;
        case 4: _t->playTab((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 5: _t->playColumn(); break;
        case 6: _t->getColumnInfo(); break;
        case 7: _t->stopTempoTimer(); break;
        case 8: _t->changeTempoButton((*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[3]))); break;
        case 9: _t->changeTempoEdit((*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[3]))); break;
        case 10: _t->insertSlideUp(); break;
        case 11: _t->insertSlideDown(); break;
        case 12: _t->insertHammerOn(); break;
        case 13: _t->insertPullOff(); break;
        case 14: _t->insertBend(); break;
        case 15: _t->insertRelease(); break;
        case 16: _t->insertVibrato(); break;
        case 17: _t->insertMutedHit(); break;
        case 18: _t->insertRest(); break;
        case 19: _t->insertBarLine(); break;
        case 20: _t->undo(); break;
        case 21: _t->remove(); break;
        case 22: _t->selectColumn((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}
QT_WARNING_POP
