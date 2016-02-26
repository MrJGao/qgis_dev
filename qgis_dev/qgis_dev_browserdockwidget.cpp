#include "qgis_dev_browserdockwidget.h"

#include "qgis_dev_dockbrowsertreeview.h"

#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QModelIndex>
#include <QSettings>
#include <QModelIndexList>
#include <QShowEvent>

#include "qgsbrowsermodel.h"
#include "qgsdataitem.h"
#include "qgsfieldproxymodel.h"
#include "qgsrasterlayer.h"

#include "qgis_dev.h"
#include "qgis_dev_browsertreefilterproxymodel.h"

qgis_dev_browserDockWidget::qgis_dev_browserDockWidget( QString name, QWidget *parent )
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

qgis_dev_browserDockWidget::~qgis_dev_browserDockWidget()
{

}

void qgis_dev_browserDockWidget::addFavouriteDirectory( QString favDir )
{
    m_model->addFavouriteDirectory( favDir );
}

void qgis_dev_browserDockWidget::addFavouriteDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this, tr( "Add directory to favourites" ) );
    if ( !directory.isEmpty() )
    {
        addFavouriteDirectory( directory );
    }
}

void qgis_dev_browserDockWidget::addFavourite()
{
    QModelIndex index = m_proxyModel->mapToSource( m_browserView->currentIndex() );
    QgsDataItem* item = m_model->dataItem( index );
    if ( !item ) {return;}

    QgsDirectoryItem* dirItem = dynamic_cast<QgsDirectoryItem*>( item );
    addFavouriteDirectory( dirItem->dirPath() );
}

void qgis_dev_browserDockWidget::removeFavourite()
{
    m_model->removeFavourite( m_proxyModel->mapToSource( m_browserView->currentIndex() ) );
}

void qgis_dev_browserDockWidget::addLayerAtIndex( const QModelIndex& index )
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

void qgis_dev_browserDockWidget::showContextMenu( const QPoint& pt )
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
        QAction *action = menu->addAction( tr( "Fast scan this dir" ), this, SLOT( toggleFastScan() ) );
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

void qgis_dev_browserDockWidget::refresh()
{
    refreshModel( QModelIndex() );
}

void qgis_dev_browserDockWidget::showFilterWidget( bool visible )
{
    mWidgetFilter->setVisible( visible );
    if ( !visible )
    {
        mLeFilter->setText( "" );
        setFilter();
    }
}

void qgis_dev_browserDockWidget::setFilterSyntax( QAction *action )
{
    if ( !action || !m_proxyModel ) {return;}

    m_proxyModel->setFilterSyntax( action->data().toString() );
}

void qgis_dev_browserDockWidget::setCaseSensitive( bool caseSensitive )
{
    if( !m_proxyModel ) {return;}

    m_proxyModel->setCaseSensitive( caseSensitive );
}

void qgis_dev_browserDockWidget::setFilter()
{
    QString filter = mLeFilter->text();
    if ( m_proxyModel )
    {
        m_proxyModel->setFilter( filter );
    }
}

void qgis_dev_browserDockWidget::addCurrentLayer()
{
    addLayerAtIndex( m_browserView->currentIndex() );
}

void qgis_dev_browserDockWidget::addSelectedLayers()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    QModelIndexList list = m_browserView->selectionModel()->selectedIndexes();
    qSort( list );
    for ( int i = list.size() - 1; i >= 0; i-- )
    {
        QgsDataItem* item = m_model->dataItem( m_proxyModel->mapToSource( list[i] ) );
        if ( item && item->type() == QgsDataItem::Layer )
        {
            QgsLayerItem* layerItem = qobject_cast<QgsLayerItem*>( item );
            if( layerItem )
            {
                addLayer( layerItem );
            }
        }
    }
    QApplication::restoreOverrideCursor();
}

void qgis_dev_browserDockWidget::showProperties()
{
    //QModelIndex index = m_proxyModel->mapToSource( m_browserView->currentIndex() );
    //QgsDataItem* item = m_model->dataItem( index );
    //if ( ! item ) { return; }

    //if ( item->type() == QgsDataItem::Layer )
    //{
    //    QgsLayerItem *layerItem = qobject_cast<QgsLayerItem*>( item );
    //    if ( layerItem != NULL )
    //    {
    //        QgsMapLayer::LayerType type = layerItem->mapLayerType();
    //        QString layerMetadata = tr( "Error" );
    //        QgsCoordinateReferenceSystem layerCrs;
    //        QString notice;

    //        // temporarily override /Projections/defaultBehaviour to avoid dialog prompt
    //        QSettings settings;
    //        QString defaultProjectionOption = settings.value( "/Projections/defaultBehaviour", "prompt" ).toString();
    //        if ( settings.value( "/Projections/defaultBehaviour", "prompt" ).toString() == "prompt" )
    //        {
    //            settings.setValue( "/Projections/defaultBehaviour", "useProject" );
    //        }

    //        // find root item
    //        // we need to create a temporary layer to get metadata
    //        // we could use a provider but the metadata is not as complete and "pretty"  and this is easier
    //        if ( type == QgsMapLayer::RasterLayer )
    //        {

    //            // should copy code from addLayer() to split uri ?
    //            QgsRasterLayer* layer = new QgsRasterLayer( layerItem->uri(), layerItem->uri(), layerItem->providerKey() );
    //            if ( layer != NULL )
    //            {
    //                if ( layer->isValid() )
    //                {
    //                    layerCrs = layer->crs();
    //                    layerMetadata = layer->metadata();
    //                }
    //                delete layer;
    //            }
    //        }
    //        else if ( type == QgsMapLayer::VectorLayer )
    //        {
    //            QgsVectorLayer* layer = new QgsVectorLayer( layerItem->uri(), layerItem->name(), layerItem->providerKey() );
    //            if ( layer != NULL )
    //            {
    //                if ( layer->isValid() )
    //                {
    //                    layerCrs = layer->crs();
    //                    layerMetadata = layer->metadata();
    //                }
    //                delete layer;
    //            }
    //        }

    //        // restore /Projections/defaultBehaviour
    //        if ( defaultProjectionOption == "prompt" )
    //        {
    //            settings.setValue( "/Projections/defaultBehaviour", defaultProjectionOption );
    //        }

    //        // initialize dialog
    //        QDialog *dialog = new QDialog( this );
    //        Ui::QgsBrowserLayerPropertiesBase ui;
    //        ui.setupUi( dialog );

    //        dialog->setWindowTitle( tr( "Layer Properties" ) );
    //        ui.leName->setText( layerItem->name() );
    //        ui.leSource->setText( layerItem->path() );
    //        ui.leProvider->setText( layerItem->providerKey() );
    //        QString myStyle = QgsApplication::reportStyleSheet();
    //        ui.txtbMetadata->document()->setDefaultStyleSheet( myStyle );
    //        ui.txtbMetadata->setHtml( layerMetadata );

    //        // report if layer was set to to project crs without prompt (may give a false positive)
    //        if ( defaultProjectionOption == "prompt" )
    //        {
    //            QgsCoordinateReferenceSystem defaultCrs =
    //                QgisApp::instance()->mapCanvas()->mapSettings().destinationCrs();
    //            if ( layerCrs == defaultCrs )
    //            {
    //                ui.lblNotice->setText( "NOTICE: Layer srs set from project (" + defaultCrs.authid() + ")" );
    //            }
    //        }

    //        dialog->show();
    //    }
    //}
    //else if ( item->type() == QgsDataItem::Directory )
    //{
    //    // initialize dialog
    //    QDialog *dialog = new QDialog( this );
    //    Ui::QgsBrowserDirectoryPropertiesBase ui;
    //    ui.setupUi( dialog );

    //    dialog->setWindowTitle( tr( "Directory Properties" ) );
    //    ui.leSource->setText( item->path() );
    //    QgsDirectoryParamWidget *paramWidget = new QgsDirectoryParamWidget( item->path(), dialog );
    //    ui.lytWidget->addWidget( paramWidget );

    //    dialog->show();
    //}
}

void qgis_dev_browserDockWidget::toggleFastScan()
{
    QModelIndex index = m_proxyModel->mapToSource( m_browserView->currentIndex() );
    QgsDataItem* item = m_model->dataItem( index );
    if( !item ) {return;}

    if ( item->type() == QgsDataItem::Directory )
    {
        QSettings settings;
        QStringList fastScanDirs = settings.value( "/qgis/scanItemsFastScanUris",
                                   QStringList() ).toStringList();
        int idx = fastScanDirs.indexOf( item->path() );
        if ( idx != -1 )
        {
            fastScanDirs.removeAt( idx );
        }
        else
        {
            fastScanDirs << item->path();
        }
        settings.setValue( "/qgis/scanItemsFastScanUris", fastScanDirs );
    }
}

void qgis_dev_browserDockWidget::refreshModel( const QModelIndex& index )
{
    QgsDataItem* item = m_model->dataItem( index );
    if ( item && ( item->capabilities2() & QgsDataItem::Fertile ) )
    {
        m_model->refresh( index );
    }

    for ( int i = 0; i < m_model->rowCount( index ); i++ )
    {
        QModelIndex idx = m_model->index( i, 0, index );
        QModelIndex proxyIdx = m_proxyModel->mapFromSource( idx );
        QgsDataItem* child = m_model->dataItem( idx );

        if ( m_browserView->isExpanded( proxyIdx )
                || m_browserView->hasExpandedDescendant( proxyIdx )
                || ( child && child->capabilities2() & QgsDataItem::Fast ) )
        {
            refreshModel( idx );
        }
        else
        {
            if ( child && ( child->capabilities2() & QgsDataItem::Fertile ) )
            {
                child->depopulate();
            }
        }
    }
}

void qgis_dev_browserDockWidget::showEvent( QShowEvent* e )
{
    // 延迟初始化 m_model
    if ( !m_model )
    {
        m_model = new QgsBrowserModel( m_browserView );
        connect( qgis_dev::instance(), SIGNAL( newProject() ), m_model, SLOT( updateProjectHome() ) );

        m_proxyModel = new qgis_dev_browserTreeFilterProxyModel( this );
        m_proxyModel->setBrowserModel( m_model );

        m_browserView->setSettingsSection( objectName().toLower() );
        m_browserView->setModel( m_proxyModel );

        m_browserView->setTextElideMode( Qt::ElideNone );
        m_browserView->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
        m_browserView->header()->setStretchLastSection( false );
    }
}

void qgis_dev_browserDockWidget::addLayer( QgsLayerItem* layerItem )
{
    if ( layerItem == NULL ) {return;}

    QString uri = qgis_dev::instance()->crsAndFormatAdjustedLayerUri( layerItem->uri(), layerItem->supportedCRS(), layerItem->supportedFormats() );
    if ( uri.isEmpty() ) {return;}

    QgsMapLayer::LayerType type = layerItem->mapLayerType();
    QString providerKey = layerItem->providerKey();

    if ( type == QgsMapLayer::VectorLayer )
    {
        qgis_dev::instance()->addVectorLayer( uri, layerItem->layerName(), providerKey );
    }
    if ( type == QgsMapLayer::RasterLayer )
    {
        qgis_dev::instance()->addRasterLayer( uri, layerItem->layerName(), providerKey );
    }
}
