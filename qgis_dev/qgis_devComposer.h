#ifndef QGIS_DEVCOMPOSER_H
#define QGIS_DEVCOMPOSER_H

#include <QObject>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QMoveEvent>
#include "ui_qgis_dev_composerBase.h"

#include <qgscomposerview.h>
#include <qgscomposition.h>
#include <qgscomposerruler.h>

#include <qgsmapcanvas.h>

class qgis_devComposer : public QMainWindow , public Ui_composerBase
{
    Q_OBJECT
public:
    //! 用于控制输出模式
    enum OutputMode
    {
        Single = 0, // 只输出单个feature
        Atlas // 输出整个合成
    };

    qgis_devComposer( QWidget *parent = 0 );
    ~qgis_devComposer();

    //! 返回map canvas指针
    QgsMapCanvas* mapCanvas();

    //! 返回composer view的指针
    QgsComposerView* view() { return m_composerView; }

    //! 返回当前的合成
    QgsComposition* composition() {return m_composition;}

signals:
    //! 缩放比例变化后，就发出这个signal
    void zoomLevelChanged();

public slots:
    //! 合成视图浏览工具
    void on_actionPanComposer_triggered();
    void on_actionZoomAll_triggered();
    void on_actionZoomActual_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionShowFullScreen_triggered( bool checked );

    //! 合成视图辅助工具
    void on_actionShowGrid_triggered( bool checked );
    void on_actionToggleRulers_triggered( bool );


    //! 地图元素工具
    void on_actionSelectTool_triggered();
    void on_actionAddShape_triggered();
    void on_actionAddMap_triggered();
    void on_actionAddArrow_triggered();
    void on_actionAddHtml_triggered();

    //! 导出当前合成
    void on_actionExportAsImage_triggered();
    void on_actionExportAsSvg_triggered();
    void on_actionExportAsPDF_triggered();

protected:
    virtual void moveEvent( QMoveEvent* ) override;


private slots:
    void setComposition( QgsComposition* composition );

private:

    void exportCompositionAsImage( qgis_devComposer::OutputMode mode );


    QString m_title; //! 合成标题

    QgsComposerView* m_composerView;
    QGridLayout* m_viewLayout;
    QgsComposerRuler* m_horizontalRuler;
    QgsComposerRuler* m_verticalRuler;
    QWidget* m_rulerLayoutFix;

    QgsComposition* m_composition; //! 当前合成对象指针

    QWidget* mainWidget; //! 窗口的central widget
};

#endif // QGIS_DEVCOMPOSER_H