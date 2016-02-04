#ifndef QGIS_DEV_H
#define QGIS_DEV_H

#include <QtGui/QMainWindow>
#include "ui_qgis_dev.h"

#include "qgis_devComposer.h"

#include <QList>
#include <QDockWidget>
#include <QProgressBar>
#include <QLabel>
#include <QDockWidget>
#include <QIcon>
#include <QStackedWidget>
#include <QTimer>

// QGis include
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgslayertreeview.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgsscalecombobox.h>
#include <qgsrendererv2.h>
#include <qgscomposermap.h>
#include <qgscomposerview.h>
#include <qgsraster.h>
#include <qgsmessagebar.h>


class QgsMapTool;

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

    QgsMessageBar* messageBar() {return m_infoBar;}

    //! 控制信息显示条显示的时间
    int messageTimeout();

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

    //! 地图工具触发事件
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPan_triggered();
    void on_actionIdentify_triggered();

private slots:
    //! 显示鼠标位置地理坐标
    void showMouseCoordinate( const QgsPoint & );
    //! 创建鹰眼图
    void createOverview();
    //! 显示当前地图比例
    void showScale( double theScale );
    //! 根据用户指定比例缩放地图
    void userScale();
    //! 居中显示用户在坐标显示栏里输入的坐标
    void userCenter();

    //! 刷新地图显示
    void refreshMapCanvas();

    //! 更改地图单位 , 目前仅用在坐标显示的控件上，未来应该会调整接口
    void mapUnitChange( int i );
    //! 晕眩效果
    void dizzy(); // 这个效果是qgis程序员隐藏的娱乐方式吧，just for fun


/// 显示栅格图像的一些功能
    //! 局部拉伸显示
    void localHistogramStretch();
    //! 全局拉伸显示
    void fullHistogramStretch();
    //! 局部累计裁剪拉伸显示
    void localCumulativeCutStretch();
    //! 全局累计裁剪拉伸显示
    void fullCumulativeCutStretch();
    //! 增加显示亮度
    void increaseBrightness();
    //! 减少显示亮度
    void decreaseBrightness();
    //! 增加显示对比度
    void increaseContrast();
    //! 减少显示对比度
    void decreaseContrast();



private:
    Ui::qgis_devClass ui;
    static qgis_dev* sm_instance;

    // 地图工具
    QgsMapTool* m_mapToolZoomIn;
    QgsMapTool* m_mapToolZoomOut;
    QgsMapTool* m_mapToolZoomFull;
    QgsMapTool* m_mapToolPan;
    QgsMapTool* m_mapToolIdentify;


    QStackedWidget* m_stackedWidget;
    QComboBox* pageViewComboBox;

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
    QLabel* m_coordsLabel; // 在状态栏显示"Coordinate / Extent"
    QLineEdit* m_coordsEdit; // 在状态栏显示地理坐标

    QList<QgsMapCanvasLayer> mapCanvasLayerSet; // 地图画布所用的图层集合

    QCursor* m_overviewMapCursor; // 鹰眼图用的鼠标指针

    unsigned int m_MousePrecisionDecimalPlaces; // 指定鼠标坐标小数点位数

    qgis_devComposer* m_composer; // 地图制图模块

    //! Helps to make people dizzy
    QTimer* m_dizzyTimer;

    QgsMessageBar* m_infoBar; // 在地图窗口上显示信息

//=== Private Member Functions ===

    //! 初始化图层管理器
    void initLayerTreeView();
    //! 构建状态栏
    void createStatusBar();

    //! 初始化地图工具
    void createMapTools();


    //! 构建打印出图视图
    void createComposer();
    //! 获取当前选中图层
    QgsMapLayer* activeLayer();

    //! 用于获取images资源库当中的Icon路径
    static const QString activeThemePath();
    static const QString defaultThemePath();


    //! 直方图拉伸
    void histogramStretch( bool visibleAreaOnly = false, QgsRaster::ContrastEnhancementLimits theLimits = QgsRaster::ContrastEnhancementMinMax );
    //! 对比度调整, updateBrightness用来控制调整对比度时是否需要同时调整亮度
    void adjustBrightnessContrast( int delta, bool updateBrightness = true );
};

#endif // QGIS_DEV_H
