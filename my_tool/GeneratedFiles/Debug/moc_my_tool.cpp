/****************************************************************************
** Meta object code from reading C++ file 'my_tool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../my_tool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my_tool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_my_tool_t {
    QByteArrayData data[24];
    char stringdata0[269];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_my_tool_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_my_tool_t qt_meta_stringdata_my_tool = {
    {
QT_MOC_LITERAL(0, 0, 7), // "my_tool"
QT_MOC_LITERAL(1, 8, 10), // "ShowCamera"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 27), // "on_sniffStartButton_clicked"
QT_MOC_LITERAL(4, 48, 27), // "on_OpenWangKaButton_clicked"
QT_MOC_LITERAL(5, 76, 26), // "on_cutOutPicButton_clicked"
QT_MOC_LITERAL(6, 103, 7), // "Getdata"
QT_MOC_LITERAL(7, 111, 4), // "data"
QT_MOC_LITERAL(8, 116, 7), // "ifprint"
QT_MOC_LITERAL(9, 124, 10), // "pcap_if_t*"
QT_MOC_LITERAL(10, 135, 1), // "d"
QT_MOC_LITERAL(11, 137, 6), // "ip6tos"
QT_MOC_LITERAL(12, 144, 5), // "char*"
QT_MOC_LITERAL(13, 150, 9), // "sockaddr*"
QT_MOC_LITERAL(14, 160, 8), // "sockaddr"
QT_MOC_LITERAL(15, 169, 7), // "address"
QT_MOC_LITERAL(16, 177, 7), // "addrlen"
QT_MOC_LITERAL(17, 185, 5), // "iptos"
QT_MOC_LITERAL(18, 191, 6), // "u_long"
QT_MOC_LITERAL(19, 198, 2), // "in"
QT_MOC_LITERAL(20, 201, 17), // "stopNetActionSlot"
QT_MOC_LITERAL(21, 219, 17), // "mOnitorActionSlot"
QT_MOC_LITERAL(22, 237, 15), // "mStopActionSlot"
QT_MOC_LITERAL(23, 253, 15) // "mIcmpActionSlot"

    },
    "my_tool\0ShowCamera\0\0on_sniffStartButton_clicked\0"
    "on_OpenWangKaButton_clicked\0"
    "on_cutOutPicButton_clicked\0Getdata\0"
    "data\0ifprint\0pcap_if_t*\0d\0ip6tos\0char*\0"
    "sockaddr*\0sockaddr\0address\0addrlen\0"
    "iptos\0u_long\0in\0stopNetActionSlot\0"
    "mOnitorActionSlot\0mStopActionSlot\0"
    "mIcmpActionSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_my_tool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    0,   77,    2, 0x0a /* Public */,
       6,    1,   78,    2, 0x0a /* Public */,
       8,    1,   81,    2, 0x0a /* Public */,
      11,    3,   84,    2, 0x0a /* Public */,
      17,    1,   91,    2, 0x0a /* Public */,
      20,    0,   94,    2, 0x0a /* Public */,
      21,    0,   95,    2, 0x0a /* Public */,
      22,    0,   96,    2, 0x0a /* Public */,
      23,    0,   97,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    0x80000000 | 12, 0x80000000 | 13, 0x80000000 | 12, QMetaType::Int,   14,   15,   16,
    0x80000000 | 12, 0x80000000 | 18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void my_tool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        my_tool *_t = static_cast<my_tool *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowCamera(); break;
        case 1: _t->on_sniffStartButton_clicked(); break;
        case 2: _t->on_OpenWangKaButton_clicked(); break;
        case 3: _t->on_cutOutPicButton_clicked(); break;
        case 4: _t->Getdata((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->ifprint((*reinterpret_cast< pcap_if_t*(*)>(_a[1]))); break;
        case 6: { char* _r = _t->ip6tos((*reinterpret_cast< sockaddr*(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< char**>(_a[0]) = _r; }  break;
        case 7: { char* _r = _t->iptos((*reinterpret_cast< u_long(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< char**>(_a[0]) = _r; }  break;
        case 8: _t->stopNetActionSlot(); break;
        case 9: _t->mOnitorActionSlot(); break;
        case 10: _t->mStopActionSlot(); break;
        case 11: _t->mIcmpActionSlot(); break;
        default: ;
        }
    }
}

const QMetaObject my_tool::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_my_tool.data,
      qt_meta_data_my_tool,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *my_tool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *my_tool::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_my_tool.stringdata0))
        return static_cast<void*>(const_cast< my_tool*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int my_tool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
