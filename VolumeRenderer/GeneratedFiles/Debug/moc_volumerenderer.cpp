/****************************************************************************
** Meta object code from reading C++ file 'volumerenderer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../volumerenderer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'volumerenderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VolumeRenderer_t {
    QByteArrayData data[15];
    char stringdata[389];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VolumeRenderer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VolumeRenderer_t qt_meta_stringdata_VolumeRenderer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VolumeRenderer"
QT_MOC_LITERAL(1, 15, 15), // "on_test_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 24), // "on_addDICOMFiles_clicked"
QT_MOC_LITERAL(4, 57, 30), // "on_isoLevelSlider_valueChanged"
QT_MOC_LITERAL(5, 88, 30), // "on_loadTestImageButton_clicked"
QT_MOC_LITERAL(6, 119, 31), // "on_loadTestImage2Button_clicked"
QT_MOC_LITERAL(7, 151, 26), // "on_loadDICOMButton_clicked"
QT_MOC_LITERAL(8, 178, 28), // "on_loadMFDICOMButton_clicked"
QT_MOC_LITERAL(9, 207, 28), // "on_loadDICOMFromFile_clicked"
QT_MOC_LITERAL(10, 236, 32), // "on_resolutionSlider_valueChanged"
QT_MOC_LITERAL(11, 269, 36), // "on_linearInterpolationButton_..."
QT_MOC_LITERAL(12, 306, 26), // "on_pointFlagButton_clicked"
QT_MOC_LITERAL(13, 333, 27), // "on_borderFlagButton_clicked"
QT_MOC_LITERAL(14, 361, 27) // "on_frameSlider_valueChanged"

    },
    "VolumeRenderer\0on_test_clicked\0\0"
    "on_addDICOMFiles_clicked\0"
    "on_isoLevelSlider_valueChanged\0"
    "on_loadTestImageButton_clicked\0"
    "on_loadTestImage2Button_clicked\0"
    "on_loadDICOMButton_clicked\0"
    "on_loadMFDICOMButton_clicked\0"
    "on_loadDICOMFromFile_clicked\0"
    "on_resolutionSlider_valueChanged\0"
    "on_linearInterpolationButton_clicked\0"
    "on_pointFlagButton_clicked\0"
    "on_borderFlagButton_clicked\0"
    "on_frameSlider_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VolumeRenderer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
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
    QMetaType::Void,

       0        // eod
};

void VolumeRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VolumeRenderer *_t = static_cast<VolumeRenderer *>(_o);
        switch (_id) {
        case 0: _t->on_test_clicked(); break;
        case 1: _t->on_addDICOMFiles_clicked(); break;
        case 2: _t->on_isoLevelSlider_valueChanged(); break;
        case 3: _t->on_loadTestImageButton_clicked(); break;
        case 4: _t->on_loadTestImage2Button_clicked(); break;
        case 5: _t->on_loadDICOMButton_clicked(); break;
        case 6: _t->on_loadMFDICOMButton_clicked(); break;
        case 7: _t->on_loadDICOMFromFile_clicked(); break;
        case 8: _t->on_resolutionSlider_valueChanged(); break;
        case 9: _t->on_linearInterpolationButton_clicked(); break;
        case 10: _t->on_pointFlagButton_clicked(); break;
        case 11: _t->on_borderFlagButton_clicked(); break;
        case 12: _t->on_frameSlider_valueChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject VolumeRenderer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VolumeRenderer.data,
      qt_meta_data_VolumeRenderer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VolumeRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VolumeRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VolumeRenderer.stringdata))
        return static_cast<void*>(const_cast< VolumeRenderer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int VolumeRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
