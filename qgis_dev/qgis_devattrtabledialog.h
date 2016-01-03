#ifndef QGIS_DEVATTRTABLEDIALOG_H
#define QGIS_DEVATTRTABLEDIALOG_H

#include <QDialog>
#include "ui_qgis_devattrtabledialog.h"
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

private slots:
    void updateTitle(); // update title of this window

    void filterExpressionBuilder();
    void filterShowAll();
    void filterSelected();
    void filterVisible();
    void filterEdited();
    void filterColumnChanged( QObject* filterAction );
    void filterQueryChanged( const QString& query );
    void filterQueryAccepted();

    void columnBoxInit(); // 字段名称初始化字段显示列表

    // 执行字段计算器
    void runFieldCalculation( QgsVectorLayer* layer, QString fieldName,
                              QString expression, QgsFeatureIds filteredIds = QgsFeatureIds() );
    void updateFieldFromExpression(); // 根据用户输入的Expression更新字段
    void updateFieldFromExpressionSelected(); // 根据用户选择的feature与输入的Expression更新字段
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
