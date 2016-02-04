#include "qgis_devmaptoolidentifyaction.h"
#include "qgis_dev.h"

#include <QMessageBox>
#include <QList>

qgis_devMapToolIdentifyAction::qgis_devMapToolIdentifyAction( QgsMapCanvas * canvas )
    : QgsMapToolIdentify( canvas )
{

}

qgis_devMapToolIdentifyAction::~qgis_devMapToolIdentifyAction()
{

}

void qgis_devMapToolIdentifyAction::canvasReleaseEvent( QMouseEvent * e )
{
    IdentifyMode mode = QgsMapToolIdentify::LayerSelection; // 控制识别模式
    QList<IdentifyResult> results = QgsMapToolIdentify::identify( e->x(), e->y(), mode ); // 这句返回识别结果

    if ( results.isEmpty() )
    {
        qgis_dev::instance()->statusBar()->showMessage( tr( "No features at this position found." ) );
    }
    else
    {
        // 显示出识别结果，这里仅作示例，结果的展示方式可以自定义
        IdentifyResult feature = results.at( 0 );
        QString title = feature.mLayer->name();
        QString content = feature.mFeature.attribute( 1 ).toString();
        // 显示识别窗口
        QMessageBox::critical( NULL,
                               title,
                               content );
    }
}

