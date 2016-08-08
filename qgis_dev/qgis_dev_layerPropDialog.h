#ifndef QGIS_DEV_LABEL_H
#define QGIS_DEV_LABEL_H

#include <QObject>
#include <QDialog>
#include <ui_qgis_dev_layerPropDialog.h>
#include <qgis_dev_labelinggui.h>

class QgsVectorLayer;
class QgsMapCanvas;
class QTreeWidgetItem;

//! 用于显示矢量图层的设置
class qgis_dev_layerPropDialog : public QDialog , public Ui::qgis_dev_layerPropDialog
{
    Q_OBJECT

public:
    qgis_dev_layerPropDialog( QWidget *parent, QgsVectorLayer* layer, QgsMapCanvas* mapCanvas );
    ~qgis_dev_layerPropDialog();

    //! 添加一个属性到选择列表中，并附上一个随机的颜色
    void addAttribute( QTreeWidgetItem* item );

public slots:
    void apply();

    //! 透明度滑块设置值
    void on_mTransparencySlider_valueChanged( int value );
    //! 添加种类按钮按下事件
    void on_mAddCategoryPushButton_clicked();
    //! 删除种类按钮按下事件
    void on_mRemoveCategoryPushButton_clicked();
    //! 属性窗口中的元素双击事件
    void on_mAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column );
    //! 查找最大值最小按钮事件
    void on_mFindMaximumValueButton_clicked();
    //! 显示图表checkbox事件
    void on_mDisplayDiagramsGroupBox_toggled( bool checked );
    //! 图表属性列表中元素双击事件
    void on_mDiagramAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column );
    //!
    void on_mEngineSettingsButton_clicked();
    //! 显示大小属性表达式窗口
    void showSizeAttributeExpressionDialog();
    //! 添加属性表达式窗口
    void showAddAttributeExpressionDialog();

private slots:
    //! 确定按钮事件
    void settingsIsOk();

    //! 标注
    void ifDisLabel( int dis );

    //! 图表
    void enableDiagram();

    void diagramChanged( int index );

    //! 选择图表背景颜色
    void on_mBackgroundColorButton_clicked();
    //! 选择图表线颜色
    void on_mDiagramPenColorButton_clicked();

private:
    //! 当前图层指针
    QgsVectorLayer* m_layer;
    //! 地图画布指针
    QgsMapCanvas* m_mapCanvas;

    qgis_dev_labelingGui* labelingGui;

    int m_availableAttributeCount;

    //! 初始化柱状图设置UI
    void setupBarUI();
    //! 初始化饼状图设置UI
    void setupPieUI();
    //! 初始化文本图设置UI
    void setupTextUI();

    //! 初始化属性字段选择面板
    void initAttributeSelection();


    //! 常规设置
    void setGenericProperty();

    //! 样式设置
    void setStyleProperty();

    //! 字段设置
    void setFieldProperty();

    //! 图表设置
    void setDiagramProperty();

    //! 标注设置
    void setLableProperty();
};


#endif // QGIS_DEV_LABEL_H
