#ifndef QGIS_DEV_H
#define QGIS_DEV_H

#include <QtGui/QMainWindow>
#include "ui_qgis_dev.h"

#include <QList>
#include <QDockWidget>
#include <QProgressBar>
#include <QLabel>
#include <QDockWidget>
#include <QIcon>

// QGis include
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgslayertreeview.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgsscalecombobox.h>
#include <qgsrendererv2.h>

class qgis_dev : public QMainWindow
{
    Q_OBJECT

public:
    qgis_dev( QWidget *parent = 0, Qt::WindowFlags flags = Qt::Window );
    ~qgis_dev();

    void addDockWidget( Qt::DockWidgetArea area, QDockWidget* dockwidget );

    //! 保持单一实例
    static qgis_dev* instance() { return sm_instance; }

    static QIcon getThemeIcon( const QString &theName );

    QgsMapCanvas* mapCanvas() {return m_mapCanvas; }

public slots:
    //! 添加矢量图层
    void addVectorLayers();
    //! 添加栅格图层
    void addRasterLayers();

    void autoSelectAddedLayer( QList<QgsMapLayer*> layers );
    //! 打开属性表
    void openAttributeTableDialog();

    //! 改变矢量图层的显示样式
    void changeSymbol( QgsVectorLayer* layer, QgsFeatureRendererV2* featureRenderer );
    //! 演示如何使用矢量图层显示样式
    void layerSymbolTest();
    //! 移除图层
    void removeAllLayers();
    void removeLayer();

private slots:
    //! 显示鼠标位置地理坐标
    void showMouseCoordinate( const QgsPoint & );
    //! 创建鹰眼图
    void createOverview();

private:
    Ui::qgis_devClass ui;
    static qgis_dev* sm_instance;

    QgsMapCanvas* m_mapCanvas; // 地图画布

    //! 图层管理
    QgsLayerTreeView* m_layerTreeView;
    QgsLayerTreeMapCanvasBridge *m_layerTreeCanvasBridge;

    QDockWidget *m_layerTreeDock; // 装图层管理器
    QDockWidget *m_layerOrderDock;
    QDockWidget *m_OverviewDock; // 装鹰眼图
    QDockWidget *m_pGpsDock;
    QDockWidget *m_logDock;

    QLabel* m_scaleLabel; // 在状态栏中显示“scale 1:”
    QgsScaleComboBox* m_scaleEdit; //! 在状态栏中显示比例尺值
    QProgressBar* m_progressBar;
    QLabel* m_coordsLabel; //! 在状态栏显示"Coordinate / Extent"
    QLineEdit* m_coordsEdit; //! 在状态栏显示地理坐标

    QList<QgsMapCanvasLayer> mapCanvasLayerSet; // 地图画布所用的图层集合

    QCursor* m_overviewMapCursor;

    unsigned int m_MousePrecisionDecimalPlaces;

    bool m_toggleOverview;


//=== Private Member Functions ===
    //! 初始化图层管理器
    void initLayerTreeView();
    //! 构建状态栏
    void createStatusBar();

    //! 获取当前选中图层
    QgsMapLayer* activeLayer();

    //! 用于获取images资源库当中的Icon路径
    static const QString activeThemePath();
    static const QString defaultThemePath();
};

#endif // QGIS_DEV_H
