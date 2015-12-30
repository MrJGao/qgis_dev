#include<qgsapplication.h>

#include "qgis_dev.h"

int main( int argc, char *argv[] )
{
    QgsApplication myApp( argc, argv, true );
    QgsApplication::setPrefixPath( "C:/Program Files/qgis2.9.0", true );
    QgsApplication::initQgis();

    qgis_dev myQgis = new qgis_dev();
    myQgis.show();
    return myApp.exec();

}