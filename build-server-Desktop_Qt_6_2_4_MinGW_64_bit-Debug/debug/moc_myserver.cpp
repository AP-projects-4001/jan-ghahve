/****************************************************************************
** Meta object code from reading C++ file 'myserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../server/myserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyServer_t {
    const uint offsetsAndSize[32];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyServer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyServer_t qt_meta_stringdata_MyServer = {
    {
QT_MOC_LITERAL(0, 8), // "MyServer"
QT_MOC_LITERAL(9, 18), // "on_thread_finished"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 7), // "qintptr"
QT_MOC_LITERAL(37, 16), // "socketdiscriptor"
QT_MOC_LITERAL(54, 2), // "id"
QT_MOC_LITERAL(57, 19), // "on_message_recieved"
QT_MOC_LITERAL(77, 8), // "senderId"
QT_MOC_LITERAL(86, 10), // "recieverId"
QT_MOC_LITERAL(97, 7), // "message"
QT_MOC_LITERAL(105, 25), // "on_message_group_recieved"
QT_MOC_LITERAL(131, 6), // "chatId"
QT_MOC_LITERAL(138, 11), // "recieverIds"
QT_MOC_LITERAL(150, 21), // "on_user_authenticated"
QT_MOC_LITERAL(172, 27), // "on_group_or_channel_created"
QT_MOC_LITERAL(200, 3) // "ids"

    },
    "MyServer\0on_thread_finished\0\0qintptr\0"
    "socketdiscriptor\0id\0on_message_recieved\0"
    "senderId\0recieverId\0message\0"
    "on_message_group_recieved\0chatId\0"
    "recieverIds\0on_user_authenticated\0"
    "on_group_or_channel_created\0ids"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x08,    1 /* Private */,
       6,    3,   49,    2, 0x08,    4 /* Private */,
      10,    4,   56,    2, 0x08,    8 /* Private */,
      13,    2,   65,    2, 0x08,   13 /* Private */,
      14,    2,   70,    2, 0x08,   16 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QStringList, QMetaType::QString,    7,   11,   12,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    5,   15,

       0        // eod
};

void MyServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_thread_finished((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->on_message_recieved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->on_message_group_recieved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 3: _t->on_user_authenticated((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->on_group_or_channel_created((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject MyServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_MyServer.offsetsAndSize,
    qt_meta_data_MyServer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyServer_t
, QtPrivate::TypeAndForceComplete<MyServer, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qintptr, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QStringList, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<qintptr, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QStringList, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int MyServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
