#include "qgis_dev_layerPropDialog.h"
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgslabel.h>
#include <qgslabelattributes.h>

qgis_dev_layerPropDialog::qgis_dev_layerPropDialog( QWidget *parent, QgsVectorLayer* layer, QgsMapCanvas* mapCanvas )
    : QDialog( parent ), m_layer( layer ), m_mapCanvas( mapCanvas )
{
    this->setupUi( this );

    //this->labelField_comboBox->enabled();

    connect( this->okButton, SIGNAL( clicked() ), this, SLOT( enableLable() ) );
    connect( this->ifDisLable_checkBox, SIGNAL( toggled( bool ) ), this, SLOT( ifDisLabel( bool ) ) );
}


qgis_dev_layerPropDialog::~qgis_dev_layerPropDialog()
{

}

void qgis_dev_layerPropDialog::enableLable()
{
    m_layer->label()->setLabelField( QgsLabel::Text, 1 );
    m_layer->label()->labelAttributes()->setColor( Qt::black );
    m_layer->label()->labelAttributes()->setFamily( "comic,15" );
    m_layer->enableLabels( true );
}

void qgis_dev_layerPropDialog::ifDisLabel( bool dis )
{
    if ( dis == true )
    {
//        this->labelField_comboBox->enabled( true );
    }
}
