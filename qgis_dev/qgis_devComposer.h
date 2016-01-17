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
    void toggleRulers( bool );

    void on_actionZoomAll_triggered();
    void on_actionZoomActual_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();

    void on_actionShowGrid_triggered( bool checked );
    void on_actionShowFullScreen_triggered( bool checked );

    void on_actionAddArrow_triggered();
    void on_actionPanComposer_triggered();
    void on_actionSelectTool_triggered();
    void on_actionAddShape_triggered();
    void on_actionAddMap_triggered();
    void on_actionAddHtml_triggered();


protected:
    virtual void moveEvent( QMoveEvent* ) override;


private slots:
    void setComposition( QgsComposition* composition );

private:
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