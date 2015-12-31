#include "qgis_devattrtabledialog.h"

#include <qgsattributetableview.h>

qgis_devattrtableDialog::qgis_devattrtableDialog( QgsVectorLayer* theVecLayer, QWidget *parent )
    : QDialog( parent )
    , mLayer( theVecLayer )
{
    ui.setupUi( this );

    setAttribute( Qt::WA_DeleteOnClose );

    QgsAttributeTableView *tv = new QgsAttributeTableView();
    ui.gridLayout->addWidget( tv );

}

qgis_devattrtableDialog::~qgis_devattrtableDialog()
{

}
