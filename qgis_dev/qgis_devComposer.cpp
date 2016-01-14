#include "qgis_devComposer.h"
#include "qgis_dev.h"

#include <QGridLayout>

qgis_devComposer::qgis_devComposer( QWidget *parent )
    : QMainWindow( parent )

{
    setupUi( this );

    m_composerView = new QgsComposerView( this );

    this->setCentralWidget( m_composerView );
}


qgis_devComposer::~qgis_devComposer()
{
}
