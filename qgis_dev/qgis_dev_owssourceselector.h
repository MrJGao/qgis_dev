#ifndef QGIS_DEV_OWSSOURCESELECTOR_H
#define QGIS_DEV_OWSSOURCESELECTOR_H

#include <qgsowssourceselect.h>

class qgis_dev_OWSSourceSelector : public QgsOWSSourceSelect
{
    Q_OBJECT

public:
    qgis_dev_OWSSourceSelector( QString providerName, QWidget *parent );
    ~qgis_dev_OWSSourceSelector();

private:

};

#endif // QGIS_DEV_OWSSOURCESELECTOR_H
