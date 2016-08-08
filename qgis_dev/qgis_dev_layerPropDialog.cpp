#include "qgis_dev_layerPropDialog.h"
#include "qgis_dev.h"
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgslabel.h>
#include <qgslabelattributes.h>

#include <QColorDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QBrush>

// for diagrams
#include <qgshistogramdiagram.h>
#include <qgspiediagram.h>
#include <qgstextdiagram.h>
#include <qgsdiagramrendererv2.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorsimplifymethod.h>

#include "qgis_dev_labelinggui.h"

qgis_dev_layerPropDialog::qgis_dev_layerPropDialog( QWidget *parent, QgsVectorLayer* layer, QgsMapCanvas* mapCanvas )
    : QDialog( parent ), m_layer( layer ), m_mapCanvas( mapCanvas ), m_availableAttributeCount( 0 )
{
    if( !m_layer ) {return;}
    setupUi( this );


    // 初始化 LabelingGui
    labelingGui = new qgis_dev_labelingGui( layer, mapCanvas, this->m_labelingFrame );
    m_labelingFrame->setEnabled( false );
    labelingGui->init();
    connect( this->m_label_checkBox, SIGNAL( toggled( bool ) ), m_labelingFrame, SLOT( setEnabled( bool )  ) );

    // 设置diagram页面
    connect( diagramType_comboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( diagramChanged( int ) ) );

    this->mDisplayDiagramsGroupBox->setChecked( false );
    //connect( this->disDiag_checkBox, SIGNAL( stateChanged( int ) ), this, SLOT( enableDiagram( int ) ) );
    //connect( this->diagSelect_comboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( diagramChanged( int ) ) );

    //connect( this->okButton, SIGNAL( clicked() ), this, SLOT( settingsIsOk() ) );
    connect( this->okButton, SIGNAL( clicked() ), this, SLOT( apply() ) );
}


qgis_dev_layerPropDialog::~qgis_dev_layerPropDialog()
{

}

void qgis_dev_layerPropDialog::ifDisLabel( int state )
{
    /*if ( state == 0 ) {this->labelField_comboBox->setEnabled( false );}
    else if( state == 2 ) {this->labelField_comboBox->setEnabled( true );}*/
}

void qgis_dev_layerPropDialog::enableDiagram()
{
    QGis::GeometryType layerType = m_layer->geometryType();
    if ( layerType == QGis::UnknownGeometry || layerType == QGis::NoGeometry )
    {
        mDisplayDiagramsGroupBox->setChecked( false );
        mDisplayDiagramsGroupBox->setEnabled( false );
    }

    // 获取图层所有字段，初始化tree widget 和 combo box
    const QgsFields& layerFields = m_layer->pendingFields();
    for ( int idx = 0; idx < layerFields.count(); ++idx )
    {
        QTreeWidgetItem* newItem = new QTreeWidgetItem( mAttributesTreeWidget );
        QString name = QString( "\"%1\"" ).arg( layerFields[idx].name() );
        newItem->setText( 0, name );
        newItem->setData( 0, Qt::UserRole, name );
        newItem->setFlags( newItem->flags() & ~Qt::ItemIsDropEnabled );
        if ( layerFields[idx].type() == QVariant::String )
        {
            // 初始化combo box
            mSizeAttributeComboBox->addItem( layerFields[idx].name(), idx );
        }
    }

    // 插入placement选项
    if ( layerType == QGis::Point || layerType == QGis::Polygon )
    {
        mPlacementComboBox->addItem( tr( "Around Point" ), QgsDiagramLayerSettings::AroundPoint );
        mPlacementComboBox->addItem( tr( "Over Point" ), QgsDiagramLayerSettings::OverPoint );
    }
    else if ( layerType == QGis::Line || layerType == QGis::Polygon )
    {
        mPlacementComboBox->addItem( tr( "Line" ), QgsDiagramLayerSettings::Line );
        mPlacementComboBox->addItem( tr( "Horizontal" ), QgsDiagramLayerSettings::Horizontal );
    }
    else if ( layerType == QGis::Polygon )
    {
        mPlacementComboBox->addItem( tr( "Free" ), QgsDiagramLayerSettings::Free );
    }
    else if ( layerType == QGis::Line )
    {
        mLineOptionsComboBox->addItem( tr( "On line" ), QgsDiagramLayerSettings::OnLine );
        mLineOptionsComboBox->addItem( tr( "Above line" ), QgsDiagramLayerSettings::AboveLine );
        mLineOptionsComboBox->addItem( tr( "Below Line" ), QgsDiagramLayerSettings::BelowLine );
        mLineOptionsComboBox->addItem( tr( "Map orientation" ), QgsDiagramLayerSettings::MapOrientation );
    }
    else
    {
        mLineOptionsComboBox->setVisible( false );
        mLineOptionsLabel->setVisible( false );
    }

    mLabelPlacementComboBox->addItem( tr( "Height" ), QgsDiagramSettings::Height );
    mLabelPlacementComboBox->addItem( tr( "x-height" ), QgsDiagramSettings::XHeight );

    mAngleOffsetComboBox->addItem( tr( "Top" ), 90 * 16 );
    mAngleOffsetComboBox->addItem( tr( "Right" ), 0 );
    mAngleOffsetComboBox->addItem( tr( "Bottom" ), 270 * 16 );
    mAngleOffsetComboBox->addItem( tr( "Left" ), 180 * 16 );
}

void qgis_dev_layerPropDialog::setupBarUI()
{

}

void qgis_dev_layerPropDialog::setupPieUI()
{

}

void qgis_dev_layerPropDialog::setupTextUI()
{

}

void qgis_dev_layerPropDialog::initAttributeSelection()
{

}

void qgis_dev_layerPropDialog::diagramChanged( int index )
{
    if ( index == 0 ) // pie
    {
        mAngleOffsetComboBox->show();
        mAngleOffsetLabel->show();
    }
    else
    {
        mAngleOffsetComboBox->hide();
        mAngleOffsetLabel->hide();
    }

    if ( index == 1 ) // hist
    {
        mBarWidthLabel->show();
        mBarWidthSpinBox->show();
        mOrientationFrame->show();
        mFixedSizeCheckBox->setChecked( false );
        mFixedSizeCheckBox->setVisible( false );
        //mDiagramSizeSpinBox->setVisible( false );
        mLinearlyScalingLabel->setText( tr( "Bar length: Scale linearly, such as the following value matches the specified size." ) );
    }
    else
    {
        mBarWidthLabel->hide();
        mBarWidthSpinBox->hide();
        mOrientationFrame->hide();
        mFixedSizeCheckBox->setChecked( true );
        mFixedSizeCheckBox->setVisible( true );
        //mDiagramSizeSpinBox->setVisible( false );
        mLinearlyScalingLabel->setText( tr( "Bar length: Scale linearly, such as the following value matches the specified size." ) );
    }

    if ( index == 2 ) // text
    {
        mLabelPlacementComboBox->show();
        mLabelPlacementLabel->show();
        mBackgroundColorLabel->show();
        mBackgroundColorButton->show();
        font_comboBox->show();
    }
    else
    {
        mLabelPlacementComboBox->hide();
        mLabelPlacementLabel->hide();
        mBackgroundColorLabel->hide();
        mBackgroundColorButton->hide();
        font_comboBox->hide();
    }

    if ( index == 1 || index == 2 ) // hist or text
    {
        mDiagramPropertiesTabWidget->setTabEnabled( 3, true );
    }
    else
    {
        mDiagramPropertiesTabWidget->setTabEnabled( 3, false );
    }

    if ( index == 0 || index == 2 ) // text and pie
    {

    }
    else
    {

    }

}

void qgis_dev_layerPropDialog::settingsIsOk()
{
    /*if ( this->ifDisLable_checkBox->checkState() == Qt::Checked )
    {
        setLableProperty();
    }
    if ( this->disDiag_checkBox->checkState() == Qt::Checked )
    {
        setDiagramProperty();
    }*/

    m_mapCanvas->refresh();
}

void qgis_dev_layerPropDialog::setDiagramProperty()
{
    ////if( !mDisplayDiagramsGroupBox->isChecked() ) {m_layer->setDiagramLayerSettings( 0 ); return;}

    //QgsDiagram* diagram = 0;
    //QString diagramType = this->diagSelect_comboBox->currentText(); // 获取当前选择的图表类型
    //bool scaleAttributeValueOk = false;

    //QgsVectorDataProvider* provider = m_layer->dataProvider();
    //double maxVal = 0;
    //int fld = 2;
    //if ( fld != -1 )
    //{
    //    bool ok = false;
    //    double val = provider->maximumValue( fld ).toDouble( &ok ); // 获取该字段的最大值
    //    if ( ok )
    //    {
    //        maxVal = val;
    //    }
    //}
    //bool mValueLineEdit = false;

    //if ( diagramType == "Text" )
    //{
    //    diagram = new QgsTextDiagram();
    //}
    //else if ( diagramType == "Pie" )
    //{
    //    diagram = new QgsPieDiagram();
    //}
    //else // if ( diagramType == DIAGRAM_NAME_HISTOGRAM )
    //{
    //    diagram = new QgsHistogramDiagram();
    //}

    //#pragma region 设置diagram属性

    //QgsDiagramSettings ds; // diagram的设置项
    ////ds.font = mDiagramFont; // 设置字体
    //ds.transparency = 0; // 设置透明度

    //QList<QColor> categoryColors; // 颜色
    //QList<QString> categoryAttributes; // 属性

    //QColor color = QColor( 255, 0, 0 );
    //color.setAlpha( 255 - ds.transparency );
    //categoryColors.append( color );
    //categoryAttributes.append( "ELEV" );

    //ds.categoryColors = categoryColors;
    //ds.categoryAttributes = categoryAttributes;
    //ds.size = QSizeF( 1, 1 );
    //ds.sizeType = static_cast<QgsDiagramSettings::SizeType>( 0 );
    //ds.labelPlacementMethod = static_cast<QgsDiagramSettings::LabelPlacementMethod>( 1 );
    //ds.scaleByArea = true;


    //ds.minimumSize = 0;

    //ds.backgroundColor = QColor( 0, 0, 0 ); // 背景色
    //ds.penColor = QColor( 0, 0, 0 ); // 轮廓颜色
    //ds.penWidth = 1; // 轮廓宽度

    //ds.minScaleDenominator = -1;
    //ds.maxScaleDenominator = -1;

    //// Diagram 方向 (histogram)
    //ds.angleOffset = 1440;
    //ds.diagramOrientation = static_cast<QgsDiagramSettings::DiagramOrientation>( 0 );

    //ds.barWidth = 5; // 宽度

    //#pragma endregion 设置diagram属性


    //QgsLinearlyInterpolatedDiagramRenderer* dr = new QgsLinearlyInterpolatedDiagramRenderer();
    //dr->setLowerValue( 0.0 );
    //dr->setLowerSize( QSizeF( 0.0, 0.0 ) );
    //dr->setUpperValue( 2 );
    //dr->setUpperSize( QSizeF( 2, 2 ) );

    //bool isExpression = true;
    //dr->setClassificationAttributeIsExpression( isExpression );

    //dr->setClassificationAttributeExpression( "" );

    //dr->setDiagram( diagram );
    //dr->setDiagramSettings( ds );
    //m_layer->setDiagramRenderer( dr );

    //QgsDiagramLayerSettings dls;
    //dls.dist = 0;
    //dls.priority = 5;

    //dls.xPosColumn = -1;
    //dls.yPosColumn = -1;
    //dls.placement = static_cast<QgsDiagramLayerSettings::Placement>( 0 );

    //dls.placementFlags = 0;

    //m_layer->setDiagramLayerSettings( dls );

    //m_layer->setTitle( "" );

    //QgsVectorSimplifyMethod simplifyMethod = m_layer->simplifyMethod();
    //simplifyMethod.setSimplifyHints( QgsVectorSimplifyMethod::NoSimplification );
    //simplifyMethod.setThreshold( 1 );
    //simplifyMethod.setForceLocalOptimization( true );
    //simplifyMethod.setMaximumScale( 1 );
    //m_layer->setSimplifyMethod( simplifyMethod );

    //m_layer->triggerRepaint();
}

void qgis_dev_layerPropDialog::setGenericProperty()
{

}

void qgis_dev_layerPropDialog::setFieldProperty()
{

}

void qgis_dev_layerPropDialog::setStyleProperty()
{

}

void qgis_dev_layerPropDialog::setLableProperty()
{
    m_layer->label()->setLabelField( QgsLabel::Text, 1 );
    m_layer->label()->labelAttributes()->setColor( Qt::black );
    m_layer->label()->labelAttributes()->setFamily( "Times" );
    m_layer->enableLabels( true );
}

void qgis_dev_layerPropDialog::apply()
{

    //#pragma region diagram

    //QSettings().setValue( "/Windows/VectorLayerProperties/diagram/tab",
    //                      mDiagramPropertiesTabWidget->currentIndex() );
    //if ( !mDisplayDiagramsGroupBox->isChecked() ) {m_layer->setDiagramRenderer( 0 ); return;}

    //QgsDiagram* diagram = 0;
    //int index = diagramType_comboBox->currentIndex();
    //QString diagramType = diagramType_comboBox->itemData( index ).toString();

    //if ( 0 == mDiagramAttributesTreeWidget->topLevelItemCount() )
    //{
    //    qgis_dev::instance()->messageBar()->pushMessage(
    //        tr( "Diagrams: No attributes added." ),
    //        tr( "You did not add any attributes to this diagram layer. Please specify the attributes to visualize on the diagrams or disable diagrams." ),
    //        QgsMessageBar::WARNING );
    //}

    //bool scaleAttributeValueOk = false;
    //// Check if a (usable) scale attribute value is inserted
    //mValueLineEdit->text().toDouble( &scaleAttributeValueOk );

    //if ( !mFixedSizeCheckBox->isChecked() && !scaleAttributeValueOk )
    //{
    //    double maxVal = DBL_MIN;
    //    QgsVectorDataProvider* provider = m_layer->dataProvider();

    //    if ( provider )
    //    {
    //        if ( diagramType == DIAGRAM_NAME_HISTOGRAM )
    //        {
    //            // Find maximum value
    //            for ( int i = 0; i < mDiagramAttributesTreeWidget->topLevelItemCount(); ++i )
    //            {
    //                QString fldName = mDiagramAttributesTreeWidget->topLevelItem( i )->data( 0, Qt::UserRole ).toString();
    //                if ( fldName.count() >= 2 && fldName.at( 0 ) == '"' && fldName.at( fldName.count() - 1 ) == '"' )
    //                {
    //                    fldName = fldName.mid( 1, fldName.count() - 2 );    // remove enclosing double quotes
    //                }
    //                int fld = provider->fieldNameIndex( fldName );
    //                if ( fld != -1 )
    //                {
    //                    bool ok = false;
    //                    double val = provider->maximumValue( fld ).toDouble( &ok );
    //                    if ( ok )
    //                    {
    //                        maxVal = qMax( maxVal, val );
    //                    }
    //                }
    //            }
    //        }
    //        else
    //        {
    //            maxVal = provider->maximumValue( mSizeAttributeComboBox->itemData( mSizeAttributeComboBox->currentIndex() ).toInt() ).toDouble();
    //        }
    //    }

    //    if ( maxVal != DBL_MIN )
    //    {
    //        qgis_dev::instance()->messageBar()->pushMessage(
    //            tr( "Interpolation value" ),
    //            tr( "You did not specify an interpolation value. A default value of %1 has been set." ).arg( QString::number( maxVal ) ),
    //            QgsMessageBar::INFO,
    //            5 );

    //        mValueLineEdit->setText( QString::number( maxVal ) );
    //    }
    //}

    //if ( diagramType == DIAGRAM_NAME_TEXT )
    //{
    //    diagram = new QgsTextDiagram();
    //}
    //else if ( diagramType == DIAGRAM_NAME_PIE )
    //{
    //    diagram = new QgsPieDiagram();
    //}
    //else // if ( diagramType == DIAGRAM_NAME_HISTOGRAM )
    //{
    //    diagram = new QgsHistogramDiagram();
    //}

    //QgsDiagramSettings ds;
    //ds.font = font_comboBox->currentFont(); // 字体
    //ds.transparency = mTransparencySlider->value(); // 透明度

    //QList<QColor> categoryColors;
    //QList<QString> categoryAttributes;
    //for ( int i = 0; i < mDiagramAttributesTreeWidget->topLevelItemCount(); ++i )
    //{
    //    QColor color = mDiagramAttributesTreeWidget->topLevelItem( i )->background( 1 ).color();
    //    color.setAlpha( 255 - ds.transparency );
    //    categoryColors.append( color );
    //    categoryAttributes.append( mDiagramAttributesTreeWidget->topLevelItem( i )->data( 0, Qt::UserRole ).toString() );
    //}
    //ds.categoryColors = categoryColors;
    //ds.categoryAttributes = categoryAttributes;
    ////ds.size = QSizeF( mDiagramSizeSpinBox->value(), mDiagramSizeSpinBox->value() );
    //ds.size = QSizeF( 50, 50 );
    ////ds.sizeType = static_cast<QgsDiagramSettings::SizeType>( mDiagramUnitComboBox->itemData( mDiagramUnitComboBox->currentIndex() ).toInt() );
    //ds.sizeType = static_cast<QgsDiagramSettings::SizeType>( 0 );
    //ds.labelPlacementMethod = static_cast<QgsDiagramSettings::LabelPlacementMethod>( mLabelPlacementComboBox->itemData( mLabelPlacementComboBox->currentIndex() ).toInt() );
    ////ds.scaleByArea = mScaleDependencyComboBox->itemData( mScaleDependencyComboBox->currentIndex() ).toBool();
    //ds.scaleByArea = false;

    //if ( mIncreaseSmallDiagramsGroupBox->isChecked() )
    //{
    //    ds.minimumSize = mIncreaseMinimumSizeSpinBox->value();
    //}
    //else
    //{
    //    ds.minimumSize = 0;
    //}

    ///*ds.backgroundColor = mBackgroundColorButton->backgroundColor();
    //ds.penColor = mDiagramPenColorButton->backgroundColor();*/

    //ds.backgroundColor = QColor( 255, 0, 0 );
    //ds.penColor = QColor( 255, 255, 0 );
    //ds.penWidth = mPenWidthSpinBox->value();
    //if ( mVisibilityGroupBox->isChecked() )
    //{
    //    ds.minScaleDenominator = mMinimumDiagramScaleLineEdit->text().toDouble();
    //    ds.maxScaleDenominator = mMaximumDiagramScaleLineEdit->text().toDouble();
    //}
    //else
    //{
    //    ds.minScaleDenominator = -1;
    //    ds.maxScaleDenominator = -1;
    //}

    //// Diagram angle offset (pie)
    //ds.angleOffset = mAngleOffsetComboBox->itemData( mAngleOffsetComboBox->currentIndex() ).toInt();

    //// Diagram orientation (histogram)
    ////ds.diagramOrientation = static_cast<QgsDiagramSettings::DiagramOrientation>( mOrientationButtonGroup->checkedButton()->property( "direction" ).toInt() );
    //ds.diagramOrientation = static_cast<QgsDiagramSettings::DiagramOrientation>( 0 );
    //ds.barWidth = mBarWidthSpinBox->value();

    //if ( mFixedSizeCheckBox->isChecked() )
    //{
    //    QgsSingleCategoryDiagramRenderer* dr = new QgsSingleCategoryDiagramRenderer();
    //    dr->setDiagram( diagram );
    //    dr->setDiagramSettings( ds );
    //    m_layer->setDiagramRenderer( dr );
    //}
    //else
    //{
    //    QgsLinearlyInterpolatedDiagramRenderer* dr = new QgsLinearlyInterpolatedDiagramRenderer();
    //    dr->setLowerValue( 0.0 );
    //    dr->setLowerSize( QSizeF( 0.0, 0.0 ) );
    //    dr->setUpperValue( mValueLineEdit->text().toDouble() );
    //    dr->setUpperSize( QSizeF( mSizeSpinBox->value(), mSizeSpinBox->value() ) );
    //    bool isExpression = mSizeAttributeComboBox->currentIndex() >= m_availableAttributeCount;
    //    dr->setClassificationAttributeIsExpression( isExpression );
    //    if ( isExpression )
    //    {
    //        dr->setClassificationAttributeExpression( mSizeAttributeComboBox->currentText() );
    //    }
    //    else
    //    {
    //        dr->setClassificationAttribute( mSizeAttributeComboBox->itemData( mSizeAttributeComboBox->currentIndex() ).toInt() );
    //    }
    //    dr->setDiagram( diagram );
    //    dr->setDiagramSettings( ds );
    //    m_layer->setDiagramRenderer( dr );
    //}

    //QgsDiagramLayerSettings dls;
    //dls.dist = mDiagramDistanceSpinBox->value();
    //dls.priority = mPrioritySlider->value();
    //if ( mDataDefinedPositionGroupBox->isChecked() )
    //{
    //    //dls.xPosColumn = mDataDefinedXComboBox->itemData( mDataDefinedXComboBox->currentIndex() ).toInt();
    //    //dls.yPosColumn = mDataDefinedYComboBox->itemData( mDataDefinedYComboBox->currentIndex() ).toInt();
    //    dls.xPosColumn = -1;
    //    dls.yPosColumn = -1;
    //}
    //else
    //{
    //    dls.xPosColumn = -1;
    //    dls.yPosColumn = -1;
    //}
    //dls.placement = ( QgsDiagramLayerSettings::Placement )mPlacementComboBox->itemData( mPlacementComboBox->currentIndex() ).toInt();
    //if ( mLineOptionsComboBox->isEnabled() )
    //{
    //    dls.placementFlags = static_cast<QgsDiagramLayerSettings::LinePlacementFlags>( mLineOptionsComboBox->itemData( mLineOptionsComboBox->currentIndex() ).toInt() );
    //}
    //m_layer->setDiagramLayerSettings( dls );
    //#pragma endregion diagram

    labelingGui->apply();

    m_mapCanvas->refresh();

}

void qgis_dev_layerPropDialog::on_mTransparencySlider_valueChanged( int value )
{
    mTransparencyLabel->setText( tr( "Transparency: %1%" ).arg( value * 100 / 255 ) );
}

void qgis_dev_layerPropDialog::on_mAddCategoryPushButton_clicked()
{
    foreach ( QTreeWidgetItem *attributeItem, mAttributesTreeWidget->selectedItems() )
    {
        addAttribute( attributeItem );
    }
}

void qgis_dev_layerPropDialog::on_mRemoveCategoryPushButton_clicked()
{
    foreach ( QTreeWidgetItem *attributeItem, mDiagramAttributesTreeWidget->selectedItems() )
    {
        delete attributeItem;
    }
}

void qgis_dev_layerPropDialog::on_mAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
    Q_UNUSED( column );
    addAttribute( item );
}

void qgis_dev_layerPropDialog::on_mFindMaximumValueButton_clicked()
{
    if ( !m_layer ) { return; }

    float maxValue = 0.0;
    if ( mSizeAttributeComboBox->currentIndex() >= m_availableAttributeCount )
    {
        QgsExpression exp( mSizeAttributeComboBox->currentText() );
        exp.prepare( m_layer->pendingFields() );
        if ( !exp.hasEvalError() )
        {
            QgsFeature feature;
            QgsFeatureIterator features = m_layer->getFeatures();
            while ( features.nextFeature( *&feature ) )
            {
                maxValue = qMax( maxValue, exp.evaluate( &feature ).toFloat() );
            }
        }
        else
        {
            QgsDebugMsgLevel( "Prepare error:" + exp.evalErrorString(), 4 );
        }
    }
    else
    {
        maxValue = m_layer->maximumValue( mSizeAttributeComboBox->itemData( mSizeAttributeComboBox->currentIndex() ).toInt() ).toFloat();
    }
    mValueLineEdit->setText( QString( "%1" ).arg( maxValue ) );
}

void qgis_dev_layerPropDialog::on_mDisplayDiagramsGroupBox_toggled( bool checked )
{
    if ( checked )
    {
        diagramChanged( diagramType_comboBox->currentIndex() );
        // Update enabled/disabled state
    }
}

void qgis_dev_layerPropDialog::on_mDiagramAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
    if ( column == 1 )
    {
        QColor newColor = QColorDialog::getColor( item->background( 1 ).color(), this, "Select Color", QColorDialog::DontUseNativeDialog );
        if( newColor.isValid() )
        {
            item->setBackground( 1, QBrush( newColor ) );
        }
    }

}

void qgis_dev_layerPropDialog::on_mEngineSettingsButton_clicked()
{

}

void qgis_dev_layerPropDialog::showSizeAttributeExpressionDialog()
{

}

void qgis_dev_layerPropDialog::showAddAttributeExpressionDialog()
{

}

void qgis_dev_layerPropDialog::on_mBackgroundColorButton_clicked()
{
    QColor color;
    color = QColorDialog::getColor( Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog );
    if ( color.isValid() )
    {
        mBackgroundColorButton->setText( color.name() );
        mBackgroundColorButton->setAutoFillBackground( true );
        mBackgroundColorButton->setStyleSheet( "background-color:" + color.name() + ";" );
    }
}

void qgis_dev_layerPropDialog::on_mDiagramPenColorButton_clicked()
{
    QColor color;
    color = QColorDialog::getColor( Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog );
    if ( color.isValid() )
    {
        mDiagramPenColorButton->setText( color.name() );
        mDiagramPenColorButton->setAutoFillBackground( true );
        mDiagramPenColorButton->setStyleSheet( "background-color:" + color.name() + ";" );
    }
}

void qgis_dev_layerPropDialog::addAttribute( QTreeWidgetItem * item )
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem( mDiagramAttributesTreeWidget );

    newItem->setText( 0, item->text( 0 ) );
    newItem->setData( 0, Qt::UserRole, item->data( 0, Qt::UserRole ) );
    newItem->setFlags( newItem->flags() & ~Qt::ItemIsDropEnabled );

    //set initial color for diagram category
    int red = 1 + ( int )( 255.0 * qrand() / ( RAND_MAX + 1.0 ) );
    int green = 1 + ( int )( 255.0 * qrand() / ( RAND_MAX + 1.0 ) );
    int blue = 1 + ( int )( 255.0 * qrand() / ( RAND_MAX + 1.0 ) );
    QColor randomColor( red, green, blue );
    newItem->setBackground( 1, QBrush( randomColor ) );
    mDiagramAttributesTreeWidget->addTopLevelItem( newItem );
}
