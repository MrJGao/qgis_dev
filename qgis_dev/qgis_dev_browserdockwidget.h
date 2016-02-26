#ifndef QGIS_DEV_BROWSERDOCKWIGHT_H
#define QGIS_DEV_BROWSERDOCKWIGHT_H

#include <QDockWidget>
#include <ui_qgsbrowserdockwidgetbase.h>

#include <QModelIndex>
#include <QShowEvent>

class QgsLayerItem;
class QgsBrowserModel;
class QModelIndex;
class qgis_dev_dockBrowserTreeView;
class QgsDataItem;
class qgis_dev_browserTreeFilterProxyModel;

//! 文件浏览器dock widget类，提供文件浏览、过滤文件、添加图层等基本操作
class qgis_dev_browserDockWidget : public QDockWidget, private Ui::QgsBrowserDockWidgetBase
{
    Q_OBJECT

public:
    qgis_dev_browserDockWidget( QString name, QWidget *parent = 0 );
    ~qgis_dev_browserDockWidget();

    //! 添加指定路径到收藏
    void addFavouriteDirectory( QString favDir );

public slots:
    //! 添加选中的图层
    void addLayerAtIndex( const QModelIndex& index );
    //! 右键菜单
    void showContextMenu( const QPoint& );

    //! 添加到收藏
    void addFavourite();
    //! 添加目录到收藏
    void addFavouriteDirectory();
    //! 移除收藏
    void removeFavourite();

    //! 刷新
    void refresh();

    //! 显示过滤器组件
    void showFilterWidget( bool visible );
    void setFilterSyntax( QAction* action );
    //! 设置大小写敏感
    void setCaseSensitive( bool caseSensitive );
    //! 设置过滤器
    void setFilter();

    //==== 图层数据右键菜单方法 ====
    //! 添加当前图层
    void addCurrentLayer();
    //! 添加选中的所有图层
    void addSelectedLayers();
    //! 显示属性信息
    void showProperties();
    //! 切换快速扫描
    void toggleFastScan();

protected:
    void refreshModel( const QModelIndex& index );
    //! 重写 showEvent 方法，并在这里初始化 m_model 等对象
    void showEvent( QShowEvent* event ) override;
    void addLayer( QgsLayerItem* layerItem );

    qgis_dev_dockBrowserTreeView* m_browserView; // 文件浏览器view
    QgsBrowserModel* m_model; // 文件浏览器model
    qgis_dev_browserTreeFilterProxyModel* m_proxyModel; // 文件浏览器过滤model
    QString m_initPath; // 初始路径

};

#endif // QGIS_DEV_BROWSERDOCKWIGHT_H
