/****************************************************************************
** Meta object code from reading C++ file 'qgis_devattrtabledialog.h'
**
** Created: Wed Jan 6 11:24:49 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qgis_devattrtabledialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgis_devattrtabledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qgis_devattrtableDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x0a,
      42,   24,   24,   24, 0x08,
      56,   24,   24,   24, 0x08,
      82,   24,   24,   24, 0x08,
      98,   24,   24,   24, 0x08,
     115,   24,   24,   24, 0x08,
     131,   24,   24,   24, 0x08,
     159,  146,   24,   24, 0x08,
     195,  189,   24,   24, 0x08,
     223,   24,   24,   24, 0x08,
     245,   24,   24,   24, 0x08,
     300,  261,   24,   24, 0x08,
     394,  367,   24,   24, 0x28,
     447,   24,   24,   24, 0x08,
     475,   24,   24,   24, 0x08,
     529,  511,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qgis_devattrtableDialog[] = {
    "qgis_devattrtableDialog\0\0editingToggled()\0"
    "updateTitle()\0filterExpressionBuilder()\0"
    "filterShowAll()\0filterSelected()\0"
    "filterVisible()\0filterEdited()\0"
    "filterAction\0filterColumnChanged(QObject*)\0"
    "query\0filterQueryChanged(QString)\0"
    "filterQueryAccepted()\0columnBoxInit()\0"
    "layer,fieldName,expression,filteredIds\0"
    "runFieldCalculation(QgsVectorLayer*,QString,QString,QgsFeatureIds)\0"
    "layer,fieldName,expression\0"
    "runFieldCalculation(QgsVectorLayer*,QString,QString)\0"
    "updateFieldFromExpression()\0"
    "updateFieldFromExpressionSelected()\0"
    "fieldName,isValid\0updateButtonStatus(QString,bool)\0"
};

void qgis_devattrtableDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        qgis_devattrtableDialog *_t = static_cast<qgis_devattrtableDialog *>(_o);
        switch (_id) {
        case 0: _t->editingToggled(); break;
        case 1: _t->updateTitle(); break;
        case 2: _t->filterExpressionBuilder(); break;
        case 3: _t->filterShowAll(); break;
        case 4: _t->filterSelected(); break;
        case 5: _t->filterVisible(); break;
        case 6: _t->filterEdited(); break;
        case 7: _t->filterColumnChanged((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 8: _t->filterQueryChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->filterQueryAccepted(); break;
        case 10: _t->columnBoxInit(); break;
        case 11: _t->runFieldCalculation((*reinterpret_cast< QgsVectorLayer*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QgsFeatureIds(*)>(_a[4]))); break;
        case 12: _t->runFieldCalculation((*reinterpret_cast< QgsVectorLayer*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 13: _t->updateFieldFromExpression(); break;
        case 14: _t->updateFieldFromExpressionSelected(); break;
        case 15: _t->updateButtonStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qgis_devattrtableDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qgis_devattrtableDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_qgis_devattrtableDialog,
      qt_meta_data_qgis_devattrtableDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qgis_devattrtableDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qgis_devattrtableDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qgis_devattrtableDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qgis_devattrtableDialog))
        return static_cast<void*>(const_cast< qgis_devattrtableDialog*>(this));
    if (!strcmp(_clname, "Ui::QgsAttributeTableDialog"))
        return static_cast< Ui::QgsAttributeTableDialog*>(const_cast< qgis_devattrtableDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int qgis_devattrtableDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
