#ifndef QGIS_DEV_LABELINGGUI_H
#define QGIS_DEV_LABELINGGUI_H

#include <QDialog>
#include <ui_qgslabelingguibase.h>
#include <QFontDatabase>
#include <qgspallabeling.h>

class QgsVectorLayer;
class QgsMapCanvas;
class QgsCharacterSelectorDialog;

class qgis_dev_labelingGui : public QWidget, private Ui::QgsLabelingGuiBase
{
    Q_OBJECT

public:
    qgis_dev_labelingGui( QgsVectorLayer* layer, QgsMapCanvas* mapCanvas, QWidget* parent );
    ~qgis_dev_labelingGui();

    //! 图层配置
    QgsPalLayerSettings layerSettings();
    //! 将配置写到图层中
    void writeSettingsToLayer();

public slots:
    //! 初始化
    void init();
    void collapseSample( bool collapse );
    void apply();

    //! 改变字体颜色
    void changeTextColor( const QColor& color );
    //! 打开标注引擎设置对话框
    void showEngineConfigDialog();
    //! 改变缓冲颜色
    void changeBufferColor( const QColor& color );

    //! 更新UI
    void updateUi();
    //! 更新预览视图
    void updatePreview();
    //! 滚动预览视图
    void scrollPreview();
    //! 更新位置控件
    void updatePlacementWidgets();
    //! 更新SVG控件
    void updateSvgWidgets( const QString& svgPath );

    void on_mPreviewSizeSlider_valueChanged( int i );
    void on_mFontSizeSpinBox_valueChanged( double d );
    void on_mFontCapitalsComboBox_currentIndexChanged( int index );
    void on_mFontFamilyCmbBx_currentFontChanged( const QFont& f );
    void on_mFontStyleComboBox_currentIndexChanged( const QString & text );
    void on_mFontUnderlineBtn_toggled( bool ckd );
    void on_mFontStrikethroughBtn_toggled( bool ckd );
    void on_mFontWordSpacingSpinBox_valueChanged( double spacing );
    void on_mFontLetterSpacingSpinBox_valueChanged( double spacing );
    void on_mFontSizeUnitWidget_changed();
    void on_mFontMinPixelSpinBox_valueChanged( int px );
    void on_mFontMaxPixelSpinBox_valueChanged( int px );
    void on_mBufferUnitWidget_changed();
    void on_mCoordXDDBtn_dataDefinedActivated( bool active );
    void on_mCoordYDDBtn_dataDefinedActivated( bool active );

    void on_mShapeTypeCmbBx_currentIndexChanged( int index );
    void on_mShapeRotationCmbBx_currentIndexChanged( int index );
    void on_mShapeSVGParamsBtn_clicked();
    void on_mShapeSVGSelectorBtn_clicked();

    void on_mPreviewTextEdit_textChanged( const QString & text );
    void on_mPreviewTextBtn_clicked();
    void on_mPreviewBackgroundBtn_colorChanged( const QColor &color );
    void on_mDirectSymbLeftToolBtn_clicked();
    void on_mDirectSymbRightToolBtn_clicked();

protected:
    void blockInitSignals( bool block );
    void blockFontChangeSignals( bool blk );
    void setPreviewBackground( QColor color );
    void syncDefinedCheckboxFrame( QgsDataDefinedButton* ddBtn, QCheckBox* chkBx, QFrame* f );
    void populateFontCapitalsComboBox();
    void populateFontStyleComboBox();
    void populatePlacementMethods();
    void populateFieldNames();
    void populateDataDefinedButtons( QgsPalLayerSettings& s );
    /**Sets data defined property attribute to map */
    void setDataDefinedProperty( const QgsDataDefinedButton* ddBtn, QgsPalLayerSettings::DataDefinedProperties p, QgsPalLayerSettings& lyr );
    void updateFont( QFont font );

private:
    QgsVectorLayer* mLayer;
    QgsMapCanvas* mMapCanvas;
    QFontDatabase mFontDB;
    QgsCharacterSelectorDialog* mCharDlg;

    QButtonGroup* mQuadrantBtnGrp;
    QButtonGroup* mDirectSymbBtnGrp;
    QButtonGroup* mUpsidedownBtnGrp;

    QButtonGroup* mPlacePointBtnGrp;
    QButtonGroup* mPlaceLineBtnGrp;
    QButtonGroup* mPlacePolygonBtnGrp;

    // background reference font
    QFont mRefFont;
    int mPreviewSize;

    int mMinPixelLimit;

    bool mLoadSvgParams;

    void enableDataDefinedAlignment( bool enable );

private slots:
    void optionsStackedWidget_CurrentChanged( int indx );
    void showBackgroundRadius( bool show );
    void showBackgroundPenStyle( bool show );
    void on_mShapeSVGPathLineEdit_textChanged( const QString& text );

};

#endif // QGIS_DEV_LABELINGGUI_H
