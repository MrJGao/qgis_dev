#include "qgis_devattrtabledialog.h"
#include "qgis_dev.h"

#include <QDockWidget>
#include <QMessageBox>

#include <qgsmapcanvas.h>
#include <qgsattributetableview.h>
#include <qgsattributetablemodel.h>
#include <qgsdistancearea.h>
#include <qgsattributeeditorcontext.h>
#include <qgsvectordataprovider.h>

#include <qgsfeaturelistview.h>
#include <qgsattributetableview.h>
#include <qgsattributetablemodel.h>
#include <qgsfeaturelistmodel.h>
#include <qgsvectorlayercache.h>

#include <qgsexpressionbuilderdialog.h>
#include <qgsexpressionselectiondialog.h>

#include <qgscontexthelp.h>
#include <qgsproject.h>


//! 这里定义一个DockWidget，将属性表窗口装进去， 实现窗口停靠
class qgis_devAttributeTableDock : public QDockWidget
{
public:
    qgis_devAttributeTableDock( const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 )
        : QDockWidget( title, parent, flags )
    {
        setObjectName( "AttributeTable" ); // set object name so the position can be saved
    }

    virtual void closeEvent( QCloseEvent* ev ) override
    {
        Q_UNUSED( ev );
        deleteLater();
    }
};

qgis_devattrtableDialog::qgis_devattrtableDialog( QgsVectorLayer* theVecLayer, QWidget *parent, Qt::WindowFlags flags )
    : QDialog( parent, flags )
    , mLayer( theVecLayer )
    , mDock( 0 )
    , mRubberBand( 0 )
{
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );

    // DistanceArea 初始化
    myDa = new QgsDistanceArea;
    myDa->setSourceCrs( mLayer->crs() );
    myDa->setEllipsoidalMode( qgis_dev::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
    //myDa->setEllipsoid( qgis_dev::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );

    // 属性编辑器右键菜单
    QgsAttributeEditorContext context;
    context.setDistanceArea( *myDa );
    //context.setVectorLayerTools( qgis_dev::instance()->vectorLayerTools );

    // 属性feature请求
    QgsFeatureRequest r;
    if ( mLayer->geometryType() != QGis::NoGeometry
            //&& settings.value( "/qgis/attributeTableBehaviour", QgsAttributeTableFilterModel::ShowAll ).toInt() == QgsAttributeTableFilterModel::ShowVisible
       )
    {
        QgsMapCanvas *mc = qgis_dev::instance()->mapCanvas();
        QgsRectangle extent( mc->mapSettings().mapToLayerCoordinates( theVecLayer, mc->extent() ) );
        r.setFilterRect( extent );

        QgsGeometry *g = QgsGeometry::fromRect( extent );
        mRubberBand = new QgsRubberBand( mc, true );
        mRubberBand->setToGeometry( g, theVecLayer );
        delete g;

        mActionShowAllFilter->setText( tr( "Show All Features In Initial Canvas Extent" ) );
    }

    // 初始化DualView
    mMainView->init( mLayer, qgis_dev::instance()->mapCanvas(), r, context );


    // 初始化界面filter的元素
    mFilterActionMapper = new QSignalMapper( this );
    mFilterColumnsMenu = new QMenu( this );
    mActionFilterColumnsMenu->setMenu( mFilterColumnsMenu );
    mApplyFilterButton->setDefaultAction( mActionApplyFilter );

    // 设置filter的显示图标
    QIcon filterIcon = qgis_dev::getThemeIcon( "mActionFilter.svg" );
    mActionShowAllFilter->setIcon( filterIcon );
    mActionAdvancedFilter->setIcon( filterIcon );
    mActionSelectedFilter->setIcon( filterIcon );
    mActionVisibleFilter->setIcon( filterIcon );
    mActionEditedFilter->setIcon( filterIcon );

    // connections
    connect( mActionAdvancedFilter, SIGNAL( triggered() ), SLOT( filterExpressionBuilder() ) );
    connect( mActionShowAllFilter, SIGNAL( triggered() ), SLOT( filterShowAll() ) );
    connect( mActionSelectedFilter, SIGNAL( triggered() ), SLOT( filterSelected() ) );
    connect( mActionVisibleFilter, SIGNAL( triggered() ), SLOT( filterVisible() ) );
    connect( mActionEditedFilter, SIGNAL( triggered() ), SLOT( filterEdited() ) );
    connect( mFilterActionMapper, SIGNAL( mapped( QObject* ) ), SLOT( filterColumnChanged( QObject* ) ) );
    connect( mFilterQuery, SIGNAL( returnPressed() ), SLOT( filterQueryAccepted() ) );
    connect( mActionApplyFilter, SIGNAL( triggered() ), SLOT( filterQueryAccepted() ) );

    // 将图层和属性表的动作关联起来
    connect( mLayer, SIGNAL( editingStarted() ), this, SLOT( editingToggled() ) );
    connect( mLayer, SIGNAL( editingStopped() ), this, SLOT( editingToggled() ) );
    connect( mLayer, SIGNAL( layerDeleted() ), this, SLOT( close() ) );
    connect( mLayer, SIGNAL( selectionChanged() ), this, SLOT( updateTitle() ) );
    connect( mLayer, SIGNAL( attributeAdded( int ) ), this, SLOT( columnBoxInit() ) );
    connect( mLayer, SIGNAL( attributeDeleted( int ) ), this, SLOT( columnBoxInit() ) );

    // 关联DualView和对话框窗口
    connect( mMainView, SIGNAL( filterChanged() ), this, SLOT( updateTitle() ) );

    // info from table to application
    //connect( this, SIGNAL( saveEdits( QgsMapLayer * ) ), qgis_dev::instance(), SLOT( saveEdits( QgsMapLayer * ) ) );
    //

    // 将组织好的属性表装入到DockWidget当中
    mDock = new qgis_devAttributeTableDock( tr( "Attribute table - %1 (%n Feature(s))",
                                            "feature count", mMainView->featureCount() ) );
    mDock->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea );
    mDock->setWidget( this );
    connect( this, SIGNAL( destroyed() ), mDock, SLOT( close ) );
    mDock->show();
    //qgis_dev::instance()->addDockWidget( Qt::BottomDockWidgetArea, mDock );

    columnBoxInit();
    updateTitle();

    // 下面就是设置所有Action的显示Icon的操作
    mRemoveSelectionButton->setIcon( qgis_dev::getThemeIcon( "/mActionUnselectAttributes.png" ) );
    mSelectedToTopButton->setIcon( qgis_dev::getThemeIcon( "/mActionSelectedToTop.png" ) );
    mCopySelectedRowsButton->setIcon( qgis_dev::getThemeIcon( "/mActionCopySelected.png" ) );
    mZoomMapToSelectedRowsButton->setIcon( qgis_dev::getThemeIcon( "/mActionZoomToSelected.svg" ) );
    mPanMapToSelectedRowsButton->setIcon( qgis_dev::getThemeIcon( "/mActionPanToSelected.svg" ) );
    mInvertSelectionButton->setIcon( qgis_dev::getThemeIcon( "/mActionInvertSelection.png" ) );
    mToggleEditingButton->setIcon( qgis_dev::getThemeIcon( "/mActionToggleEditing.svg" ) );
    mSaveEditsButton->setIcon( qgis_dev::getThemeIcon( "/mActionSaveEdits.svg" ) );
    mDeleteSelectedButton->setIcon( qgis_dev::getThemeIcon( "/mActionDeleteSelected.svg" ) );
    mOpenFieldCalculator->setIcon( qgis_dev::getThemeIcon( "/mActionCalculateField.png" ) );
    mAddAttribute->setIcon( qgis_dev::getThemeIcon( "/mActionNewAttribute.png" ) );
    mRemoveAttribute->setIcon( qgis_dev::getThemeIcon( "/mActionDeleteAttribute.png" ) );
    mTableViewButton->setIcon( qgis_dev::getThemeIcon( "/mActionOpenTable.png" ) );
    mAttributeViewButton->setIcon( qgis_dev::getThemeIcon( "/mActionPropertyItem.png" ) );
    mExpressionSelectButton->setIcon( qgis_dev::getThemeIcon( "/mIconExpressionSelect.svg" ) );
    mAddFeature->setIcon( qgis_dev::getThemeIcon( "/mActionNewTableRow.png" ) );

    // Toggle Editing
    bool canChangeAttributes = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
    bool canDeleteFeatures = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures;
    bool canAddAttributes = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::AddAttributes;
    bool canDeleteAttributes = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteAttributes;
    bool canAddFeatures = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::AddFeatures;

    mToggleEditingButton->blockSignals( true );
    mToggleEditingButton->setCheckable( true );
    mToggleEditingButton->setChecked( mLayer->isEditable() );
    mToggleEditingButton->setEnabled( ( canChangeAttributes || canDeleteFeatures || canAddAttributes || canDeleteAttributes || canAddFeatures ) && !mLayer->isReadOnly() );
    mToggleEditingButton->blockSignals( false );

    mSaveEditsButton->setEnabled( mToggleEditingButton->isEnabled() && mLayer->isEditable() );
    mAddAttribute->setEnabled( ( canChangeAttributes || canAddAttributes ) && mLayer->isEditable() );
    mDeleteSelectedButton->setEnabled( canDeleteFeatures && mLayer->isEditable() );
    mAddFeature->setEnabled( canAddFeatures && mLayer->isEditable() && mLayer->geometryType() == QGis::NoGeometry );
    mAddFeature->setHidden( !canAddFeatures || mLayer->geometryType() != QGis::NoGeometry );


    mMainViewButtonGroup->setId( mTableViewButton, QgsDualView::AttributeTable );
    mMainViewButtonGroup->setId( mAttributeViewButton, QgsDualView::AttributeEditor );

    // 默认显示图层的全部属性
    filterShowAll();

    mFieldModel = new QgsFieldModel;
    mFieldModel->setLayer( theVecLayer );
    mFieldCombo->setModel( mFieldModel );
    connect( mRunFieldCalc, SIGNAL( clicked() ), SLOT( updateFieldFromExpression() ) );
    connect( mRunFieldCalcSelected, SIGNAL( clicked() ), SLOT( updateFieldFromExpressionSelected() ) );
    connect( mUpdateExpressionText, SIGNAL( fieldChanged( QString, bool ) ), SLOT( updateButtonStatus( QString, bool ) ) );
    mUpdateExpressionText->setLayer( mLayer );
    mUpdateExpressionText->setLeftHandButtonStyle( true );
    editingToggled();
}

qgis_devattrtableDialog::~qgis_devattrtableDialog()
{
    delete myDa;
    delete mRubberBand;
}

void qgis_devattrtableDialog::filterExpressionBuilder()
{
    // Show expression builder
    QgsExpressionBuilderDialog dlg( mLayer, mFilterQuery->text(), this );
    dlg.setWindowTitle( tr( "Expression based filter" ) );

    QgsDistanceArea myDa;
    myDa.setSourceCrs( mLayer->crs().srsid() );
    myDa.setEllipsoidalMode( qgis_dev::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
    //myDa.setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );
    dlg.setGeomCalculator( myDa );

    if ( dlg.exec() == QDialog::Accepted )
    {
        setFilterExpression( dlg.expressionText() );
    }
}

void qgis_devattrtableDialog::filterShowAll()
{
    mFilterButton->setDefaultAction( mActionShowAllFilter );
    mFilterButton->setPopupMode( QToolButton::InstantPopup );
    mCbxCaseSensitive->setVisible( false );
    mFilterQuery->setVisible( false );
    mApplyFilterButton->setVisible( false );
    mMainView->setFilterMode( QgsAttributeTableFilterModel::ShowAll );
    updateTitle();
}

void qgis_devattrtableDialog::filterSelected()
{
    mFilterButton->setDefaultAction( mActionSelectedFilter );
    mFilterButton->setPopupMode( QToolButton::InstantPopup );
    mCbxCaseSensitive->setVisible( false );
    mFilterQuery->setVisible( false );
    mApplyFilterButton->setVisible( false );
    mMainView->setFilterMode( QgsAttributeTableFilterModel::ShowSelected );
}

void qgis_devattrtableDialog::filterVisible()
{
    if ( !mLayer->hasGeometryType() )
    {
        filterShowAll();
        return;
    }

    mFilterButton->setDefaultAction( mActionVisibleFilter );
    mFilterButton->setPopupMode( QToolButton::InstantPopup );
    mCbxCaseSensitive->setVisible( false );
    mFilterQuery->setVisible( false );
    mApplyFilterButton->setVisible( false );
    mMainView->setFilterMode( QgsAttributeTableFilterModel::ShowVisible );
}

void qgis_devattrtableDialog::filterEdited()
{
    mFilterButton->setDefaultAction( mActionEditedFilter );
    mFilterButton->setPopupMode( QToolButton::InstantPopup );
    mCbxCaseSensitive->setVisible( false );
    mFilterQuery->setVisible( false );
    mApplyFilterButton->setVisible( false );
    mMainView->setFilterMode( QgsAttributeTableFilterModel::ShowEdited );
}

void qgis_devattrtableDialog::filterQueryAccepted()
{
    if ( mFilterQuery->text().isEmpty() )
    {
        filterShowAll();
        return;
    }
    filterQueryChanged( mFilterQuery->text() );
}

void qgis_devattrtableDialog::filterQueryChanged( const QString& query )
{
    QString str;
    if ( mFilterButton->defaultAction() == mActionAdvancedFilter )
    {
        str = query;
    }
    else
    {
        QString fieldName = mFilterButton->defaultAction()->text();

        const QgsFields& flds = mLayer->pendingFields();
        int fldIndex = mLayer->fieldNameIndex( fieldName );
        if ( fldIndex < 0 )
        {
            return;
        }

        // 判断属性字段是否为数字
        QVariant::Type fldType = flds[fldIndex].type();
        bool numeric = ( fldType == QVariant::Int || fldType == QVariant::Double || fldType == QVariant::LongLong );

        // 如果属性是字符串，判断应该用“ILIKE”或者“LIKE”
        QString sensString = "ILIKE";
        if ( mCbxCaseSensitive->isChecked() )
        {
            sensString = "LIKE";
        }

        QSettings settings;
        QString nullValue = settings.value( "qgis/nullValue", "NULL" ).toString();

        if ( mFilterQuery->displayText() == nullValue )
        {
            str = QString( "%1 IS NULL" ).arg( QgsExpression::quotedColumnRef( fieldName ) );
        }
        else
        {
            str = QString( "%1 %2 '%3'" )
                  .arg( QgsExpression::quotedColumnRef( fieldName ) )
                  .arg( numeric ? "=" : sensString )
                  .arg( numeric
                        ? mFilterQuery->displayText().replace( "'", "''" )
                        :
                        "%" + mFilterQuery->displayText().replace( "'", "''" ) + "%" ); // escape quotes
        }
    }

    // 以上为解析字符串， str才是最后的查询字符串
    setFilterExpression( str );
    updateTitle(); // 更新属性窗口标题
}

void qgis_devattrtableDialog::filterColumnChanged( QObject* filterAction )
{
    mFilterButton->setDefaultAction( qobject_cast<QAction *>( filterAction ) );
    mFilterButton->setPopupMode( QToolButton::InstantPopup );
    mCbxCaseSensitive->setVisible( true );
    mFilterQuery->setVisible( true );
    mApplyFilterButton->setVisible( true );
}

void qgis_devattrtableDialog::editingToggled()
{
    mToggleEditingButton->blockSignals( true );
    mToggleEditingButton->setChecked( mLayer->isEditable() );
    mSaveEditsButton->setEnabled( mLayer->isEditable() );
    mToggleEditingButton->blockSignals( false );

    bool canChangeAttributes = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
    bool canDeleteFeatures = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures;
    bool canAddAttributes = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::AddAttributes;
    bool canAddFeatures = mLayer->dataProvider()->capabilities() & QgsVectorDataProvider::AddFeatures;
    mAddAttribute->setEnabled( ( canChangeAttributes || canAddAttributes ) && mLayer->isEditable() );
    mDeleteSelectedButton->setEnabled( canDeleteFeatures && mLayer->isEditable() );
    mAddFeature->setEnabled( canAddFeatures && mLayer->isEditable() && mLayer->geometryType() == QGis::NoGeometry );

    mUpdateExpressionBox->setVisible( mLayer->isEditable() );
    // not necessary to set table read only if layer is not editable
    // because model always reflects actual state when returning item flags
}

void qgis_devattrtableDialog::updateTitle()
{
    QWidget* w = mDock ? qobject_cast<QWidget*>( mDock ) : qobject_cast<QWidget*>( this );
    w->setWindowTitle( tr( "Attribute table - %1 :: Features total: %2, filtered: %3, selected: %4%5" )
                       .arg( mLayer->name() )
                       .arg( mMainView->featureCount() )
                       .arg( mMainView->filteredFeatureCount() )
                       .arg( mLayer->selectedFeatureCount() )
                       .arg( mRubberBand ? tr( ", spatially limited" ) : "" ) );
    if ( mMainView->filterMode() == QgsAttributeTableFilterModel::ShowAll )
    {
        mRunFieldCalc->setText( tr( "Update All" ) );
    }
    else
    {
        mRunFieldCalc->setText( tr( "Update Filtered" ) );
    }

    bool enabled = mLayer->selectedFeatureCount() > 0;
    mRunFieldCalcSelected->setEnabled( enabled );
}

void qgis_devattrtableDialog::columnBoxInit()
{
    foreach ( QAction* a, mFilterColumnsMenu->actions() )
    {
        mFilterColumnsMenu->removeAction( a );
        mFilterActionMapper->removeMappings( a );
        mFilterButton->removeAction( a );
        delete a;
    }
    mFilterButton->addAction( mActionShowAllFilter );
    mFilterButton->addAction( mActionSelectedFilter );
    if ( mLayer->hasGeometryType() )
    {
        mFilterButton->addAction( mActionVisibleFilter );
    }
    mFilterButton->addAction( mActionEditedFilter );
    mFilterButton->addAction( mActionFilterColumnsMenu );
    mFilterButton->addAction( mActionAdvancedFilter );

    QList<QgsField> fields = mLayer->pendingFields().toList();

    foreach ( const QgsField field, fields )
    {
        int idx = mLayer->fieldNameIndex( field.name() );
        if ( idx < 0 )
        {
            continue;
        }

        if ( mLayer->editorWidgetV2( idx ) != "Hidden" )
        {
            QIcon icon = qgis_dev::getThemeIcon( "/mActionNewAttribute.png" );
            QString text = field.name();

            // Generate action for the filter popup button
            QAction* filterAction = new QAction( icon, text, mFilterButton );
            mFilterActionMapper->setMapping( filterAction, filterAction );
            connect( filterAction, SIGNAL( triggered() ), mFilterActionMapper, SLOT( map() ) );
            mFilterColumnsMenu->addAction( filterAction );
        }
    }
}

void qgis_devattrtableDialog::runFieldCalculation( QgsVectorLayer* layer, QString fieldName, QString expression, QgsFeatureIds filteredIds /*= QgsFeatureIds() */ )
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    mLayer->beginEditCommand( "Field calculator" );

    QModelIndex modelindex = mFieldModel->indexFromName( fieldName );
    int fieldindex = modelindex.data( QgsFieldModel::FieldIndexRole ).toInt();

    bool calculationSuccess = true;
    QString error;

    QgsExpression exp( expression );
    exp.setGeomCalculator( *myDa );
    bool useGeometry = exp.needsGeometry();

    QgsFeatureRequest request( mMainView->masterModel()->request() );
    useGeometry |= request.filterType() == QgsFeatureRequest::FilterRect;
    request.setFlags( useGeometry ? QgsFeatureRequest::NoFlags : QgsFeatureRequest::NoGeometry );

    int rownum = 1;

    const QgsField &fld = layer->pendingFields()[ fieldindex ];

    //go through all the features and change the new attributes
    QgsFeatureIterator fit = layer->getFeatures( request );
    QgsFeature feature;
    while ( fit.nextFeature( feature ) )
    {
        if ( !filteredIds.isEmpty() && !filteredIds.contains( feature.id() ) )
        {
            continue;
        }

        exp.setCurrentRowNumber( rownum );
        QVariant value = exp.evaluate( &feature );
        fld.convertCompatible( value );
        // Bail if we have a update error
        if ( exp.hasEvalError() )
        {
            calculationSuccess = false;
            error = exp.evalErrorString();
            break;
        }
        else
        {
            QVariant oldvalue = feature.attributes().value( fieldindex );
            mLayer->changeAttributeValue( feature.id(), fieldindex, value, oldvalue );
        }

        rownum++;
    }

    QApplication::restoreOverrideCursor();

    if ( !calculationSuccess )
    {
        QMessageBox::critical( 0, tr( "Error" ), tr( "An error occured while evaluating the calculation string:\n%1" ).arg( error ) );
        mLayer->destroyEditCommand();
        return;
    }

    mLayer->endEditCommand();
}

void qgis_devattrtableDialog::updateFieldFromExpression()
{
    bool filtered = mMainView->filterMode() != QgsAttributeTableFilterModel::ShowAll;
    QgsFeatureIds filteredIds = filtered ? mMainView->filteredFeatures() : QgsFeatureIds();
    runFieldCalculation( mLayer, mFieldCombo->currentText(), mUpdateExpressionText->currentField(), filteredIds );
}

void qgis_devattrtableDialog::updateFieldFromExpressionSelected()
{
    QgsFeatureIds filteredIds = mLayer->selectedFeaturesIds();
    runFieldCalculation( mLayer, mFieldCombo->currentText(), mUpdateExpressionText->currentField(), filteredIds );
}

void qgis_devattrtableDialog::updateButtonStatus( QString fieldName, bool isValid )
{
    Q_UNUSED( fieldName );
    mRunFieldCalc->setEnabled( isValid );
}

void qgis_devattrtableDialog::setFilterExpression( QString filterString )
{
    mFilterQuery->setText( filterString );
    mFilterButton->setDefaultAction( mActionAdvancedFilter );
    mFilterButton->setPopupMode( QToolButton::MenuButtonPopup );
    mCbxCaseSensitive->setVisible( false );
    mFilterQuery->setVisible( true );
    mApplyFilterButton->setVisible( true );
    mMainView->setFilterMode( QgsAttributeTableFilterModel::ShowFilteredList );

    QgsFeatureIds filteredFeatures;
    QgsDistanceArea myDa;

    myDa.setSourceCrs( mLayer->crs().srsid() );
    myDa.setEllipsoidalMode( qgis_dev::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
    myDa.setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );

    // parse search string and build parsed tree
    QgsExpression filterExpression( filterString );
    if ( filterExpression.hasParserError() )
    {
        qgis_dev::instance()->messageBar()->pushMessage( tr( "Parsing error" ), filterExpression.parserErrorString(), QgsMessageBar::WARNING, qgis_dev::instance()->messageTimeout() );
        return;
    }

    if ( ! filterExpression.prepare( mLayer->pendingFields() ) )
    {
        qgis_dev::instance()->messageBar()->pushMessage( tr( "Evaluation error" ), filterExpression.evalErrorString(), QgsMessageBar::WARNING, qgis_dev::instance()->messageTimeout() );
    }

    bool fetchGeom = filterExpression.needsGeometry();

    QApplication::setOverrideCursor( Qt::WaitCursor );

    filterExpression.setGeomCalculator( myDa );
    QgsFeatureRequest request( mMainView->masterModel()->request() );
    request.setSubsetOfAttributes( filterExpression.referencedColumns(), mLayer->pendingFields() );
    if ( !fetchGeom )
    {
        request.setFlags( QgsFeatureRequest::NoGeometry );
    }
    QgsFeatureIterator featIt = mLayer->getFeatures( request );

    QgsFeature f;

    while ( featIt.nextFeature( f ) )
    {
        if ( filterExpression.evaluate( &f ).toInt() != 0 )
        {
            filteredFeatures << f.id();
        }

        // check if there were errors during evaluating
        if ( filterExpression.hasEvalError() )
        {
            break;
        }
    }

    featIt.close();

    mMainView->setFilteredFeatures( filteredFeatures );

    QApplication::restoreOverrideCursor();

    if ( filterExpression.hasEvalError() )
    {
        qgis_dev::instance()->messageBar()->pushMessage( tr( "Error filtering" ), filterExpression.evalErrorString(), QgsMessageBar::WARNING, qgis_dev::instance()->messageTimeout() );
        return;
    }
}

void qgis_devattrtableDialog::on_mExpressionSelectButton_clicked()
{
    QgsExpressionSelectionDialog* dlg = new QgsExpressionSelectionDialog( mLayer );
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->show();
}

void qgis_devattrtableDialog::on_mCopySelectedRowsButton_clicked()
{

}

void qgis_devattrtableDialog::on_mToggleEditingButton_toggled()
{

}

void qgis_devattrtableDialog::on_mSaveEditsButton_clicked()
{

}

void qgis_devattrtableDialog::on_mInvertSelectionButton_clicked()
{
    mLayer->invertSelection();
}

void qgis_devattrtableDialog::on_mRemoveSelectionButton_clicked()
{
    mLayer->removeSelection();
}

void qgis_devattrtableDialog::on_mZoomMapToSelectedRowsButton_clicked()
{
    qgis_dev::instance()->mapCanvas()->zoomToSelected( mLayer );
}

void qgis_devattrtableDialog::on_mPanMapToSelectedRowsButton_clicked()
{
    qgis_dev::instance()->mapCanvas()->panToSelected( mLayer );
}

void qgis_devattrtableDialog::on_mSelectedToTopButton_toggled()
{
    if ( mSelectedToTopButton->isChecked() )
    {
        mMainView->setSelectedOnTop( true );
    }
    else
    {
        mMainView->setSelectedOnTop( false );
    }

}

void qgis_devattrtableDialog::on_mAddAttribute_clicked()
{
    if ( !mLayer ) {return;}

    // 先获取 model
    QgsAttributeTableModel* masterModel = mMainView->masterModel();
    //QgsAddAttrDialog dialog( mLayer, this );
    //if ( dialog.exec() == QDialog::Accepted )
    //{
    //    mLayer->beginEditCommand( tr( "Attribute added" ) );
    //    if ( mLayer->addAttribute( dialog.field() ) )
    //    {
    //        mLayer->endEditCommand();
    //    }
    //    else
    //    {
    //        mLayer->destroyEditCommand();
    //        QMessageBox::critical( this,
    //                               tr( "Failed to add field" ),
    //                               tr( "Failed to add field '%1' of type '%2'. Is the field name unique?" ).arg( dialog.field().name() ).arg( dialog.field().typeName() ) );
    //    }
    //    // 更新model
    //    masterModel->reload( masterModel->index( 0, 0 ), masterModel->index( masterModel->rowCount() - 1, masterModel->columnCount() - 1 ) );
    //    columnBoxInit();
    //}
}

void qgis_devattrtableDialog::on_mRemoveAttribute_clicked()
{

}

void qgis_devattrtableDialog::on_mOpenFieldCalculator_clicked()
{

}

void qgis_devattrtableDialog::on_mDeleteSelectedButton_clicked()
{

}

void qgis_devattrtableDialog::on_mMainView_currentChanged( int )
{

}

void qgis_devattrtableDialog::on_mAddFeature_clicked()
{

}

void qgis_devattrtableDialog::on_mHelpButton_clicked()
{
    QgsContextHelp::run( metaObject()->className() );
}
