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
#include <QDir>

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

#include "qgis_devlayertreeviewmenuprovider.h"
#include "qgis_devattrtabledialog.h"


#include <qgsfeaturelistview.h>
#include <qgsattributetableview.h>
#include <qgsattributetablemodel.h>
#include <qgsfeaturelistmodel.h>
#include <qgsvectorlayercache.h>


qgis_dev* qgis_dev::sm_instance = 0;

qgis_dev::qgis_dev( QWidget *parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags )
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

    //! 初始化map canvas, 并装入layout中
    m_mapCanvas = new QgsMapCanvas();
    m_mapCanvas->enableAntiAliasing( true );
    m_mapCanvas->setCanvasColor( QColor( 255, 255, 255 ) );

    //! 初始化图层管理器
    m_layerTreeView = new QgsLayerTreeView( this );
    initLayerTreeView();

    //! 布局
    QWidget* centralWidget = this->centralWidget();
    QGridLayout* centralLayout = new QGridLayout( centralWidget );
    centralLayout->addWidget( m_mapCanvas, 0, 0, 1, 1 );

    // connections
    connect( ui.actionAdd_Vector, SIGNAL( triggered() ), this, SLOT( addVectorLayers() ) );
    connect( ui.actionAdd_Raster, SIGNAL( triggered() ), this, SLOT( addRasterLayers() ) );
}

qgis_dev::~qgis_dev()
{
    delete sm_instance;
}

void qgis_dev::addVectorLayers()
{
    /* QString filename = QFileDialog::getOpenFileName( this, tr( "open vector" ), "", "*.shp" );
     if ( filename == QString::null ) { return;}*/

    // test attribute table
    QString filename = "D:\\Data\\qgis_sample_data\\shapefiles\\airports.shp" ;

    QStringList temp = filename.split( QDir::separator() );
    QString basename = temp.at( temp.size() - 1 );
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
    QStringList temp = filename.split( QDir::separator() );
    QString basename = temp.at( temp.size() - 1 );
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
    // btnRemoveItem->setDefaultAction( this->m_actionRemoveLayer );
    btnRemoveItem->setAutoRaise( true );


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
    connect( m_coordsEdit, SIGNAL( returnPressed() ), this, SLOT( userCenter() ) );

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
    //qgis_devattrtableDialog* d = new qgis_devattrtableDialog( mylayer, this );
    //d->show();


    QgsVectorLayerCache* lc = new QgsVectorLayerCache( mylayer, mylayer->featureCount() );
    QgsAttributeTableView* tv = new QgsAttributeTableView();

    QgsAttributeTableModel* tm = new QgsAttributeTableModel( lc, this );

    QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel( QgisApp::instance(), tm, tm );

    tfm->setFilterMode( QgsAttributeTableFilterModel::ShowAll );
    tm->loadLayer();
    tv->setModel( tfm );
    tv->show();

}

QgsMapLayer* qgis_dev::activeLayer()
{
    return m_layerTreeView ? m_layerTreeView->currentLayer() : 0;
}

void qgis_dev::removeLayer()
{
    m_layerTreeView->defaultActions()->actionRemoveGroupOrLayer();
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

