#include<qgsapplication.h>
#include <qgseditorwidgetregistry.h>

#include <QSettings>

#include "qgis_dev.h"

int main( int argc, char *argv[] )
{
    // 为了使用 QSettings
    QCoreApplication::setOrganizationName( "Jacory" );
    QCoreApplication::setOrganizationDomain( "jacory.com" );
    QCoreApplication::setApplicationName( "QGis_Dev" );

    QgsApplication myApp( argc, argv, true );
    QgsApplication::setPrefixPath( "C:/Program Files/qgis2.9.0", true );
    QgsApplication::initQgis();
    myApp.setDefaultSvgPaths( QStringList( "../images/svg" ) ); // svg file path

    qgis_dev* myQgis = new qgis_dev();
    QgsEditorWidgetRegistry::initEditors( myQgis->mapCanvas() ); // 初始化所有编辑器控件
    myQgis->setWindowState( Qt::WindowMaximized ); // 最大化程序窗口
    myQgis->show();

    return myApp.exec();
}
