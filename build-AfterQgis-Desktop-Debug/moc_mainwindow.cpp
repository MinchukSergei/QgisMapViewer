/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AfterQgis/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      49,   11,   11,   11, 0x0a,
      69,   58,   11,   11, 0x0a,
     103,   11,   11,   11, 0x0a,
     122,   11,   11,   11, 0x0a,
     164,  137,   11,   11, 0x0a,
     220,  209,   11,   11, 0x08,
     268,  254,   11,   11, 0x08,
     319,  309,  302,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0zoomInMode()\0zoomOutMode()\0"
    "panMode()\0locate()\0layer,name\0"
    "addLayer(QgsVectorLayer&,QString)\0"
    "addLocationLayer()\0addAllLayers()\0"
    "layer,isLocation,isVisible\0"
    "enableLayerLables(QgsVectorLayer*,bool,bool)\0"
    "layerNames\0readLayerNames(QVector<QString>&)\0"
    "layerLabeling\0readLayerLabeling(QVector<bool>&)\0"
    "double\0fMin,fMax\0fRand(double,double)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->zoomInMode(); break;
        case 1: _t->zoomOutMode(); break;
        case 2: _t->panMode(); break;
        case 3: _t->locate(); break;
        case 4: _t->addLayer((*reinterpret_cast< QgsVectorLayer(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->addLocationLayer(); break;
        case 6: _t->addAllLayers(); break;
        case 7: _t->enableLayerLables((*reinterpret_cast< QgsVectorLayer*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->readLayerNames((*reinterpret_cast< QVector<QString>(*)>(_a[1]))); break;
        case 9: _t->readLayerLabeling((*reinterpret_cast< QVector<bool>(*)>(_a[1]))); break;
        case 10: { double _r = _t->fRand((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
