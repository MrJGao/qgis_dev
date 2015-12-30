#ifndef QGIS_DEV_H
#define QGIS_DEV_H

#include <QtGui/QMainWindow>
#include "ui_qgis_dev.h"

#include <QList>
#include <QDockWidget>
#include <QProgressBar>
#include <QLabel>
#include <QDockWidget>

// QGis include
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgslayertreeview.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgsscalecombobox.h>

class qgis_dev : public QMainWindow
{
    Q_OBJECT

public:
    qgis_dev( QWidget *parent = 0, Qt::WFlags flags = 0 );
    ~qgis_dev();

    void addDockWidget( Qt::DockWidgetArea area, QDockWidget* dockwidget );

    //! 保持单一实例
    static inline qgis_dev *instance() { return sm_instance; };
public slots:
    //! 添加矢量图层
    void addVectorLayers();
    //! 添加栅格图层
    void addRasterLayers();

    void autoSelectAddedLayer( QList<QgsMapLayer*> layers );
private:
    Ui::qgis_devClass ui;
    static qgis_dev* sm_instance;

    QgsMapCanvas* m_mapCanvas; // 地图画布

    //! 图层管理
    QgsLayerTreeView* m_layerTreeView;
    QgsLayerTreeMapCanvasBridge *m_layerTreeCanvasBridge;


    QDockWidget *m_layerTreeDock;
    QDockWidget *m_layerOrderDock;
    /*QDockWidget *m_OverviewDock;
    QDockWidget *m_pGpsDock;
    QDockWidget *m_logDock;*/

    QLabel* m_scaleLabel; // 在状态栏中显示“scale 1:”
    QgsScaleComboBox* m_scaleEdit; //! 在状态栏中显示比例尺值
    QProgressBar* m_progressBar;
    QLabel* m_coordsLabel; //! 在状态栏显示"Coordinate / Extent"
    QLineEdit* m_coordsEdit; //! 在状态栏显示地理坐标

    QList<QgsMapCanvasLayer> mapCanvasLayerSet; // 地图画布所用的图层集合



    void initLayerTreeView();
    void createStatusBar();

};

#endif // QGIS_DEV_H
