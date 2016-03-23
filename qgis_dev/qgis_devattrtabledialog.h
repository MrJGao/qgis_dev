#ifndef QGIS_DEVATTRTABLEDIALOG_H
#define QGIS_DEVATTRTABLEDIALOG_H

#include <QDialog>
#include "ui_qgsattributetabledialog.h"

#include <QDockWidget>
#include <QSignalMapper>

#include <qgsvectorlayer.h>
#include <qgsfieldmodel.h>
#include <qgsrubberband.h>

// 属性表对话框
class qgis_devattrtableDialog : public QDialog, public Ui::QgsAttributeTableDialog
{
    Q_OBJECT

public:
    qgis_devattrtableDialog( QgsVectorLayer* theVecLayer, QWidget *parent = 0, Qt::WindowFlags flags = Qt::Window );
    ~qgis_devattrtableDialog();

    /**
    * Sets the filter expression to filter visible features
    * @param filterString filter query string. QgsExpression compatible.
    */
    void setFilterExpression( QString filterString );

public slots:
    void editingToggled(); // Toggles editing mode

signals:
    //! 该图层编辑状态打开消息
    void editingToggled( QgsMapLayer* layer );
    //! 保存更改消息
    void saveEdits( QgsMapLayer* layer );

private slots:
    /// ==== 按钮事件 ====
    //! 复制选择的行
    void on_mCopySelectedRowsButton_clicked();
    //! 切换编辑模式
    void on_mToggleEditingButton_toggled();
    //! 保存编辑
    void on_mSaveEditsButton_clicked();

    //! 反选
    void on_mInvertSelectionButton_clicked();
    //! 清除选择
    void on_mRemoveSelectionButton_clicked();
    //! 缩放到所选择的行
    void on_mZoomMapToSelectedRowsButton_clicked();
    //! 漫游到所选行
    void on_mPanMapToSelectedRowsButton_clicked();
    //! 移动选择行到顶端
    void on_mSelectedToTopButton_toggled();

    //! 打开添加属性对话框
    void on_mAddAttribute_clicked();

    //! 打开移除属性对话框
    void on_mRemoveAttribute_clicked();
    //! 打开字段计算器
    void on_mOpenFieldCalculator_clicked();

    //! 删除选择的要素
    void on_mDeleteSelectedButton_clicked();

    //! 当窗口状态改变时（例如从属性表状态切换到视图状态），更改菜单上按钮的状态
    void on_mMainView_currentChanged( int );

    //! 添加要素
    void on_mAddFeature_clicked();
    //! 帮助
    void on_mHelpButton_clicked();

    //! 打开根据属性查询要素对话框
    void on_mExpressionSelectButton_clicked();

    //! 更新属性窗口标题
    void updateTitle();

    //! 过滤表达式构建器，从 advanced filter 触发
    void filterExpressionBuilder();
    //! 过滤显示所有要素
    void filterShowAll();
    //! 过滤仅显示已选择要素
    void filterSelected();
    //! 过滤仅显示可见要素
    void filterVisible();
    //! 过滤仅显示已编辑要素
    void filterEdited();
    //! 过滤列更改事件
    void filterColumnChanged( QObject* filterAction );
    //! 过滤查询更改事件
    void filterQueryChanged( const QString& query );
    //! 应用过滤查询
    void filterQueryAccepted();

    //! 字段名称初始化字段显示列表
    void columnBoxInit();

    //! 执行字段计算器
    void runFieldCalculation( QgsVectorLayer* layer, QString fieldName,
                              QString expression, QgsFeatureIds filteredIds = QgsFeatureIds() );
    //! 根据用户输入的Expression更新字段
    void updateFieldFromExpression();
    //! 根据用户选择的feature与输入的Expression更新字段
    void updateFieldFromExpressionSelected();
    //! 更新按钮状态
    void updateButtonStatus( QString fieldName, bool isValid );
private:

    QMenu* mMenuActions; // 菜单动作
    QAction* mActionToggleEditing; // 编辑开关

    QDockWidget* mDock; // 使得属性表窗口可Dock
    QgsDistanceArea* myDa;

    QMenu* mFilterColumnsMenu;
    QSignalMapper* mFilterActionMapper;

    QgsVectorLayer* mLayer; // 显示该图层的属性信息
    QgsFieldModel* mFieldModel; // 字段数据模型

    QgsRubberBand* mRubberBand;
};

#endif // QGIS_DEVATTRTABLEDIALOG_H
