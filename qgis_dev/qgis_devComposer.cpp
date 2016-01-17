#include "qgis_devComposer.h"
#include "qgis_dev.h"

#include <QPalette>
#include <QGridLayout>

qgis_devComposer::qgis_devComposer( QWidget *parent )
    : QMainWindow( parent ),
      m_title( "" )
{
    setupUi( this );

    //! 初始化两个标尺
    m_horizontalRuler = new QgsComposerRuler( QgsComposerRuler::Horizontal );
    m_verticalRuler = new QgsComposerRuler( QgsComposerRuler::Vertical );

    m_rulerLayoutFix = new QWidget();
    m_rulerLayoutFix->setAttribute( Qt::WA_NoMousePropagation );
    m_rulerLayoutFix->setBackgroundRole( QPalette::Window );
    m_rulerLayoutFix->setFixedSize( m_verticalRuler->rulerSize(), m_horizontalRuler->rulerSize() );

    //! 初始化composerView
    m_composerView = new QgsComposerView( this );
    m_composerView->setHorizontalRuler( m_horizontalRuler );
    m_composerView->setVerticalRuler( m_verticalRuler );
    m_composerView->setContentsMargins( 0, 0, 0, 0 );
    m_composerView->setFocusPolicy( Qt::ClickFocus );

    //! Composition
    m_composition = new QgsComposition( qgis_dev::instance()->mapCanvas()->mapSettings() );
    actionUndo->setEnabled( false );
    actionRedo->setEnabled( false );
    if ( m_composition->undoStack() )
    {
        connect( m_composition->undoStack(), SIGNAL( canUndoChanged( bool ) ), actionUndo, SLOT( setEnabled( bool ) ) );
        connect( m_composition->undoStack(), SIGNAL( canRedoChanged( bool ) ), actionRedo, SLOT( setEnabled( bool ) ) );
    }

    m_composerView->setComposition( m_composition );

    //! 布局
    m_viewLayout = new QGridLayout;
    m_viewLayout->setSpacing( 0 );
    m_viewLayout->setMargin( 0 );

    m_viewLayout->addWidget( m_rulerLayoutFix, 0, 0 );
    m_viewLayout->addWidget( m_horizontalRuler, 0, 1 );
    m_viewLayout->addWidget( m_verticalRuler, 1, 0 );
    m_viewLayout->addWidget( m_composerView, 1, 1 );

    mainWidget = new QWidget( this );
    mainWidget->setLayout( m_viewLayout );
    this->setCentralWidget( mainWidget );



    // connections
    connect( actionShow_Rulers, SIGNAL( triggered( bool ) ), this, SLOT( toggleRulers( bool ) ) );
    connect( m_composerView, SIGNAL( compositionSet( QgsComposition* ) ), this, SLOT( setComposition( QgsComposition* ) ) );


    actionShow_Rulers->setChecked( true ); //! 默认显示标尺

}


qgis_devComposer::~qgis_devComposer()
{
}

QgsMapCanvas* qgis_devComposer::mapCanvas()
{
    return qgis_dev::instance()->mapCanvas();
}

void qgis_devComposer::moveEvent( QMoveEvent* )
{

}

void qgis_devComposer::toggleRulers( bool showRuler )
{
    m_horizontalRuler->setVisible( showRuler );
    m_verticalRuler->setVisible( showRuler );
    m_rulerLayoutFix->setVisible( showRuler );
}

void qgis_devComposer::setComposition( QgsComposition* composition )
{
    if ( !composition ) { return; }

    m_composition = composition;
}

void qgis_devComposer::on_actionZoomActual_triggered()
{
    m_composerView->setZoomLevel( 1.0 );
}

void qgis_devComposer::on_actionZoomIn_triggered()
{
    m_composerView->scale( 2, 2 );
    m_composerView->updateRulers();
    m_composerView->update();
    emit zoomLevelChanged();
}

void qgis_devComposer::on_actionZoomOut_triggered()
{
    m_composerView->scale( .5, .5 );
    m_composerView->updateRulers();
    m_composerView->update();
    emit zoomLevelChanged();
}

void qgis_devComposer::on_actionZoomAll_triggered()
{
    if ( m_composerView )
    {
        m_composerView->fitInView( m_composition->sceneRect(), Qt::KeepAspectRatio );
    }
}

void qgis_devComposer::on_actionShowGrid_triggered( bool checked )
{
    m_composition->setGridVisible( checked );
}

void qgis_devComposer::on_actionShowFullScreen_triggered( bool checked )
{
    // 暂时没有作用
    if ( checked == true )
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}

void qgis_devComposer::on_actionAddArrow_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::AddArrow );
    }
}

void qgis_devComposer::on_actionPanComposer_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::Pan );
    }
}

void qgis_devComposer::on_actionSelectTool_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::Select );
    }
}

void qgis_devComposer::on_actionAddMap_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::AddMap );
    }
}

void qgis_devComposer::on_actionAddHtml_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::AddHtml );
    }
}

void qgis_devComposer::on_actionAddShape_triggered()
{
    if ( m_composerView )
    {
        m_composerView->setCurrentTool( QgsComposerView::AddRectangle );
    }
}

