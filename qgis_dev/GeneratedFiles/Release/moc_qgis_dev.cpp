/****************************************************************************
** Meta object code from reading C++ file 'qgis_dev.h'
**
** Created: Tue Jan 19 20:52:10 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qgis_dev.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgis_dev.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qgis_dev[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      28,    9,    9,    9, 0x0a,
      53,   46,    9,    9, 0x0a,
      95,    9,    9,    9, 0x0a,
     144,  122,    9,    9, 0x0a,
     196,    9,    9,    9, 0x0a,
     214,    9,    9,    9, 0x0a,
     232,    9,    9,    9, 0x0a,
     246,    9,    9,    9, 0x08,
     276,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qgis_dev[] = {
    "qgis_dev\0\0addVectorLayers()\0"
    "addRasterLayers()\0layers\0"
    "autoSelectAddedLayer(QList<QgsMapLayer*>)\0"
    "openAttributeTableDialog()\0"
    "layer,featureRenderer\0"
    "changeSymbol(QgsVectorLayer*,QgsFeatureRendererV2*)\0"
    "layerSymbolTest()\0removeAllLayers()\0"
    "removeLayer()\0showMouseCoordinate(QgsPoint)\0"
    "createOverview()\0"
};

void qgis_dev::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qgis_dev *_t = static_cast<qgis_dev *>(_o);
        switch (_id) {
        case 0: _t->addVectorLayers(); break;
        case 1: _t->addRasterLayers(); break;
        case 2: _t->autoSelectAddedLayer((*reinterpret_cast< QList<QgsMapLayer*>(*)>(_a[1]))); break;
        case 3: _t->openAttributeTableDialog(); break;
        case 4: _t->changeSymbol((*reinterpret_cast< QgsVectorLayer*(*)>(_a[1])),(*reinterpret_cast< QgsFeatureRendererV2*(*)>(_a[2]))); break;
        case 5: _t->layerSymbolTest(); break;
        case 6: _t->removeAllLayers(); break;
        case 7: _t->removeLayer(); break;
        case 8: _t->showMouseCoordinate((*reinterpret_cast< const QgsPoint(*)>(_a[1]))); break;
        case 9: _t->createOverview(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qgis_dev::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qgis_dev::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qgis_dev,
      qt_meta_data_qgis_dev, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qgis_dev::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qgis_dev::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qgis_dev::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qgis_dev))
        return static_cast<void*>(const_cast< qgis_dev*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qgis_dev::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
