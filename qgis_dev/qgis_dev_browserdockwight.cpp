#include "qgis_dev_browserdockwight.h"

#include "qgis_dev_dockbrowsertreeview.h"

#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QModelIndex>
#include <QSettings>

#include "qgsbrowsermodel.h"
#include "qgsdataitem.h"
#include "qgsfieldproxymodel.h"

#include "qgis_dev.h"
#include "qgis_dev_browsertreefilterproxymodel.h"

qgis_dev_browserDockWight::qgis_dev_browserDockWight( QString name, QWidget *parent )
    : QDockWidget( parent ),
      m_model( NULL ),
      m_proxyModel( NULL )
{
    setupUi( this );

    setWindowTitle( name );

    m_browserView = new qgis_dev_dockBrowserTreeView( this );
    mLayoutBrowser->addWidget( m_browserView );

    // 以下几个是 QgsBrowserDockWidgetBase 中的成员
    mBtnRefresh->setIcon( QgsApplication::getThemeIcon( "mActionDraw.svg" ) );
    mBtnAddLayers->setIcon( QgsApplication::getThemeIcon( "mActionAdd.svg" ) );
    mBtnCollapse->setIcon( QgsApplication::getThemeIcon( "mActionCollapseTree.png" ) );

    mWidgetFilter->hide();
    mLeFilter->setPlaceholderText( tr( "Type here to filter current item..." ) );

    QMenu* menu = new QMenu( this );
    menu->setSeparatorsCollapsible( false );
    mBtnFilterOptions->setMenu( menu );

    QAction* action = new QAction( tr( "Case Sensitive" ), menu );
    action->setData( "case" );
    action->setCheckable( true );
    action->setChecked( false );
    connect( action, SIGNAL( toggled( bool ) ), this, SLOT( setCaseSensitive( bool ) ) );
    menu->addAction( action );
    QActionGroup* group = new QActionGroup( menu );
    action = new QAction( tr( "Filter Pattern Syntax" ), group );
    action->setSeparator( true );
    menu->addAction( action );
    action = new QAction( tr( "Normal" ), group );
    action->setData( "normal" );
    action->setCheckable( true );
    action->setChecked( true );
    menu->addAction( action );
    action = new QAction( tr( "Wildcard(s)" ), group );
    action->setData( "wildcard" );
    action->setCheckable( true );
    menu->addAction( action );
    action = new QAction( tr( "Regular Expression" ), group );
    action->setData( "regexp" );
    action->setCheckable( true );
    menu->addAction( action );

    connect( mBtnRefresh, SIGNAL( clicked() ), this, SLOT( refresh() ) );
    connect( mBtnAddLayers, SIGNAL( clicked() ), this, SLOT( addSelectedLayers() ) );
    connect( mBtnCollapse, SIGNAL( clicked() ), m_browserView, SLOT( collapseAll() ) );
    connect( mBtnFilterShow, SIGNAL( toggled( bool ) ), this, SLOT( showFilterWidget( bool ) ) );
    connect( mLeFilter, SIGNAL( returnPressed() ), this, SLOT( setFilter() ) );
    connect( mLeFilter, SIGNAL( cleared() ), this, SLOT( setFilter() ) );
    connect( mLeFilter, SIGNAL( textChanged( const QString & ) ), this, SLOT( setFilter() ) );
    connect( group, SIGNAL( triggered( QAction * ) ), this, SLOT( setFilterSyntax( QAction * ) ) );

    connect( m_browserView, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( showContextMenu( const QPoint & ) ) );
    connect( m_browserView, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( addLayerAtIndex( const QModelIndex& ) ) );
}

qgis_dev_browserDockWight::~qgis_dev_browserDockWight()
{

}

void qgis_dev_browserDockWight::addFavouriteDirectory( QString favDir )
{
    m_model->addFavouriteDirectory( favDir );
}

void qgis_dev_browserDockWight::addFavouriteDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this, tr( "Add directory to favourites" ) );
    if ( !directory.isEmpty() )
    {
        addFavouriteDirectory( directory );
    }
}

void qgis_dev_browserDockWight::addFavourite()
{
    QModelIndex index = m_proxyModel->mapToSource( m_browserView->currentIndex() );
    QgsDataItem* item = m_model->dataItem( index );
    if ( !item ) {return;}

    QgsDirectoryItem* dirItem = dynamic_cast<QgsDirectoryItem*>( item );
    addFavouriteDirectory( dirItem->dirPath() );
}

void qgis_dev_browserDockWight::removeFavourite()
{
    m_model->removeFavourite( m_proxyModel->mapToSource( m_browserView->currentIndex() ) );
}

void qgis_dev_browserDockWight::addLayerAtIndex( const QModelIndex& index )
{
    QgsDataItem* item = m_model->dataItem( m_proxyModel->mapToSource( index ) );
    if ( item != NULL && item->type() == QgsDataItem::Layer )
    {
        QgsLayerItem* layerItem = qobject_cast<QgsLayerItem*>( item );
        if ( layerItem != NULL )
        {
            QApplication::setOverrideCursor( Qt::WaitCursor );
            addLayer( layerItem );
            QApplication::restoreOverrideCursor();
        }
    }
}

void qgis_dev_browserDockWight::showContextMenu( const QPoint& pt )
{
    QModelIndex index = m_proxyModel->mapToSource( m_browserView->indexAt( pt ) );
    QgsDataItem* item = m_model->dataItem( index );
    if( !item ) {return;}

    QMenu* menu = new QMenu( this );
    if ( item->type() == QgsDataItem::Directory ) // 如果选中的对象是目录
    {
        QSettings settings;
        QStringList favDirs = settings.value( "/browser/favourites" ).toStringList();
        bool inFavDirs = item->parent() && item->parent()->type() == QgsDataItem::Favourites;

        if ( item->parent() && !inFavDirs )
        {
            // only non-root directories can be added as favourites
            menu->addAction( tr( "Add as a favourite" ), this, SLOT( addFavourite() ) );
        }
        else if ( inFavDirs )
        {
            // only favourites can be removed
            menu->addAction( tr( "Remove favourite" ), this, SLOT( removeFavourite() ) );
        }
        menu->addAction( tr( "Properties" ), this, SLOT( showProperties() ) );
        QAction *action = menu->addAction( tr( "Fast scan this dir." ), this, SLOT( toggleFastScan() ) );
        action->setCheckable( true );
        action->setChecked( settings.value( "/qgis/scanItemsFastScanUris",
                                            QStringList() ).toStringList().contains( item->path() ) );
    }
    else if( item->type() == QgsDataItem::Layer ) // 如果选中的对象是图层
    {
        menu->addAction( tr( "Add Layer" ), this, SLOT( addCurrentLayer() ) );
        menu->addAction( tr( "Add Selected Layers" ), this, SLOT( addSelectedLayers() ) );
        menu->addAction( tr( "Properties" ), this, SLOT( showProperties() ) );
    }
    else if ( item->type() == QgsDataItem::Favourites ) // 如果选中的对象是在收藏里面
    {
        menu->addAction( tr( "Add a directory" ), this, SLOT( addFavouriteDirectory() ) );
    }

    QList<QAction*> actions = item->actions();
    if ( !actions.isEmpty() )
    {
        if ( !menu->actions().isEmpty() )
        {
            menu->addSeparator();
        }
        // add action to the menu
        menu->addActions( actions );
    }

    if ( menu->actions().count() == 0 )
    {
        delete menu;
        return;
    }

    menu->popup( m_browserView->mapToGlobal( pt ) );
}



void qgis_dev_browserDockWight::refresh()
{

}

void qgis_dev_browserDockWight::showFilterWidget( bool visible )
{

}

void qgis_dev_browserDockWight::setFilterSyntax( QAction * )
{

}

void qgis_dev_browserDockWight::setCaseSensitive( bool caseSensitive )
{

}

void qgis_dev_browserDockWight::setFilter()
{

}

void qgis_dev_browserDockWight::addCurrentLayer()
{

}

void qgis_dev_browserDockWight::addSelectedLayers()
{

}

void qgis_dev_browserDockWight::showProperties()
{

}

void qgis_dev_browserDockWight::toggleFastScan()
{

}

void qgis_dev_browserDockWight::refreshModel( const QModelIndex& index )
{

}

void qgis_dev_browserDockWight::showEvent( QShowEvent* event )
{

}

void qgis_dev_browserDockWight::addLayer( QgsLayerItem* layerItem )
{
    if ( layerItem == NULL ) {return;}

    QString uri = qgis_dev::instance()->crsAndFormatAdjustedLayerUri( layerItem->uri(), layerItem->supportedCRS(), layerItem->supportedFormats() );
    if ( uri.isEmpty() ) {return;}

    QgsMapLayer::LayerType type = layerItem->mapLayerType();
    QString providerKey = layerItem->providerKey();

    if ( type == QgsMapLayer::VectorLayer )
    {
        //qgis_dev::instance()->addVectorLayer( uri, layerItem->layerName(), providerKey );
    }
    if ( type == QgsMapLayer::RasterLayer )
    {
        //qgis_dev::instance()->addRasterLayer( uri, layerItem->layerName(), providerKey );
    }
}
