#include "qgis_devlayertreeviewmenuprovider.h"
#include "qgis_dev.h"

#include <QMenu>
#include <QModelIndex>
#include <QIcon>

// QGis include
#include <qgslayertreeviewdefaultactions.h>
#include <qgslayertreenode.h>
#include <qgslayertreemodel.h>
#include <qgslayertree.h>
#include <qgsrasterlayer.h>

qgis_devLayerTreeViewMenuProvider::qgis_devLayerTreeViewMenuProvider( QgsLayerTreeView *view, QgsMapCanvas* canvas )
    : mView( view )
    , mCanvas( canvas )
{
}

qgis_devLayerTreeViewMenuProvider::~qgis_devLayerTreeViewMenuProvider()
{

}

QMenu* qgis_devLayerTreeViewMenuProvider::createContextMenu()
{
    // 设置这个路径是为了获取图标文件
    QString iconDir = "../images/themes/default/";

    QMenu* menu = new QMenu;

    QgsLayerTreeViewDefaultActions* actions = mView->defaultActions();

    QModelIndex idx = mView->currentIndex();

    // global menu
    if ( !idx.isValid() )
    {
        menu->addAction( actions->actionAddGroup( menu ) );
        menu->addAction( QIcon( iconDir + "mActionExpandTree.png" ), tr( "&Expand All" ), mView, SLOT( expandAll() ) );
        menu->addAction( QIcon( iconDir + "mActionCollapseTree.png" ), tr( "&Collapse All" ), mView, SLOT( collapseAll() ) );
    }
    else if ( QgsLayerTreeNode* node = mView->layerTreeModel()->index2node( idx ) )
    {
        // layer or group selected
        if ( QgsLayerTree::isGroup( node ) )
        {
            menu->addAction( actions->actionZoomToGroup( mCanvas, menu ) );
            menu->addAction( QIcon( iconDir + "mActionRemoveLayer.svg" ), tr( "&Remove" ), qgis_dev::instance(), SLOT( removeLayer() ) );
            menu->addAction( actions->actionRenameGroupOrLayer( menu ) );
            if ( mView->selectedNodes( true ).count() >= 2 )
            {
                menu->addAction( actions->actionGroupSelected( menu ) );
            }
            menu->addAction( actions->actionAddGroup( menu ) );
        }
        else if ( QgsLayerTree::isLayer( node ) )
        {
            QgsMapLayer* layer = QgsLayerTree::toLayer( node )->layer();
            menu->addAction( actions->actionZoomToLayer( mCanvas, menu ) );
            menu->addAction( actions->actionShowInOverview( menu ) );

            // 如果选择的是栅格图层
            if ( layer && layer->type() == QgsMapLayer::RasterLayer )
            {
                menu->addAction( tr( "&Zoom to Best Scale (100%)" ), qgis_dev::instance(), SLOT( legendLayerZoomNative() ) );

                QgsRasterLayer* rasterLayer =  qobject_cast<QgsRasterLayer *>( layer );
                if ( rasterLayer && rasterLayer->rasterType() != QgsRasterLayer::Palette )
                {
                    menu->addAction( tr( "&Stretch Using Current Extent" ), qgis_dev::instance(), SLOT( legendLayerStretchUsingCurrentExtent() ) );
                }
            }

            menu->addAction( QIcon( "mActionRemoveLayer.svg" ), tr( "&Remove" ), qgis_dev::instance(), SLOT( removeLayer() ) );
        }
    }

    return menu;
}

void qgis_devLayerTreeViewMenuProvider::addLegendLayerAction( QAction * action, QString menu, QString id, QgsMapLayer::LayerType type, bool allLayers )
{

}

void qgis_devLayerTreeViewMenuProvider::addLegendLayerActionForLayer( QAction * action, QgsMapLayer * layer )
{

}

void qgis_devLayerTreeViewMenuProvider::removeLegendLayerActionsForLayer( QgsMapLayer * layer )
{

}

QList< LegendLayerAction > qgis_devLayerTreeViewMenuProvider::legendLayerActions( QgsMapLayer::LayerType type ) const
{
    return mLegendLayerActionMap.contains( type ) ? mLegendLayerActionMap.value( type ) : QList<LegendLayerAction>();
}
