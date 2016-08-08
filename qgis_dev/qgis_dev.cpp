#include "qgis_dev.h"

#include "qgis_devlayertreeviewmenuprovider.h"
#include "qgis_devattrtabledialog.h"
#include "qgis_devmaptoolidentifyaction.h"
#include "qgis_dev_layerPropDialog.h"

#include <QSizeF>
#include <QDialog>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
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
#include <QSizePolicy>
#include <QBitmap>
#include <QMap>
#include <QRegExp>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

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
#include <qgsproviderregistry.h>
#include <qgsnewvectorlayerdialog.h>
#include <qgsmessagebaritem.h>
#include <qgsnewmemorylayerdialog.h>

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
#include <qgscategorizedsymbolrendererv2.h>

// for map tools
#include <qgsmaptool.h>
#include <qgsmaptoolzoom.h>
#include <qgsmaptoolidentify.h>
#include <qgsmaptoolpan.h>

// for open source map
#include <qgsowssourceselect.h>
#include "qgis_dev_owssourceselector.h"
#include "qgis_dev_addfeaturetool.h"
#include "qgsmessagebar.h"

// for diagrams
#include <qgshistogramdiagram.h>
#include <qgsdiagramrendererv2.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorsimplifymethod.h>

#include <qgsoptionsdialogbase.h>

#include <qgslabel.h>

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
    }
    sm_instance = this;

    ui.setupUi( this );

    //! 初始化map canvas
    m_mapCanvas = new QgsMapCanvas();
    // 给一个默认的地图范围
    QgsRectangle* defaultExtent = new QgsRectangle( -1, -1, 1, 1 );
    m_mapCanvas->setExtent( *defaultExtent );

    // 设置鼠标滚轮缩放，不改变原有地图中心
    m_mapCanvas->setWheelAction( QgsMapCanvas::WheelZoomToMouseCursor );
    m_mapCanvas->enableAntiAliasing( true );
    m_mapCanvas->setCanvasColor( QColor( 255, 255, 255 ) );

    //! 创建地图工具
    createMapTools();

    //! 构建打印出图视图
    m_composer = new qgis_devComposer( this );
    createComposer();

    //! 初始化图层管理器
    m_layerTreeView = new QgsLayerTreeView( this );
    initLayerTreeView();

    //! 初始化文件浏览窗口
    m_browserDockWight = new qgis_dev_browserDockWidget( tr( "Browser" ), this );
    addDockWidget( Qt::LeftDockWidgetArea, m_browserDockWight );

    //! 初始化信息显示条
    m_infoBar = new QgsMessageBar( this );
    m_infoBar->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );

    //! 初始化图层属性窗口
    m_layerPropDialog = 0;

    //! 布局
    QWidget* centralWidget = this->centralWidget();
    QGridLayout* centralLayout = new QGridLayout( centralWidget );

    m_stackedWidget = new QStackedWidget( this );
    m_stackedWidget->setLayout( new QHBoxLayout() );
    m_stackedWidget->addWidget( m_mapCanvas );
    m_stackedWidget->addWidget( m_composer );

    centralLayout->addWidget( m_stackedWidget, 0, 0, 1, 1 );
    centralLayout->addWidget( m_infoBar, 1, 0, 1, 1 );

    //! 初始化status bar
    createStatusBar();

    // connections
    connect( m_mapCanvas, SIGNAL( xyCoordinates( const QgsPoint& ) ), this, SLOT( showMouseCoordinate( const QgsPoint& ) ) );
    connect( ui.actionAdd_Vector, SIGNAL( triggered() ), this, SLOT( addVectorLayers() ) );
    connect( ui.actionAddWMSlayer, SIGNAL( triggered() ), this, SLOT( addWMSLayers() ) );
    connect( ui.actionAdd_WCF_Layer, SIGNAL( triggered() ), this, SLOT( addWCSLayers() ) );
    connect( ui.actionAdd_WFS_Layer, SIGNAL( triggered() ), this, SLOT( addWFSLayers() ) );
    connect( ui.actionAdd_Raster, SIGNAL( triggered() ), this, SLOT( addRasterLayers() ) );
    connect( ui.actionNew_Vector, SIGNAL( triggered() ), this, SLOT( newVectorLayer() ) );
    connect( ui.actionNew_Memory_Vector_Layer, SIGNAL( triggered() ), this, SLOT( newMemoryLayer() ) );

    connect( ui.actionToggle_Overview, SIGNAL( triggered() ), this, SLOT( createOverview() ) );
    connect( m_mapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );

    //! 栅格显示的几个工具
    connect( ui.actionLocalHistogramStretch, SIGNAL( triggered() ), this, SLOT( localHistogramStretch() ) );
    connect( ui.actionFullHistogramStretch, SIGNAL( triggered() ), this, SLOT( fullHistogramStretch() ) );
    connect( ui.actionLocalCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( localCumulativeCutStretch() ) );
    connect( ui.actionFullCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( fullCumulativeCutStretch() ) );
    connect( ui.actionIncreaseBrightness, SIGNAL( triggered() ), this, SLOT( increaseBrightness() ) );
    connect( ui.actionDecreaseBrightness, SIGNAL( triggered() ), this, SLOT( decreaseBrightness() ) );
    connect( ui.actionIncreaseContrast, SIGNAL( triggered() ), this, SLOT( increaseContrast() ) );
    connect( ui.actionDecreaseContrast, SIGNAL( triggered() ), this, SLOT( decreaseContrast() ) );
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

    addVectorLayer( filename, basename, "ogr" );
}

void qgis_dev::addVectorLayer( QString vecLayerPath, QString basename, QString providerKey )
{
    QgsVectorLayer* vecLayer = new QgsVectorLayer( vecLayerPath, basename, providerKey, false );
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
    m_mapCanvas->setCurrentLayer( vecLayer );
}

void qgis_dev::addRasterLayers()
{
    QString filename = QFileDialog::getOpenFileName( this, tr( "open vector" ), "",
                       "image(*.jpg *.png *.bmp);;remote sensing image(*.tif)" );
    if ( filename == "" ) { return; }
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
    connect( m_layerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer* ) ), this, SLOT( activeLayerChanged( QgsMapLayer* ) ) );


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

    /*QgsVectorLayerCache * lc = new QgsVectorLayerCache( mylayer, mylayer->featureCount() );
    QgsAttributeTableView* tv = new QgsAttributeTableView();

    QgsAttributeTableModel* tm = new QgsAttributeTableModel( lc, this );

    QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel( m_mapCanvas, tm, tm );

    tfm->setFilterMode( QgsAttributeTableFilterModel::ShowAll );
    tm->loadLayer();
    tv->setModel( tfm );
    tv->show();*/

    qgis_devattrtableDialog* d = new qgis_devattrtableDialog( mylayer, this );
    d->show();
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

void qgis_dev::localHistogramStretch()
{
    histogramStretch( true, QgsRaster::ContrastEnhancementMinMax );
}

void qgis_dev::fullHistogramStretch()
{
    histogramStretch( false, QgsRaster::ContrastEnhancementMinMax );
}

void qgis_dev::localCumulativeCutStretch()
{
    histogramStretch( true, QgsRaster::ContrastEnhancementCumulativeCut );
}

void qgis_dev::fullCumulativeCutStretch()
{
    histogramStretch( false, QgsRaster::ContrastEnhancementCumulativeCut );
}

void qgis_dev::increaseBrightness()
{
    int step = 1;
    if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
    {
        step = 10;
    }
    adjustBrightnessContrast( step );
}

void qgis_dev::decreaseBrightness()
{
    int step = -1;
    if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
    {
        step = -10;
    }
    adjustBrightnessContrast( step );
}

void qgis_dev::increaseContrast()
{
    int step = 1;
    if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
    {
        step = 10;
    }
    adjustBrightnessContrast( step, false );
}

void qgis_dev::decreaseContrast()
{
    int step = -1;
    if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
    {
        step = -10;
    }
    adjustBrightnessContrast( step, false );
}

void qgis_dev::histogramStretch( bool visibleAreaOnly /*= false*/,
                                 QgsRaster::ContrastEnhancementLimits theLimits /*= QgsRaster::ContrastEnhancementMinMax */ )
{
    QgsMapLayer* myLayer = m_layerTreeView->currentLayer();
    if ( !myLayer ) // 判断是否为地图图层
    {
        messageBar()->pushMessage( tr( "No Layer Selected" ),
                                   tr( "To perform a full histogram stretch, you need to have a raster layer selected." ),
                                   QgsMessageBar::INFO, messageTimeout() );
        return;
    }

    QgsRasterLayer* myRasterLayer = qobject_cast<QgsRasterLayer*>( myLayer );
    if ( !myRasterLayer ) // 判断是否为栅格图层
    {
        messageBar()->pushMessage( tr( "No Layer Selected" ),
                                   tr( "To perform a full histogram stretch, you need to have a raster layer selected." ),
                                   QgsMessageBar::INFO, messageTimeout() );
        return;
    }

    QgsRectangle myRectangle;
    if ( visibleAreaOnly )
    {
        myRectangle = m_mapCanvas->mapSettings().outputExtentToLayerExtent( myRasterLayer, m_mapCanvas->extent() );
    }
    // 这一句是关键
    myRasterLayer->setContrastEnhancement( QgsContrastEnhancement::StretchToMinimumMaximum, theLimits, myRectangle );
    m_mapCanvas->refresh();
}

int qgis_dev::messageTimeout()
{
    return 5; /*这个参数应该让用户自定义*/
}

void qgis_dev::adjustBrightnessContrast( int delta, bool updateBrightness /*= true */ )
{
    foreach( QgsMapLayer* layer, m_layerTreeView->selectedLayers() ) // 遍历所有选择的图层
    {
        if ( !layer )
        {
            messageBar()->pushMessage( tr( "No Layer Selected" ),
                                       tr( "To change brightness or contrast, you need to have a raster layer selected." ),
                                       QgsMessageBar::INFO, messageTimeout() );
            return;
        }

        QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer *>( layer );
        if ( !rasterLayer )
        {
            messageBar()->pushMessage( tr( "No Layer Selected" ),
                                       tr( "To change brightness or contrast, you need to have a raster layer selected." ),
                                       QgsMessageBar::INFO, messageTimeout() );
            return;
        }

        // 这里是关键，用QgsBrightnessFilter类控制亮度与对比度
        QgsBrightnessContrastFilter* brightnessFilter = rasterLayer->brightnessFilter();
        if ( updateBrightness )
        {
            brightnessFilter->setBrightness( brightnessFilter->brightness() + delta );
        }
        else
        {
            brightnessFilter->setContrast( brightnessFilter->contrast() + delta );
        }

        rasterLayer->triggerRepaint(); // 重画栅格图层

    }
}

void qgis_dev::on_actionZoomIn_triggered()
{
    m_mapCanvas->setMapTool( m_mapToolZoomIn );
}

void qgis_dev::on_actionZoomOut_triggered()
{
    m_mapCanvas->setMapTool( m_mapToolZoomOut );
}

void qgis_dev::on_actionPan_triggered()
{
    m_mapCanvas->setMapTool( m_mapToolPan );
}

void qgis_dev::createMapTools()
{
    m_mapToolPan = new QgsMapToolPan( m_mapCanvas );
    m_mapToolZoomIn = new QgsMapToolZoom( m_mapCanvas, false );
    m_mapToolZoomOut = new QgsMapToolZoom( m_mapCanvas, true );
    m_mapToolIdentify = new qgis_devMapToolIdentifyAction( m_mapCanvas );
}

void qgis_dev::on_actionIdentify_triggered()
{
    m_mapCanvas->setMapTool( m_mapToolIdentify );
    m_mapToolIdentify->activate();
}

void qgis_dev::addWMSLayers()
{
    QDialog *wms = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "wms" ), this ) );
    if ( !wms )
    {
        statusBar()->showMessage( tr( "cannot add wms layer." ), 10 );
    }

    connect( wms, SIGNAL( addRasterLayer( QString const &, QString const &, QString const & ) ),
             this, SLOT( addOpenSourceRasterLayer( QString const &, QString const &, QString const & ) ) );

    wms->exec();

    delete wms;

    // 这个图层显示有畸变
    /*addOpenSourceRasterLayer( "contextualWMSLegend=0&crs=EPSG:4326&dpiMode=7&featureCount=10&format=image/jpeg&layers=cartographic:national-boundaries&styles=&url=http://sedac.ciesin.columbia.edu:80/geoserver/wms",
                              "cartographic:national-boundaries", "wms" );*/

    // 这句说明添加图层是没有问题的
    //addOpenSourceRasterLayer( "contextualWMSLegend=0&crs=EPSG:4326&dpiMode=all&featureCount=10&format=image/gif&layers=DC&styles=&url=http://wms.lizardtech.com/lizardtech/iserv/ows",
    //                          "DC",
    //                          "wms" );

    /* qgis_dev_OWSSourceSelector* ows = new qgis_dev_OWSSourceSelector(  "wms", this );

     connect( ows, SIGNAL( addRasterLayer( const QString&, const QString&, const QString& ) ),
              this, SLOT( addWMSLayer( const QString&, const QString&, const QString& ) ) );
     ows->exec();*/

}

void qgis_dev::addOpenSourceRasterLayer( const QString& url, const QString& basename, const QString& providerKey )
{
    QgsRasterLayer *rasterLayer = 0;

    if ( providerKey.isEmpty() )
    {
        rasterLayer = new QgsRasterLayer( url, basename );
    }
    else
    {
        rasterLayer = new QgsRasterLayer( url, basename, providerKey );
    }

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

void qgis_dev::addWCSLayers()
{
    QDialog *wcs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "wcs" ), this ) );
    if ( !wcs )
    {
        statusBar()->showMessage( tr( "cannot add wcs layer." ), 10 );
    }

    connect( wcs, SIGNAL( addRasterLayer( QString const &, QString const &, QString const & ) ),
             this, SLOT( addOpenSourceRasterLayer( QString const &, QString const &, QString const & ) ) );

    wcs->exec();
    delete wcs;
}

void qgis_dev::addWFSLayers()
{
    if ( !m_mapCanvas ) {return;}

    QDialog *wfs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "WFS" ), this ) );
    if ( !wfs )
    {
        QMessageBox::warning( this, tr( "WFS" ), tr( "Cannot get WFS select dialog from provider." ) );
        return;
    }
    connect( wfs, SIGNAL( addWfsLayer( QString, QString ) ),
             this, SLOT( addWfsLayer( const QString, const QString ) ) );

    //re-enable wfs with extent setting: pass canvas info to source select
    wfs->setProperty( "MapExtent", m_mapCanvas->extent().toString() );
    if ( m_mapCanvas->mapSettings().hasCrsTransformEnabled() )
    {
        //if "on the fly" reprojection is active, pass canvas CRS
        wfs->setProperty( "MapCRS", m_mapCanvas->mapSettings().destinationCrs().authid() );
    }

    bool bkRenderFlag = m_mapCanvas->renderFlag();
    m_mapCanvas->setRenderFlag( false );
    wfs->exec();
    m_mapCanvas->setRenderFlag( bkRenderFlag );
    delete wfs;
}

void qgis_dev::addWFSLayer( const QString& url, const QString& typeName )
{
    QgsVectorLayer* vecLayer = new QgsVectorLayer( url, typeName, "WFS", false );
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

QString qgis_dev::crsAndFormatAdjustedLayerUri( const QString& uri,
        const QStringList& supportedCrs, const QStringList& supportedFormats )
{
    QString newuri = uri;

    // Adjust layer CRS to project CRS
    QgsCoordinateReferenceSystem testCrs;
    foreach( QString c, supportedCrs )
    {
        testCrs.createFromOgcWmsCrs( c );
        if ( testCrs == m_mapCanvas->mapSettings().destinationCrs() )
        {
            newuri.replace( QRegExp( "crs=[^&]+" ), "crs=" + c );
            break;
        }
    }

    // Use the last used image format
    QString lastImageEncoding = QSettings().value( "/qgis/lastWmsImageEncoding", "image/png" ).toString();
    foreach ( QString fmt, supportedFormats )
    {
        if ( fmt == lastImageEncoding )
        {
            newuri.replace( QRegExp( "format=[^&]+" ), "format=" + fmt );
            break;
        }
    }
    return newuri;
}

void qgis_dev::addRasterLayer( QString rasterLayerPath, QString basename, QString providerKey )
{
    QgsRasterLayer* rasterLayer = new QgsRasterLayer( rasterLayerPath, basename, providerKey, false );
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

void qgis_dev::newVectorLayer()
{
    QString enc;
    QString fileName = QgsNewVectorLayerDialog::runAndCreateLayer( this, &enc );

    if ( !fileName.isEmpty() )
    {
        //then add the layer to the view
        QStringList fileNames;
        fileNames.append( fileName );
        //todo: the last parameter will change accordingly to layer type
        //addVectorLayers( fileNames, enc, "file" );
    }
    else if ( fileName.isNull() )
    {
        QLabel *msgLabel = new QLabel( tr( "Layer creation failed. Please check the <a href=\"#messageLog\">message log</a> for further information." ), messageBar() );
        msgLabel->setWordWrap( true );
        //connect( msgLabel, SIGNAL( linkActivated( QString ) ), mLogDock, SLOT( show() ) );
        QgsMessageBarItem *item = new QgsMessageBarItem( msgLabel, QgsMessageBar::WARNING );
        messageBar()->pushItem( item );
    }
}

void qgis_dev::newMemoryLayer()
{
    QgsVectorLayer* newLayer = QgsNewMemoryLayerDialog::runAndCreateLayer( this );
    if ( newLayer )
    {
        //then add the layer to the view
        QList< QgsMapLayer* > layers;
        layers << newLayer;

        QgsMapLayerRegistry::instance()->addMapLayers( layers );
        newLayer->startEditing();
    }
}

void qgis_dev::showProperty()
{
    QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( activeLayer() );
    if ( !layer ) { return; }
    if( m_layerPropDialog == 0 )
    {
        m_layerPropDialog = new qgis_dev_layerPropDialog( this, layer, m_mapCanvas );
    }
    m_layerPropDialog->exec();
}

void qgis_dev::activeLayerChanged( QgsMapLayer* layer )
{
    if ( !layer ) { return; }
    if ( m_mapCanvas ) {m_mapCanvas->setCurrentLayer( layer );}

    if ( layer->type() == QgsMapLayer::VectorLayer )
    {
        enableRasterTools( false );
        enableVectorTools( true );
    }
    else if ( layer->type() == QgsMapLayer::RasterLayer )
    {
        enableVectorTools( false );
        enableRasterTools( true );
    }
}

void qgis_dev::showMessage( QString message, QgsMessageBar::MessageLevel level )
{
    this->messageBar()->pushMessage( message, level, messageTimeout() );
}

void qgis_dev::on_actionAdd_Feature_triggered()
{
    QgsMapTool* addFeatureTool = new qgis_dev_addFeatureTool( m_mapCanvas );
    m_mapCanvas->setMapTool( addFeatureTool );
    connect( addFeatureTool, SIGNAL( messageEmitted( QString, QgsMessageBar::MessageLevel ) ), this, SLOT( showMessage( QString, QgsMessageBar::MessageLevel ) ) );
}

void qgis_dev::on_actionToggle_Editing_triggered()
{
    if ( ui.actionToggle_Editing->isChecked() == true )
    {
        // start editing
    }
    else
    {
        // stop editing
    }

}

void qgis_dev::on_actionSaveLayer_Edits_triggered()
{

}

void qgis_dev::on_actionMove_Feature_triggered()
{

}

void qgis_dev::on_actionNode_Tool_triggered()
{

}

void qgis_dev::on_actionDelete_Selected_triggered()
{

}

void qgis_dev::on_actionCut_Features_triggered()
{

}

void qgis_dev::on_actionCopy_Features_triggered()
{

}

void qgis_dev::on_actionPaste_Features_triggered()
{
    //testCatergorySymbol();
    //QgsOptionsDialogBase* d = new QgsOptionsDialogBase( "ProjectProperties" );
    //d->show();

    // label function
    //testVecLayerLabel();

    testUseLabelConfigDialog();
}

void qgis_dev::testCatergorySymbol()
{
    QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( activeLayer() );
    if ( !layer->isValid() ) { return; }

    // 创建 svgMarkerSymbolLayer
    QgsMarkerSymbolLayerV2* symMarker1 = new QgsSvgMarkerSymbolLayerV2( "money/money_bank2.svg" );
    QgsSvgMarkerSymbolLayerV2* symMarker2 = new QgsSvgMarkerSymbolLayerV2( "money/money_atm.svg" );

    QgsSymbolLayerV2List symList1;
    symList1.append( symMarker1 );

    QgsSymbolLayerV2List symList2;
    symList2.append( symMarker2 );

    QgsMarkerSymbolV2* sym1 = new QgsMarkerSymbolV2( symList1 );
    QgsMarkerSymbolV2* sym2 = new QgsMarkerSymbolV2( symList2 );
    // 第一个图标
    int value1 = 1;
    QString symbolName1 = "symbol1";
    bool render1 = true;
    QgsRendererCategoryV2* renCate1 = new QgsRendererCategoryV2( value1, sym1, symbolName1, render1 );

    int value2 = 2;
    QString symbolName2 = "symbol2";
    QString label2 = "label2";
    bool render2 = true;
    QgsRendererCategoryV2* renCate2 = new QgsRendererCategoryV2( value2, sym2, symbolName2, render2 );

    QgsCategoryList cateList;
    cateList.append( *renCate1 );
    cateList.append( *renCate2 );

    QgsCategorizedSymbolRendererV2* cateSymRenderer = new QgsCategorizedSymbolRendererV2( "ID", cateList );

    layer->setRendererV2( cateSymRenderer );

    refreshMapCanvas();
}

void qgis_dev::on_actionActionNullMapTool_triggered()
{
    QgsMapTool *lastMapTool = m_mapCanvas->mapTool();
    m_mapCanvas->unsetMapTool( lastMapTool );
}

void qgis_dev::enableRasterTools( bool val )
{
    ui.actionLocalHistogramStretch->setEnabled( val );
    ui.actionFullHistogramStretch->setEnabled( val );
    ui.actionLocalCumulativeCutStretch->setEnabled( val );
    ui.actionFullCumulativeCutStretch->setEnabled( val );
    ui.actionIncreaseBrightness->setEnabled( val );
    ui.actionDecreaseBrightness->setEnabled( val );
    ui.actionIncreaseContrast->setEnabled( val );
    ui.actionDecreaseContrast->setEnabled( val );
}

void qgis_dev::enableVectorTools( bool val )
{

}

void qgis_dev::markDirty()
{
    QgsProject::instance()->dirty( true );
}

void qgis_dev::testVecLayerLabel()
{
    QgsVectorLayer* layer = ( QgsVectorLayer* )this->activeLayer();
    if ( layer == NULL || layer->isValid() == false ) { return; }

    // 首先是定义一个 QgsPalLayerSettings 变量，并启用他的属性设置
    QgsPalLayerSettings layerSettings;
    layerSettings.enabled = true;

    // 然后就可以开始根据API文档中的属性，进行自定义配置了
    layerSettings.fieldName = layer->pendingFields()[3].name(); // 设置Label图层
    layerSettings.centroidWhole = true; // 设置位置参考的中心点

    // Label 字体设置
    layerSettings.textColor = QColor( 0, 0, 0 ); // 设置字体颜色
    layerSettings.textFont = QFont( "Times", 12 ); // 设置字体和大小

    // Label 轮廓buffer设置
    layerSettings.bufferDraw = true;
    layerSettings.bufferColor = QColor( 255, 0, 0 ); // 轮廓buffer的颜色
    layerSettings.bufferSize = 1; // 轮廓buffer大小
    layerSettings.bufferTransp = 0.5; // 轮廓buffer的透明度

    // Label 阴影绘制
    layerSettings.shadowDraw = true;
    layerSettings.shadowOffsetAngle = 135; // 阴影的角度
    layerSettings.shadowOffsetDist = 1; // 阴影与Label的距离

    layerSettings.fieldName = layer->pendingFields()[3].name(); // 设置Label图层
    layerSettings.setDataDefinedProperty( layerSettings.Size, true, false, NULL, "size" );
    layerSettings.setDataDefinedProperty( layerSettings.Color, true, false, NULL, "color" );
    layerSettings.setDataDefinedProperty( layerSettings.Family, true, false, NULL, "font" );

    layerSettings.writeToLayer( layer ); // 将配置写入图层
    m_mapCanvas->refresh();
}

void qgis_dev::testUseLabelConfigDialog()
{
    QgsVectorLayer* layer = ( QgsVectorLayer* )this->activeLayer();
    if ( layer == NULL || layer->isValid() == false ) { return; }

    QDialog *dialog = new QDialog( this );
    QDialogButtonBox* btnBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    QVBoxLayout* hlayout = new QVBoxLayout( dialog );

    qgis_dev_labelingGui * d = new qgis_dev_labelingGui( layer, m_mapCanvas, dialog );
    d->init();

    hlayout->addWidget( d );
    hlayout->addWidget( btnBox );
    connect( btnBox, SIGNAL( accepted() ), d, SLOT( apply() ) );
    connect( btnBox, SIGNAL( rejected() ), dialog, SLOT( close() ) );
    dialog->show();
}







































