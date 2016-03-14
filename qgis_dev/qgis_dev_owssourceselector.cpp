#include "qgis_dev_owssourceselector.h"

#include <QSettings>
#include <QMap>

qgis_dev_OWSSourceSelector::qgis_dev_OWSSourceSelector( QString providerName, QWidget *parent )
    : QgsOWSSourceSelect( providerName, parent )
{
    QMap<QString, QString> exampleServers;
    exampleServers["DM Solutions GMap"] = "http://www2.dmsolutions.ca/cgi-bin/mswms_gmap";
    exampleServers["Lizardtech server"] =  "http://wms.lizardtech.com/lizardtech/iserv/ows";
    // Nice to have the qgis users map, but I'm not sure of the URL at the moment.
    //  exampleServers["Qgis users map"] = "http://qgis.org/wms.cgi";

    QSettings settings;
    settings.beginGroup( "/Qgis/connections-wms" );
    QMap<QString, QString>::const_iterator i = exampleServers.constBegin();
    for ( ; i != exampleServers.constEnd(); ++i )
    {
        // Only do a server if it's name doesn't already exist.
        QStringList keys = settings.childGroups();
        if ( !keys.contains( i.key() ) )
        {
            QString path = i.key();
            settings.setValue( path + "/url", i.value() );
        }
    }
    settings.endGroup();
    settings.beginGroup( "/Qgis/connections-wms" );
    this->mConnectionsComboBox->clear();
    this->mConnectionsComboBox->addItems( settings.childGroups() );

}

qgis_dev_OWSSourceSelector::~qgis_dev_OWSSourceSelector()
{

}
