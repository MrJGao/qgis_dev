/****************************************************************************
** Meta object code from reading C++ file 'qgis_devComposer.h'
**
** Created: Sun Jan 17 23:19:00 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qgis_devComposer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgis_devComposer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qgis_devComposer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   17,   17,   17, 0x0a,
      56,   17,   17,   17, 0x0a,
      85,   17,   17,   17, 0x0a,
     117,   17,   17,   17, 0x0a,
     145,   17,   17,   17, 0x0a,
     182,  174,   17,   17, 0x0a,
     216,  174,   17,   17, 0x0a,
     256,   17,   17,   17, 0x0a,
     286,   17,   17,   17, 0x0a,
     319,   17,   17,   17, 0x0a,
     351,   17,   17,   17, 0x0a,
     381,   17,   17,   17, 0x0a,
     409,   17,   17,   17, 0x0a,
     450,  438,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qgis_devComposer[] = {
    "qgis_devComposer\0\0zoomLevelChanged()\0"
    "toggleRulers(bool)\0on_actionZoomAll_triggered()\0"
    "on_actionZoomActual_triggered()\0"
    "on_actionZoomIn_triggered()\0"
    "on_actionZoomOut_triggered()\0checked\0"
    "on_actionShowGrid_triggered(bool)\0"
    "on_actionShowFullScreen_triggered(bool)\0"
    "on_actionAddArrow_triggered()\0"
    "on_actionPanComposer_triggered()\0"
    "on_actionSelectTool_triggered()\0"
    "on_actionAddShape_triggered()\0"
    "on_actionAddMap_triggered()\0"
    "on_actionAddHtml_triggered()\0composition\0"
    "setComposition(QgsComposition*)\0"
};

void qgis_devComposer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qgis_devComposer *_t = static_cast<qgis_devComposer *>(_o);
        switch (_id) {
        case 0: _t->zoomLevelChanged(); break;
        case 1: _t->toggleRulers((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_actionZoomAll_triggered(); break;
        case 3: _t->on_actionZoomActual_triggered(); break;
        case 4: _t->on_actionZoomIn_triggered(); break;
        case 5: _t->on_actionZoomOut_triggered(); break;
        case 6: _t->on_actionShowGrid_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_actionShowFullScreen_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_actionAddArrow_triggered(); break;
        case 9: _t->on_actionPanComposer_triggered(); break;
        case 10: _t->on_actionSelectTool_triggered(); break;
        case 11: _t->on_actionAddShape_triggered(); break;
        case 12: _t->on_actionAddMap_triggered(); break;
        case 13: _t->on_actionAddHtml_triggered(); break;
        case 14: _t->setComposition((*reinterpret_cast< QgsComposition*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qgis_devComposer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qgis_devComposer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qgis_devComposer,
      qt_meta_data_qgis_devComposer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qgis_devComposer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qgis_devComposer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qgis_devComposer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qgis_devComposer))
        return static_cast<void*>(const_cast< qgis_devComposer*>(this));
    if (!strcmp(_clname, "Ui_composerBase"))
        return static_cast< Ui_composerBase*>(const_cast< qgis_devComposer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qgis_devComposer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void qgis_devComposer::zoomLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
