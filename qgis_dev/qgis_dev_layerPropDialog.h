#ifndef QGIS_DEV_LABEL_H
#define QGIS_DEV_LABEL_H

#include <QObject>
#include <QDialog>
#include <ui_qgis_dev_layerPropDialog.h>

class QgsVectorLayer;
class QgsMapCanvas;

//! 用于显示矢量图层的标注
class qgis_dev_layerPropDialog : public QDialog , public Ui::qgis_dev_layerPropDialog
{
    Q_OBJECT

public:
    qgis_dev_layerPropDialog( QWidget *parent, QgsVectorLayer* layer, QgsMapCanvas* mapCanvas );
    ~qgis_dev_layerPropDialog();

private slots:
    void enableLable();
    void ifDisLabel( bool dis );

private:
    //! 当前图层指针
    QgsVectorLayer* m_layer;
    //! 地图画布指针
    QgsMapCanvas* m_mapCanvas;

};


#endif // QGIS_DEV_LABEL_H
