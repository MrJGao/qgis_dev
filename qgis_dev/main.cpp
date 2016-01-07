#include<qgsapplication.h>

#include "qgis_dev.h"

int main( int argc, char *argv[] )
{
    QgsApplication myApp( argc, argv, true );
    QgsApplication::setPrefixPath( "C:/Program Files/qgis2.9.0", true );
    QgsApplication::initQgis();
    myApp.setDefaultSvgPaths( QStringList( "../images/svg" ) ); // svg file path

    qgis_dev* myQgis = new qgis_dev();
    myQgis->setWindowState( Qt::WindowMaximized );
    myQgis->show();

    //test attribute table
    myQgis->addVectorLayers();
    myQgis->openAttributeTableDialog();

    // test vector layer symbol
    /*myQgis->layerSymbolTest();*/

    return myApp.exec();
}
