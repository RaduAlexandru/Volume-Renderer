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
    QByteArrayData data[38];
    char stringdata[1049];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VolumeRenderer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VolumeRenderer_t qt_meta_stringdata_VolumeRenderer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VolumeRenderer"
QT_MOC_LITERAL(1, 15, 26), // "progressValueChangedSignal"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 8), // "newValue"
QT_MOC_LITERAL(4, 52, 24), // "generatingFinishedSignal"
QT_MOC_LITERAL(5, 77, 23), // "generatingStartedSignal"
QT_MOC_LITERAL(6, 101, 19), // "dataFinishedReading"
QT_MOC_LITERAL(7, 121, 30), // "on_isoLevelSlider_valueChanged"
QT_MOC_LITERAL(8, 152, 28), // "on_loadDICOMFromFile_clicked"
QT_MOC_LITERAL(9, 181, 32), // "on_resolutionSlider_valueChanged"
QT_MOC_LITERAL(10, 214, 41), // "on_linearInterpolationSlider_..."
QT_MOC_LITERAL(11, 256, 31), // "on_toleranceSlider_valueChanged"
QT_MOC_LITERAL(12, 288, 30), // "on_marchingCubesButton_clicked"
QT_MOC_LITERAL(13, 319, 39), // "on_adaptiveMarchingCubes2Butt..."
QT_MOC_LITERAL(14, 359, 39), // "on_adaptiveMarchingCubes3Butt..."
QT_MOC_LITERAL(15, 399, 35), // "on_normalsPerTriangleButton_c..."
QT_MOC_LITERAL(16, 435, 34), // "on_normalsPerVerticeButton_cl..."
QT_MOC_LITERAL(17, 470, 29), // "on_orientationZButton_clicked"
QT_MOC_LITERAL(18, 500, 29), // "on_orientationXButton_clicked"
QT_MOC_LITERAL(19, 530, 29), // "on_orientationYButton_clicked"
QT_MOC_LITERAL(20, 560, 29), // "on_generateMeshButton_clicked"
QT_MOC_LITERAL(21, 590, 28), // "on_interactiveButton_clicked"
QT_MOC_LITERAL(22, 619, 33), // "on_octreeDepthSlider_valueCha..."
QT_MOC_LITERAL(23, 653, 35), // "on_borderYBottomSlider_valueC..."
QT_MOC_LITERAL(24, 689, 32), // "on_borderYTopSlider_valueChanged"
QT_MOC_LITERAL(25, 722, 33), // "on_borderXLeftSlider_valueCha..."
QT_MOC_LITERAL(26, 756, 34), // "on_borderXRightSlider_valueCh..."
QT_MOC_LITERAL(27, 791, 24), // "on_loadObjButton_clicked"
QT_MOC_LITERAL(28, 816, 25), // "on_writeObjButton_clicked"
QT_MOC_LITERAL(29, 842, 24), // "progressValueChangedSlot"
QT_MOC_LITERAL(30, 867, 16), // "progressTextSlot"
QT_MOC_LITERAL(31, 884, 16), // "finishedMeshSlot"
QT_MOC_LITERAL(32, 901, 19), // "finishedNormalsSlot"
QT_MOC_LITERAL(33, 921, 23), // "dataFinishedReadingSlot"
QT_MOC_LITERAL(34, 945, 32), // "on_saveToSecondaryButton_clicked"
QT_MOC_LITERAL(35, 978, 25), // "on_clearPinButton_clicked"
QT_MOC_LITERAL(36, 1004, 21), // "on_moveButton_clicked"
QT_MOC_LITERAL(37, 1026, 22) // "on_scaleButton_clicked"

    },
    "VolumeRenderer\0progressValueChangedSignal\0"
    "\0newValue\0generatingFinishedSignal\0"
    "generatingStartedSignal\0dataFinishedReading\0"
    "on_isoLevelSlider_valueChanged\0"
    "on_loadDICOMFromFile_clicked\0"
    "on_resolutionSlider_valueChanged\0"
    "on_linearInterpolationSlider_valueChanged\0"
    "on_toleranceSlider_valueChanged\0"
    "on_marchingCubesButton_clicked\0"
    "on_adaptiveMarchingCubes2Button_clicked\0"
    "on_adaptiveMarchingCubes3Button_clicked\0"
    "on_normalsPerTriangleButton_clicked\0"
    "on_normalsPerVerticeButton_clicked\0"
    "on_orientationZButton_clicked\0"
    "on_orientationXButton_clicked\0"
    "on_orientationYButton_clicked\0"
    "on_generateMeshButton_clicked\0"
    "on_interactiveButton_clicked\0"
    "on_octreeDepthSlider_valueChanged\0"
    "on_borderYBottomSlider_valueChanged\0"
    "on_borderYTopSlider_valueChanged\0"
    "on_borderXLeftSlider_valueChanged\0"
    "on_borderXRightSlider_valueChanged\0"
    "on_loadObjButton_clicked\0"
    "on_writeObjButton_clicked\0"
    "progressValueChangedSlot\0progressTextSlot\0"
    "finishedMeshSlot\0finishedNormalsSlot\0"
    "dataFinishedReadingSlot\0"
    "on_saveToSecondaryButton_clicked\0"
    "on_clearPinButton_clicked\0"
    "on_moveButton_clicked\0on_scaleButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VolumeRenderer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  189,    2, 0x06 /* Public */,
       4,    0,  192,    2, 0x06 /* Public */,
       5,    0,  193,    2, 0x06 /* Public */,
       6,    0,  194,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  195,    2, 0x08 /* Private */,
       8,    0,  196,    2, 0x08 /* Private */,
       9,    0,  197,    2, 0x08 /* Private */,
      10,    0,  198,    2, 0x08 /* Private */,
      11,    0,  199,    2, 0x08 /* Private */,
      12,    0,  200,    2, 0x08 /* Private */,
      13,    0,  201,    2, 0x08 /* Private */,
      14,    0,  202,    2, 0x08 /* Private */,
      15,    0,  203,    2, 0x08 /* Private */,
      16,    0,  204,    2, 0x08 /* Private */,
      17,    0,  205,    2, 0x08 /* Private */,
      18,    0,  206,    2, 0x08 /* Private */,
      19,    0,  207,    2, 0x08 /* Private */,
      20,    0,  208,    2, 0x08 /* Private */,
      21,    0,  209,    2, 0x08 /* Private */,
      22,    0,  210,    2, 0x08 /* Private */,
      23,    0,  211,    2, 0x08 /* Private */,
      24,    0,  212,    2, 0x08 /* Private */,
      25,    0,  213,    2, 0x08 /* Private */,
      26,    0,  214,    2, 0x08 /* Private */,
      27,    0,  215,    2, 0x08 /* Private */,
      28,    0,  216,    2, 0x08 /* Private */,
      29,    1,  217,    2, 0x08 /* Private */,
      30,    1,  220,    2, 0x08 /* Private */,
      31,    0,  223,    2, 0x08 /* Private */,
      32,    0,  224,    2, 0x08 /* Private */,
      33,    0,  225,    2, 0x08 /* Private */,
      34,    0,  226,    2, 0x08 /* Private */,
      35,    0,  227,    2, 0x08 /* Private */,
      36,    0,  228,    2, 0x08 /* Private */,
      37,    0,  229,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    2,
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
        case 0: _t->progressValueChangedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->generatingFinishedSignal(); break;
        case 2: _t->generatingStartedSignal(); break;
        case 3: _t->dataFinishedReading(); break;
        case 4: _t->on_isoLevelSlider_valueChanged(); break;
        case 5: _t->on_loadDICOMFromFile_clicked(); break;
        case 6: _t->on_resolutionSlider_valueChanged(); break;
        case 7: _t->on_linearInterpolationSlider_valueChanged(); break;
        case 8: _t->on_toleranceSlider_valueChanged(); break;
        case 9: _t->on_marchingCubesButton_clicked(); break;
        case 10: _t->on_adaptiveMarchingCubes2Button_clicked(); break;
        case 11: _t->on_adaptiveMarchingCubes3Button_clicked(); break;
        case 12: _t->on_normalsPerTriangleButton_clicked(); break;
        case 13: _t->on_normalsPerVerticeButton_clicked(); break;
        case 14: _t->on_orientationZButton_clicked(); break;
        case 15: _t->on_orientationXButton_clicked(); break;
        case 16: _t->on_orientationYButton_clicked(); break;
        case 17: _t->on_generateMeshButton_clicked(); break;
        case 18: _t->on_interactiveButton_clicked(); break;
        case 19: _t->on_octreeDepthSlider_valueChanged(); break;
        case 20: _t->on_borderYBottomSlider_valueChanged(); break;
        case 21: _t->on_borderYTopSlider_valueChanged(); break;
        case 22: _t->on_borderXLeftSlider_valueChanged(); break;
        case 23: _t->on_borderXRightSlider_valueChanged(); break;
        case 24: _t->on_loadObjButton_clicked(); break;
        case 25: _t->on_writeObjButton_clicked(); break;
        case 26: _t->progressValueChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->progressTextSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->finishedMeshSlot(); break;
        case 29: _t->finishedNormalsSlot(); break;
        case 30: _t->dataFinishedReadingSlot(); break;
        case 31: _t->on_saveToSecondaryButton_clicked(); break;
        case 32: _t->on_clearPinButton_clicked(); break;
        case 33: _t->on_moveButton_clicked(); break;
        case 34: _t->on_scaleButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VolumeRenderer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRenderer::progressValueChangedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (VolumeRenderer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRenderer::generatingFinishedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (VolumeRenderer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRenderer::generatingStartedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (VolumeRenderer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRenderer::dataFinishedReading)) {
                *result = 3;
            }
        }
    }
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
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void VolumeRenderer::progressValueChangedSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VolumeRenderer::generatingFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void VolumeRenderer::generatingStartedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void VolumeRenderer::dataFinishedReading()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
