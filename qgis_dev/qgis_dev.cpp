#include "qgis_dev.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QFont>
#include <QLineEdit>
#include <QToolButton>
#include <QMargins>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <Qt>
#include <QBitmap>

// QGis include
#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>
#include <qgsrasterlayer.h>
#include <qgsproject.h>
#include <qgslayertreemodel.h>
#include <qgsapplication.h>
#include <qgslayertreelayer.h>
#include <qgslayertreegroup.h>
#include <qgslayertreeregistrybridge.h>
#include <qgslayertreeviewdefaultactions.h>
#include <qgsattributedialog.h>
#include <qgscursors.h>

#include "qgis_devlayertreeviewmenuprovider.h"
#include "qgis_devattrtabledialog.h"

// for attribute table
#include <qgsfeaturelistview.h>
#include <qgsattributetableview.h>
#include <qgsattributetablemodel.h>
#include <qgsfeaturelistmodel.h>
#include <qgsvectorlayercache.h>

#include <qgsattributedialog.h>
#include <qgseditorwidgetfactory.h>


// for layer symbol
#include <qgssymbollayerv2.h>
#include <qgssymbolv2.h>
#include <qgsmarkersymbollayerv2.h>
#include <qgsvectorlayerrenderer.h>
#include <qgsrendercontext.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgssymbollayerv2.h>
#include <qgsmapcanvas.h>
#include <qgsmapoverviewcanvas.h>

qgis_dev* qgis_dev::sm_instance = 0;

qgis_dev::qgis_dev( QWidget *parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags ),
      m_MousePrecisionDecimalPlaces( 0 )
{
    if ( sm_instance )
    {
        QMessageBox::critical(
            this,
            tr( "Multiple Instances of QgisApp" ),
            tr( "Multiple instances of QGIS application object detected.\nPlease contact the developers.\n" ) );
        // abort();
    }
    sm_instance = this;

    ui.setupUi( this );

    //! 初始化map canvas
    m_mapCanvas = new QgsMapCanvas();
    m_mapCanvas->enableAntiAliasing( true );
    m_mapCanvas->setCanvasColor( QColor( 255, 255, 255 ) );

    //! 构建打印出图视图
    m_composer = new qgis_devComposer( this );
    createComposer();

    //! 初始化图层管理器
    m_layerTreeView = new QgsLayerTreeView( this );
    initLayerTreeView();

    //! 布局
    QWidget* centralWidget = this->centralWidget();
    QGridLayout* centralLayout = new QGridLayout( centralWidget );

    m_stackedWidget = new QStackedWidget( this );
    m_stackedWidget->setLayout( new QHBoxLayout() );
    m_stackedWidget->addWidget( m_mapCanvas );
    m_stackedWidget->addWidget( m_composer );

    centralLayout->addWidget( m_stackedWidget, 0, 0, 1, 1 );

    //! 初始化status bar
    createStatusBar();

    // connections
    connect( m_mapCanvas, SIGNAL( xyCoordinates( const QgsPoint& ) ), this, SLOT( showMouseCoordinate( const QgsPoint& ) ) );
    connect( ui.actionAdd_Vector, SIGNAL( triggered() ), this, SLOT( addVectorLayers() ) );
    connect( ui.actionAdd_Raster, SIGNAL( triggered() ), this, SLOT( addRasterLayers() ) );
    connect( ui.actionToggle_Overview, SIGNAL( triggered() ), this, SLOT( createOverview() ) );
    connect( m_mapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );
}

qgis_dev::~qgis_dev()
{
    delete sm_instance;
}

void qgis_dev::addVectorLayers()
{
    QString filename = QFileDialog::getOpenFileName( this, tr( "open vector" ), "", "*.shp" );
    if ( filename == QString::null ) { return;}

    QFileInfo fi( filename );
    QString basename = fi.baseName();
    QgsVectorLayer* vecLayer = new QgsVectorLayer( filename, basename, "ogr", false );
    if ( !vecLayer->isValid() )
    {
        QMessageBox::critical( this, "error", "layer is invalid" );
        return;
    }

    QgsMapLayerRegistry::instance()->addMapLayer( vecLayer );
    mapCanvasLayerSet.append( vecLayer );
    m_mapCanvas->setExtent( vecLayer->extent() );
    m_mapCanvas->setLayerSet( mapCanvasLayerSet );
    m_mapCanvas->setVisible( true );
    m_mapCanvas->freeze( false );
    m_mapCanvas->refresh();
}

void qgis_dev::addRasterLayers()
{
    QString filename = QFileDialog::getOpenFileName( this, tr( "open vector" ), "", "*.tif" );

    QFileInfo fi( filename );
    QString basename = fi.baseName();
    QgsRasterLayer* rasterLayer = new QgsRasterLayer( filename, basename, "gdal", false );
    if ( !rasterLayer->isValid() )
    {
        QMessageBox::critical( this, "error", "layer is invalid" );
        return;
    }

    QgsMapLayerRegistry::instance()->addMapLayer( rasterLayer );
    mapCanvasLayerSet.append( rasterLayer );
    m_mapCanvas->setExtent( rasterLayer->extent() );
    m_mapCanvas->setLayerSet( mapCanvasLayerSet );
    m_mapCanvas->setVisible( true );
    m_mapCanvas->freeze( false );
    m_mapCanvas->refresh();
}

void qgis_dev::initLayerTreeView()
{
    QgsLayerTreeModel* model = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), this );
    model->setFlag( QgsLayerTreeModel::AllowNodeRename );
    model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
    model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
    model->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
    model->setAutoCollapseLegendNodes( 10 );

    m_layerTreeView->setModel( model );

    // 添加右键菜单
    m_layerTreeView->setMenuProvider( new qgis_devLayerTreeViewMenuProvider( m_layerTreeView, m_mapCanvas ) );

    connect( QgsProject::instance()->layerTreeRegistryBridge(), SIGNAL( addedLayersToLayerTree( QList<QgsMapLayer*> ) ),
             this, SLOT( autoSelectAddedLayer( QList<QgsMapLayer*> ) ) );

    // add group tool button
    QToolButton * btnAddGroup = new QToolButton();
    btnAddGroup->setAutoRaise( true );
    btnAddGroup->setIcon( qgis_dev::getThemeIcon( "mActionAdd.png" ) );

    btnAddGroup->setToolTip( tr( "Add Group" ) );
    connect( btnAddGroup, SIGNAL( clicked() ), m_layerTreeView->defaultActions(), SLOT( addGroup() ) );

    // expand / collapse tool buttons
    QToolButton* btnExpandAll = new QToolButton();
    btnExpandAll->setAutoRaise( true );
    btnExpandAll->setIcon( qgis_dev::getThemeIcon( "mActionExpandTree.png" ) );
    btnExpandAll->setToolTip( tr( "Expand All" ) );
    connect( btnExpandAll, SIGNAL( clicked() ), m_layerTreeView, SLOT( expandAll() ) );

    QToolButton* btnCollapseAll = new QToolButton();
    btnCollapseAll->setAutoRaise( true );
    btnCollapseAll->setIcon( qgis_dev::getThemeIcon( "mActionCollapseTree.png" ) );
    btnCollapseAll->setToolTip( tr( "Collapse All" ) );
    connect( btnCollapseAll, SIGNAL( clicked() ), m_layerTreeView, SLOT( collapseAll() ) );

    // remove item button
    QToolButton* btnRemoveItem = new QToolButton();
    btnRemoveItem->setIcon( qgis_dev::getThemeIcon( "mActionRemoveLayer.svg" ) );
    btnRemoveItem->setAutoRaise( true );
    connect( btnRemoveItem, SIGNAL( clicked() ), this, SLOT( removeLayer() ) );

    // 按钮布局
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    toolbarLayout->setContentsMargins( QMargins( 5, 0, 5, 0 ) );
    toolbarLayout->addWidget( btnAddGroup );
    toolbarLayout->addWidget( btnCollapseAll );
    toolbarLayout->addWidget( btnExpandAll );
    toolbarLayout->addWidget( btnRemoveItem );
    toolbarLayout->addStretch();

    QVBoxLayout* vboxLayout = new QVBoxLayout();
    vboxLayout->setMargin( 0 );
    vboxLayout->addLayout( toolbarLayout );
    vboxLayout->addWidget( m_layerTreeView );

    // 装进dock widget中
    m_layerTreeDock = new QDockWidget( tr( "Layers" ), this );
    m_layerTreeDock->setObjectName( "Layers" );
    m_layerTreeDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

    QWidget* w = new QWidget();
    w->setLayout( vboxLayout );
    m_layerTreeDock->setWidget( w );
    addDockWidget( Qt::LeftDockWidgetArea, m_layerTreeDock );

    // 连接地图画布和图层管理器
    m_layerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), m_mapCanvas, this );
    connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ),
             m_layerTreeCanvasBridge, SLOT( writeProject( QDomDocument& ) ) );
    connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ),
             m_layerTreeCanvasBridge, SLOT( readProject( QDomDocument ) ) );


}

void qgis_dev::createStatusBar()
{
    statusBar()->setStyleSheet( "QStatusBar::item {border: none;}" );

    //! 添加地图和打印视图切换的ComboBox
    pageViewComboBox = new QComboBox( statusBar() );
    pageViewComboBox->addItem( tr( "Map" ) );
    pageViewComboBox->addItem( tr( "Composer" ) );
    connect( pageViewComboBox, SIGNAL( activated( int ) ), m_stackedWidget, SLOT( setCurrentIndex( int ) ) );
    statusBar()->addWidget( pageViewComboBox );

    //! 添加进度条
    m_progressBar = new QProgressBar( statusBar() );
    m_progressBar->setObjectName( "m_progressBar" );
    m_progressBar->setMaximum( 100 );
    m_progressBar->hide();
    statusBar()->addPermanentWidget( m_progressBar, 1 );
    connect( m_mapCanvas, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( m_mapCanvas, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );

    QFont myFont( "Arial", 9 );
    statusBar()->setFont( myFont );

    //! 添加坐标显示格式选择
    QComboBox* coordFormatComboBox = new QComboBox( statusBar() );
    coordFormatComboBox->addItem( tr( "Decimal Degree" ) );
    coordFormatComboBox->addItem( tr( "Degree" ) );
    statusBar()->addPermanentWidget( coordFormatComboBox );
    connect( coordFormatComboBox, SIGNAL( activated( int ) ), this, SLOT( mapUnitChange( int ) ) );

    //! 添加坐标显示标签
    m_coordsLabel = new QLabel( QString(), statusBar() );
    m_coordsLabel->setObjectName( "m_coordsLabel" );
    m_coordsLabel->setFont( myFont );
    m_coordsLabel->setMinimumWidth( 10 );
    m_coordsLabel->setMargin( 3 );
    m_coordsLabel->setAlignment( Qt::AlignCenter );
    m_coordsLabel->setFrameStyle( QFrame::NoFrame );
    m_coordsLabel->setText( tr( "Coordinate:" ) );
    m_coordsLabel->setToolTip( tr( "Current map coordinate" ) );
    statusBar()->addPermanentWidget( m_coordsLabel, 0 );

    //! 坐标编辑标签
    m_coordsEdit = new QLineEdit( QString(), statusBar() );
    m_coordsEdit->setObjectName( "m_coordsEdit" );
    m_coordsEdit->setFont( myFont );
    m_coordsEdit->setMinimumWidth( 10 );
    m_coordsEdit->setMaximumWidth( 300 );
    m_coordsEdit->setContentsMargins( 0, 0, 0, 0 );
    m_coordsEdit->setAlignment( Qt::AlignCenter );
    statusBar()->addPermanentWidget( m_coordsEdit, 0 );
    //m_coordsEdit->setReadOnly( true );
    connect( m_coordsEdit, SIGNAL( returnPressed() ), this, SLOT( userCenter() ) );

    m_dizzyTimer = new QTimer( this );
    connect( m_dizzyTimer, SIGNAL( timeout() ), this, SLOT( dizzy() ) );

    //! 比例尺标签
    m_scaleLabel = new QLabel( QString(), statusBar() );
    m_scaleLabel->setObjectName( "m_scaleLabel" );
    m_scaleLabel->setFont( myFont );
    m_scaleLabel->setMinimumWidth( 10 );
    m_scaleLabel->setMargin( 3 );
    m_scaleLabel->setAlignment( Qt::AlignCenter );
    m_scaleLabel->setFrameStyle( QFrame::NoFrame );
    m_scaleLabel->setText( tr( "Scale" ) );
    m_scaleLabel->setToolTip( tr( "Current map scale" ) );
    statusBar()->addPermanentWidget( m_scaleLabel, 0 );

    m_scaleEdit = new QgsScaleComboBox( statusBar() );
    m_scaleEdit->setObjectName( "m_scaleEdit" );
    m_scaleEdit->setFont( myFont );
    m_scaleEdit->setMinimumWidth( 10 );
    m_scaleEdit->setMaximumWidth( 100 );
    m_scaleEdit->setContentsMargins( 0, 0, 0, 0 );
    m_scaleEdit->setToolTip( tr( "Current map scale (formatted as x:y)" ) );
    statusBar()->addPermanentWidget( m_scaleEdit, 0 );
    connect( m_scaleEdit,  SIGNAL( scaleChanged() ), this, SLOT( userScale() ) );

}

void qgis_dev::autoSelectAddedLayer( QList<QgsMapLayer*> layers )
{
    if ( layers.count() )
    {
        QgsLayerTreeLayer* nodeLayer = QgsProject::instance()->layerTreeRoot()->findLayer( layers[0]->id() );

        if ( !nodeLayer )
        {
            return;
        }

        QModelIndex index = m_layerTreeView->layerTreeModel()->node2index( nodeLayer );
        m_layerTreeView->setCurrentIndex( index );
    }
}

void qgis_dev::addDockWidget( Qt::DockWidgetArea area, QDockWidget* dockwidget )
{
    QMainWindow::addDockWidget( area, dockwidget );
    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

    dockwidget->show();
    m_mapCanvas->refresh();
}

void qgis_dev::openAttributeTableDialog()
{
    QgsVectorLayer* mylayer = qobject_cast<QgsVectorLayer*>( activeLayer() );
    if ( !mylayer ) { return; }
    qgis_devattrtableDialog* d = new qgis_devattrtableDialog( mylayer, this );

    d->show();


    /* QgsVectorLayerCache * lc = new QgsVectorLayerCache( mylayer, mylayer->featureCount() );
     QgsAttributeTableView* tv = new QgsAttributeTableView();

     QgsAttributeTableModel* tm = new QgsAttributeTableModel( lc, this );

     QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel( m_mapCanvas, tm, tm );

     tfm->setFilterMode( QgsAttributeTableFilterModel::ShowAll );
     tm->loadLayer();
     tv->setModel( tfm );
     tv->show();*/
}

QgsMapLayer* qgis_dev::activeLayer()
{
    return m_layerTreeView ? m_layerTreeView->currentLayer() : 0;
}

void qgis_dev::removeLayer()
{
    if ( !m_layerTreeView ) {return;}
    foreach( QgsMapLayer* layer, m_layerTreeView->selectedLayers() )
    {
        QgsVectorLayer* veclayer = qobject_cast<QgsVectorLayer*>( layer );
        if ( veclayer && veclayer->isEditable() ) {return;}
    }

    QList<QgsLayerTreeNode*> selectedNodes = m_layerTreeView->selectedNodes( true );

    // validate selection
    if ( selectedNodes.isEmpty() )
    {
        QMessageBox::critical(
            this,
            tr( "Error" ),
            tr( "No selection valid" ) );
        return;
    }

    foreach ( QgsLayerTreeNode* node, selectedNodes )
    {
        QgsLayerTreeGroup* parentGroup = qobject_cast<QgsLayerTreeGroup*>( node->parent() );
        if ( parentGroup )
        {
            parentGroup->removeChildNode( node );
        }
    }
    m_mapCanvas->refresh();
}

QIcon qgis_dev::getThemeIcon( const QString &theName )
{
    QString myPreferredPath = activeThemePath() + QDir::separator() + theName;
    QString myDefaultPath = defaultThemePath() + QDir::separator() + theName;

    if ( QFile::exists( myPreferredPath ) )
    {
        return QIcon( myPreferredPath );
    }
    else if ( QFile::exists( myDefaultPath ) )
    {
        //could still return an empty icon if it
        //doesnt exist in the default theme either!
        return QIcon( myDefaultPath );
    }
    else
    {
        return QIcon();
    }
}

const QString qgis_dev::activeThemePath()
{
    return "";
}

const QString qgis_dev::defaultThemePath()
{
    return ":/images/themes/default/";
}

void qgis_dev::changeSymbol( QgsVectorLayer* layer, QgsFeatureRendererV2* featureRenderer )
{
    if ( !layer->isValid() ) {return;}
}

void qgis_dev::layerSymbolTest()
{
    // 获取当前选中的图层
    QgsVectorLayer* veclayer = qobject_cast<QgsVectorLayer*>( this->activeLayer() );
    if( !veclayer->isValid() ) { return; }

    if ( veclayer->geometryType() == QGis::Point )
    {
        // 创建 svgMarkerSymbolLayer
        QgsSvgMarkerSymbolLayerV2* svgMarker = new QgsSvgMarkerSymbolLayerV2( "money/money_bank2.svg" );

        QgsSymbolLayerV2List symList;
        symList.append( svgMarker );

        QgsMarkerSymbolV2* markSym = new QgsMarkerSymbolV2( symList );

        QgsSingleSymbolRendererV2* symRenderer = new QgsSingleSymbolRendererV2( markSym );

        svgMarker->setSize( 10 );
        veclayer->setRendererV2( symRenderer );
    }

}

void qgis_dev::showMouseCoordinate( const QgsPoint &p )
{
    if ( m_mapCanvas->mapUnits() == QGis::Degrees ) // 坐标度分秒的显示方式
    {
        QString format = QgsProject::instance()->readEntry( "PositionPrecision", "/DegreeFormat", "D" );
        if ( format == "DM" ) // 只显示度和分
        {
            m_coordsEdit->setText( p.toDegreesMinutes( m_MousePrecisionDecimalPlaces ) );
        }
        else if ( format == "DMS" ) // 显示度分秒
        {
            m_coordsEdit->setText( p.toDegreesMinutesSeconds( m_MousePrecisionDecimalPlaces ) );
        }
        else
        {
            m_coordsEdit->setText( p.toString( m_MousePrecisionDecimalPlaces ) );
        }
    }
    else
    {
        m_coordsEdit->setText( p.toString( m_MousePrecisionDecimalPlaces ) );
    }

    if ( m_coordsEdit->width() > m_coordsEdit->minimumWidth() )
    {
        m_coordsEdit->setMinimumWidth( m_coordsEdit->width() );
    }
}

void qgis_dev::removeAllLayers()
{
    QgsMapLayerRegistry::instance()->removeAllMapLayers();
}

void qgis_dev::createOverview()
{
    QgsMapOverviewCanvas* overviewCanvas = new QgsMapOverviewCanvas( NULL, m_mapCanvas );
    overviewCanvas->setWhatsThis( tr( "Map overview canvas." ) );

    QBitmap overviewPanBmp = QBitmap::fromData( QSize( 16, 16 ), pan_bits ); // 包含在qgscursors.h文件中
    QBitmap overviewPanBmpMask = QBitmap::fromData( QSize( 16, 16 ), pan_mask_bits );
    m_overviewMapCursor = new QCursor( overviewPanBmp, overviewPanBmpMask, 0, 0 );
    overviewCanvas->setCursor( *m_overviewMapCursor );

    // 装进鹰眼图中
    m_OverviewDock = new QDockWidget( tr( "Overview" ), this );
    m_OverviewDock->setObjectName( "Overview" );
    m_OverviewDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    m_OverviewDock->setWidget( overviewCanvas );
    addDockWidget( Qt::LeftDockWidgetArea, m_OverviewDock );

    m_OverviewDock->setMinimumSize( 200, 200 );

    m_mapCanvas->enableOverviewMode( overviewCanvas );

    // 将所有图层管理器里面的图层都加入到鹰眼图中
    if ( m_layerTreeView )
    {
        foreach( QgsLayerTreeLayer* nodeLayer, m_layerTreeView->layerTreeModel()->rootGroup()->findLayers() )
        {
            nodeLayer->setCustomProperty( "overview", 1 );
        }
    }

}

void qgis_dev::createComposer()
{

}

void qgis_dev::showScale( double theScale )
{
    m_scaleEdit->setScale( 1.0 / theScale );
    if ( m_scaleEdit->width() > m_scaleEdit->minimumWidth() )
    {
        m_scaleEdit->setMinimumWidth( m_scaleEdit->width() );
    }
}

void qgis_dev::userScale()
{
    m_mapCanvas->zoomScale( 1.0 / m_scaleEdit->scale() );
}

void qgis_dev::userCenter()
{
    // 先检查是不是要打开或关闭晕眩效果
    if ( m_coordsEdit->text() == "dizzy" )
    {

        if ( m_dizzyTimer->isActive() )
        {
            m_dizzyTimer->stop();
            m_mapCanvas->setSceneRect( m_mapCanvas->viewport()->rect() );
            m_mapCanvas->setTransform( QTransform() );
        }
        else
        {
            m_dizzyTimer->start( 100 );
        }
    }
    // retro是要重新加载地图
    else if ( m_coordsEdit->text() == "retro" )
    {
        m_mapCanvas->setProperty( "retro", !m_mapCanvas->property( "retro" ).toBool() );
        refreshMapCanvas();
    }

    QStringList parts = m_coordsEdit->text().split( ',' );
    if ( parts.size() != 2 ) {return;}

    bool xOk;
    double x = parts.at( 0 ).toDouble( &xOk );
    if ( !xOk )	{return;}

    bool yOk;
    double y = parts.at( 1 ).toDouble( &yOk );
    if ( !yOk )	{return;}

    // 居中显示 x 和 y 的坐标点
    m_mapCanvas->setCenter( QgsPoint( x, y ) );
    m_mapCanvas->refresh();
}

void qgis_dev::dizzy()
{
    int d = 10;
    int r = 4;
    QRectF rect = m_mapCanvas->sceneRect();
    if ( rect.x() < -d || rect.x() > d || rect.y() < -d || rect.y() > d )
    {
        return;
    }
    rect.moveTo( ( qrand() % ( 2 * d ) ) - d, ( qrand() % ( 2 * d ) ) - d );
    m_mapCanvas->setSceneRect( rect );
    QTransform matrix;
    matrix.rotate( ( qrand() % ( 2 * r ) ) - r );
    m_mapCanvas->setTransform( matrix );
}

void qgis_dev::refreshMapCanvas()
{
    m_mapCanvas->stopRendering();
    QgsMapLayerRegistry::instance()->reloadAllLayers();
    m_mapCanvas->clearCache();
    m_mapCanvas->refresh();
}

void qgis_dev::mapUnitChange( int i )
{
    switch( i )
    {
    case 1: // DecimalDegress
        m_mapCanvas->setMapUnits( QGis::DecimalDegrees );
        break;
    case 2:
        m_mapCanvas->setMapUnits( QGis::Degrees );
        break;
    }
}

