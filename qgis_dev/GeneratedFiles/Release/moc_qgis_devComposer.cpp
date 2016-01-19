/****************************************************************************
** Meta object code from reading C++ file 'qgis_devComposer.h'
**
** Created: Tue Jan 19 20:08:42 2016
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
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   17,   17,   17, 0x0a,
      70,   17,   17,   17, 0x0a,
      99,   17,   17,   17, 0x0a,
     131,   17,   17,   17, 0x0a,
     159,   17,   17,   17, 0x0a,
     196,  188,   17,   17, 0x0a,
     230,   17,   17,   17, 0x0a,
     268,  188,   17,   17, 0x0a,
     308,   17,   17,   17, 0x0a,
     340,   17,   17,   17, 0x0a,
     370,   17,   17,   17, 0x0a,
     398,   17,   17,   17, 0x0a,
     428,   17,   17,   17, 0x0a,
     457,   17,   17,   17, 0x0a,
     492,   17,   17,   17, 0x0a,
     525,   17,   17,   17, 0x0a,
     570,  558,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qgis_devComposer[] = {
    "qgis_devComposer\0\0zoomLevelChanged()\0"
    "on_actionPanComposer_triggered()\0"
    "on_actionZoomAll_triggered()\0"
    "on_actionZoomActual_triggered()\0"
    "on_actionZoomIn_triggered()\0"
    "on_actionZoomOut_triggered()\0checked\0"
    "on_actionShowGrid_triggered(bool)\0"
    "on_actionToggleRulers_triggered(bool)\0"
    "on_actionShowFullScreen_triggered(bool)\0"
    "on_actionSelectTool_triggered()\0"
    "on_actionAddShape_triggered()\0"
    "on_actionAddMap_triggered()\0"
    "on_actionAddArrow_triggered()\0"
    "on_actionAddHtml_triggered()\0"
    "on_actionExportAsImage_triggered()\0"
    "on_actionExportAsSvg_triggered()\0"
    "on_actionExportAsPDF_triggered()\0"
    "composition\0setComposition(QgsComposition*)\0"
};

void qgis_devComposer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qgis_devComposer *_t = static_cast<qgis_devComposer *>(_o);
        switch (_id) {
        case 0: _t->zoomLevelChanged(); break;
        case 1: _t->on_actionPanComposer_triggered(); break;
        case 2: _t->on_actionZoomAll_triggered(); break;
        case 3: _t->on_actionZoomActual_triggered(); break;
        case 4: _t->on_actionZoomIn_triggered(); break;
        case 5: _t->on_actionZoomOut_triggered(); break;
        case 6: _t->on_actionShowGrid_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_actionToggleRulers_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_actionShowFullScreen_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_actionSelectTool_triggered(); break;
        case 10: _t->on_actionAddShape_triggered(); break;
        case 11: _t->on_actionAddMap_triggered(); break;
        case 12: _t->on_actionAddArrow_triggered(); break;
        case 13: _t->on_actionAddHtml_triggered(); break;
        case 14: _t->on_actionExportAsImage_triggered(); break;
        case 15: _t->on_actionExportAsSvg_triggered(); break;
        case 16: _t->on_actionExportAsPDF_triggered(); break;
        case 17: _t->setComposition((*reinterpret_cast< QgsComposition*(*)>(_a[1]))); break;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void qgis_devComposer::zoomLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
