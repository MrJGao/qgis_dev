#include<qgsapplication.h>
#include <qgseditorwidgetregistry.h>

#include "qgis_dev.h"

int main( int argc, char *argv[] )
{
    QgsApplication myApp( argc, argv, true );
    QgsApplication::setPrefixPath( "C:/Program Files/qgis2.9.0", true );
    QgsApplication::initQgis();

    qgis_dev* myQgis = new qgis_dev();
    QgsEditorWidgetRegistry::initEditors( myQgis->mapCanvas() );
    myQgis->setWindowState( Qt::WindowMaximized );
    myQgis->show();

    //test attribute table
    myQgis->addVectorLayers();
    myQgis->openAttributeTableDialog();

    return myApp.exec();

}
