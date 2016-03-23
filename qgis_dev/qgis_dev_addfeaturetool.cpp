#include "qgis_dev_addfeaturetool.h"
#include "qgis_dev.h"
#include <QStringList>

#include <qgsvectorlayer.h>
#include <qgslogger.h>
#include <qgsvectordataprovider.h>
#include "qgscsexception.h"
#include "qgsproject.h"
#include "qgsmaplayerregistry.h"
#include "qgsmaptopixel.h"
#include "qgsgeometry.h"
#include "qgsfeature.h"


qgis_dev_addFeatureTool::qgis_dev_addFeatureTool( QgsMapCanvas* mapCanvas )
    : QgsMapTool( mapCanvas )
{
    mToolName = tr( "Add Feature" );
    mRubberBand = 0;
    mTempRubberBand = 0;
    mValidator = 0;
}

qgis_dev_addFeatureTool::~qgis_dev_addFeatureTool()
{

}


bool qgis_dev_addFeatureTool::addFeature( QgsVectorLayer *layer, QgsFeature *feature, bool showModal /*= true */ )
{
    if ( !layer || !layer->isEditable() ) {return false;}

    QgsAttributeMap defaultAttributes;

    QgsVectorDataProvider *provider = layer->dataProvider();

    QSettings settings;
    bool reuseLastValues = settings.value( "/qgis/digitizing/reuseLastValues", false ).toBool();
    QgsDebugMsg( QString( "reuseLastValues: %1" ).arg( reuseLastValues ) );

    // add the fields to the QgsFeature
    const QgsFields& fields = layer->pendingFields();
    feature->initAttributes( fields.count() );
    for ( int idx = 0; idx < fields.count(); ++idx )
    {
        QVariant v = provider->defaultValue( idx );
        feature->setAttribute( idx, v );
    }

    //show the dialog to enter attribute values
    //only show if enabled in settings and layer has fields
    bool isDisabledAttributeValuesDlg = ( fields.count() == 0 ) || settings.value( "/qgis/digitizing/disable_enter_attribute_values_dialog", false ).toBool();

    // override application-wide setting with any layer setting
    switch ( layer->featureFormSuppress() )
    {
    case QgsVectorLayer::SuppressOn:
        isDisabledAttributeValuesDlg = true;
        break;
    case QgsVectorLayer::SuppressOff:
        isDisabledAttributeValuesDlg = false;
        break;
    case QgsVectorLayer::SuppressDefault:
        break;
    }
    if ( isDisabledAttributeValuesDlg )
    {
        layer->beginEditCommand( "" );
        bool mFeatureSaved = layer->addFeature( *feature );

        if ( mFeatureSaved )
        {
            layer->endEditCommand();
        }
        else
        {
            layer->destroyEditCommand();
        }
    }
    else
    {
        // 这里添加代码，做增加要素时填写属性

    }
}

void qgis_dev_addFeatureTool::canvasReleaseEvent( QMouseEvent* e )
{
    // 获取当前图层
    QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( mCanvas->currentLayer() );

    // 判断当前图层是否为矢量图层
    if( !layer ) {emit messageEmitted( tr( "not a valid vector layer." ) ); return;}

    // 判断数据驱动状态
    QgsVectorDataProvider* provider = layer->dataProvider();
    if ( !( provider->capabilities() & QgsVectorDataProvider::AddFeatures ) )
    {
        emit messageEmitted(
            tr( "The data provider for this layer does not support the addition of features." ),
            QgsMessageBar::WARNING );
        return;
    }

    // 判断当前图层是否可编辑
    if( !layer->isEditable() ) {emit messageEmitted( tr( "can't edit this layer." ) ); return;}

    // 得到点坐标，转换为地图坐标
    QgsPoint savePoint;
    try
    {
        savePoint = toLayerCoordinates( layer, mCanvas->mapSettings().mapToPixel().toMapCoordinates( e->pos() ) );
        QgsDebugMsg( "savePoint = " + savePoint.toString() );
    }
    catch ( QgsCsException &cse )
    {
        Q_UNUSED( cse );
        emit messageEmitted( tr( "Cannot transform the point to the layers coordinate system" ), QgsMessageBar::WARNING );
        return;
    }


    switch( layer->geometryType() )
    {
    case QGis::Point:
        m_captureMode = CapturePoint;
        break;
    case QGis::Line:
        m_captureMode = CaptureLine;
        break;
    case QGis::Polygon:
        m_captureMode = CapturePolygon;
        break;
    default:
        break;
    }

    QgsGeometry* g = 0; // 新建一个geometry
    if ( m_captureMode == CapturePoint )
    {
        // 转换为geometry
        if ( layer->wkbType() == QGis::WKBPoint || layer->wkbType() == QGis::WKBPoint25D )
        {
            g = QgsGeometry::fromPoint( savePoint );
        }
        else if( layer->wkbType() == QGis::WKBMultiPoint || layer->wkbType() == QGis::WKBMultiPoint25D )
        {
            g = QgsGeometry::fromMultiPoint( QgsMultiPoint() << savePoint );
        }

        // 转换为feature
        QgsFeature feature( layer->pendingFields(), 0 );
        feature.setGeometry( g );
        addFeature( layer, &feature, false );
        //layer->addFeature( feature, true );
        mCanvas->setExtent( layer->extent() );
        mCanvas->refresh();
    }
    else if ( m_captureMode == CaptureLine || m_captureMode == CapturePolygon )
    {
        if ( e->button() == Qt::LeftButton ) // 鼠标左键
        {
            int error = addVertex( mCanvas->mapSettings().mapToPixel().toMapCoordinates( e->pos() ) );
            if ( error == 1 ) {return;} // current layer is not a vector layer
            else if ( error == 2 ) // problem with coordinate transformation
            {
                emit messageEmitted( tr( "Cannot transform the point to the layers coordinate system" ), QgsMessageBar::WARNING );
                return;
            }
            startCapturing();
        }
        else if ( e->button() == Qt::RightButton ) // 鼠标右键
        {
            deleteTempRubberBand();
            if ( m_captureMode == CaptureLine && m_captureList.size() < 2 ) { return; }
            if ( m_captureMode == CapturePolygon && m_captureList.size() < 3 ) { return; }

            QgsFeature* feature = new QgsFeature( layer->pendingFields(), 0 );
            QgsGeometry* g = 0; // 新建一个geometry
            if ( m_captureMode == CaptureLine )
            {
                if ( layer->wkbType() == QGis::WKBLineString || layer->wkbType() == QGis::WKBLineString25D )
                {
                    g = QgsGeometry::fromPolyline( m_captureList.toVector() );
                }
                else if ( layer->wkbType() == QGis::WKBMultiLineString || layer->wkbType() == QGis::WKBMultiLineString25D )
                {
                    g = QgsGeometry::fromMultiPolyline( QgsMultiPolyline() << m_captureList.toVector() );
                }
                else
                {
                    emit messageEmitted( tr( "Cannot add feature. Unknown WKB type" ), QgsMessageBar::CRITICAL );
                    stopCapturing();
                    delete feature;
                    return;
                }
                feature->setGeometry( g );
            }
            else if ( m_captureMode == CapturePolygon )
            {
                if ( layer->wkbType() == QGis::WKBPolygon ||  layer->wkbType() == QGis::WKBPolygon25D )
                {
                    g = QgsGeometry::fromPolygon( QgsPolygon() << m_captureList.toVector() );
                }
                else if ( layer->wkbType() == QGis::WKBMultiPolygon ||  layer->wkbType() == QGis::WKBMultiPolygon25D )
                {
                    g = QgsGeometry::fromMultiPolygon( QgsMultiPolygon() << ( QgsPolygon() << m_captureList.toVector() ) );
                }
                else
                {
                    emit messageEmitted( tr( "Cannot add feature. Unknown WKB type" ), QgsMessageBar::CRITICAL );
                    stopCapturing();
                    delete feature;
                    return;
                }

                if ( !g )
                {
                    stopCapturing();
                    delete feature;
                    return; // invalid geometry; one possibility is from duplicate points
                }
                feature->setGeometry( g );

                int avoidIntersectionsReturn = feature->geometry()->avoidIntersections();
                if ( avoidIntersectionsReturn == 1 )
                {
                    //not a polygon type. Impossible to get there
                }
#if 0
                else if ( avoidIntersectionsReturn == 2 ) //MH120131: disable this error message until there is a better way to cope with the single type / multi type problem
                {
                    //bail out...
                    emit messageEmitted( tr( "The feature could not be added because removing the polygon intersections would change the geometry type" ), QgsMessageBar::CRITICAL );
                    delete feature;
                    stopCapturing();
                    return;
                }
#endif
                else if ( avoidIntersectionsReturn == 3 )
                {
                    emit messageEmitted( tr( "An error was reported during intersection removal" ), QgsMessageBar::CRITICAL );
                }

                if ( !feature->geometry()->asWkb() ) //avoid intersection might have removed the whole geometry
                {
                    QString reason;
                    if ( avoidIntersectionsReturn != 2 )
                    {
                        reason = tr( "The feature cannot be added because it's geometry is empty" );
                    }
                    else
                    {
                        reason = tr( "The feature cannot be added because it's geometry collapsed due to intersection avoidance" );
                    }
                    emit messageEmitted( reason, QgsMessageBar::CRITICAL );
                    delete feature;
                    stopCapturing();
                    return;
                }
            }

            if ( addFeature( layer, feature, false ) )
            {
                //add points to other features to keep topology up-to-date
                int topologicalEditing = QgsProject::instance()->readNumEntry( "Digitizing", "/TopologicalEditing", 0 );

                //use always topological editing for avoidIntersection.
                //Otherwise, no way to guarantee the geometries don't have a small gap in between.
                QStringList intersectionLayers = QgsProject::instance()->readListEntry( "Digitizing", "/AvoidIntersectionsList" );
                bool avoidIntersection = !intersectionLayers.isEmpty();
                if ( avoidIntersection ) //try to add topological points also to background layers
                {
                    QStringList::const_iterator lIt = intersectionLayers.constBegin();
                    for ( ; lIt != intersectionLayers.constEnd(); ++lIt )
                    {
                        QgsMapLayer* ml = QgsMapLayerRegistry::instance()->mapLayer( *lIt );
                        QgsVectorLayer* vl = qobject_cast<QgsVectorLayer*>( ml );
                        //can only add topological points if background layer is editable...
                        if ( vl && vl->geometryType() == QGis::Polygon && vl->isEditable() )
                        {
                            vl->addTopologicalPoints( feature->geometry() );
                        }
                    }
                }
                else if ( topologicalEditing )
                {
                    layer->addTopologicalPoints( feature->geometry() );
                }
            }

            stopCapturing();
        }
    }
}

void qgis_dev_addFeatureTool::activate()
{
    QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( mCanvas->currentLayer() );
    if ( layer && layer->geometryType() == QGis::NoGeometry )
    {
        QgsFeature f;
        addFeature( layer, &f, false );
        return;
    }

    QgsMapTool::activate();
}

void qgis_dev_addFeatureTool::notifyNotVectorLayer()
{
    emit messageEmitted( tr( "No active vector layer" ) );
}

void qgis_dev_addFeatureTool::notifyNotEditableLayer()
{
    emit messageEmitted( tr( "Layer not editable" ) );
}

qgis_dev_addFeatureTool::CaptureMode qgis_dev_addFeatureTool::mode()
{
    return m_captureMode;
}

int qgis_dev_addFeatureTool::addVertex( const QgsPoint& point )
{
    if ( mode() == CaptureNone ) { QgsDebugMsg( "invalid capture mode" ); return 2;}

    QgsPoint layerPoint;
    int res = nextPoint( point, layerPoint );
    if ( res != 0 ) {return res;} // 当前点必须是最后一个点

    if ( !mRubberBand ) // 没有rubber band，就创建一个
    {
        mRubberBand = createRubberBand( m_captureMode == CapturePolygon ? QGis::Polygon : QGis::Line );
    }
    mRubberBand->addPoint( point );
    m_captureList.append( layerPoint );

    if ( !mTempRubberBand )
    {
        mTempRubberBand = createRubberBand( m_captureMode == CapturePolygon ? QGis::Polygon : QGis::Line, true );
    }
    else
    {
        mTempRubberBand->reset( m_captureMode == CapturePolygon ? true : false );
    }

    if ( m_captureMode == CaptureLine )
    {
        mTempRubberBand->addPoint( point );
    }
    else if ( m_captureMode == CapturePolygon )
    {
        const QgsPoint *firstPoint = mRubberBand->getPoint( 0, 0 );
        mTempRubberBand->addPoint( *firstPoint );
        mTempRubberBand->movePoint( point );
        mTempRubberBand->addPoint( point );
    }

    validateGeometry(); // 验证几何有效性

    return 0;
}

void qgis_dev_addFeatureTool::startCapturing()
{
    mCapturing = true;
}

void qgis_dev_addFeatureTool::deleteTempRubberBand()
{
    if ( mTempRubberBand )
    {
        delete mTempRubberBand;
        mTempRubberBand = 0;
    }
}

void qgis_dev_addFeatureTool::stopCapturing()
{
    if ( mRubberBand )
    {
        delete mRubberBand;
        mRubberBand = 0;
    }

    if ( mTempRubberBand )
    {
        delete mTempRubberBand;
        mTempRubberBand = 0;
    }

    while ( !mGeomErrorMarkers.isEmpty() )
    {
        delete mGeomErrorMarkers.takeFirst();
    }

    mGeomErrors.clear();

    mCapturing = false;
    m_captureList.clear();
    mCanvas->refresh();
}

int qgis_dev_addFeatureTool::nextPoint( const QgsPoint& mapPoint, QgsPoint& layerPoint )
{
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( mCanvas->currentLayer() );
    if ( !vlayer ) { QgsDebugMsg( "no vector layer" ); return 1;}
    try
    {
        layerPoint = toLayerCoordinates( vlayer, mapPoint ); //transform snapped point back to layer crs
    }
    catch ( QgsCsException &cse )
    {
        Q_UNUSED( cse );
        QgsDebugMsg( "transformation to layer coordinate failed" );
        return 2;
    }

    return 0;
}

QgsRubberBand* qgis_dev_addFeatureTool::createRubberBand( QGis::GeometryType geometryType /*= QGis::Line*/, bool alternativeBand /*= false */ )
{
    QSettings settings;
    QgsRubberBand* rb = new QgsRubberBand( mCanvas, geometryType );
    rb->setWidth( settings.value( "/qgis/digitizing/line_width", 1 ).toInt() );
    QColor color( settings.value( "/qgis/digitizing/line_color_red", 255 ).toInt(),
                  settings.value( "/qgis/digitizing/line_color_green", 0 ).toInt(),
                  settings.value( "/qgis/digitizing/line_color_blue", 0 ).toInt() );
    double myAlpha = settings.value( "/qgis/digitizing/line_color_alpha", 200 ).toInt() / 255.0;
    if ( alternativeBand )
    {
        myAlpha = myAlpha * settings.value( "/qgis/digitizing/line_color_alpha_scale", 0.75 ).toDouble();
        rb->setLineStyle( Qt::DotLine );
    }
    if ( geometryType == QGis::Polygon )
    {
        color.setAlphaF( myAlpha );
    }
    color.setAlphaF( myAlpha );
    rb->setColor( color );
    rb->show();
    return rb;
}

void qgis_dev_addFeatureTool::validateGeometry()
{
    QSettings settings;
    if ( settings.value( "/qgis/digitizing/validate_geometries", 1 ).toInt() == 0 ) {return;}

    if ( mValidator )
    {
        mValidator->deleteLater();
        mValidator = 0;
    }

    mTip = "";
    mGeomErrors.clear();
    while ( !mGeomErrorMarkers.isEmpty() )
    {
        delete mGeomErrorMarkers.takeFirst();
    }

    QgsGeometry *g = 0;
    switch ( m_captureMode )
    {
    case CaptureNone:
    case CapturePoint:
        return;

    case CaptureLine:
        if ( m_captureList.size() < 2 ) {return;}
        g = QgsGeometry::fromPolyline( m_captureList.toVector() );
        break;

    case CapturePolygon:
        if ( m_captureList.size() < 3 ) {return;}
        g = QgsGeometry::fromPolygon( QgsPolygon() << ( QgsPolyline() << m_captureList.toVector() << m_captureList[0] ) );
        break;
    }

    if ( !g ) {return;}

    mValidator = new QgsGeometryValidator( g );
    connect( mValidator, SIGNAL( errorFound( QgsGeometry::Error ) ), this, SLOT( addError( QgsGeometry::Error ) ) );
    connect( mValidator, SIGNAL( finished() ), this, SLOT( validationFinished() ) );
    mValidator->start();

    QStatusBar *sb = qgis_dev::instance()->statusBar();
    sb->showMessage( tr( "Validation started." ) );
    delete g;
}

