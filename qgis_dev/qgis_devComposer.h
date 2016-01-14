#ifndef QGIS_DEVCOMPOSER_H
#define QGIS_DEVCOMPOSER_H

#include <QObject>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>

#include "ui_qgis_dev_composerBase.h"
#include <qgscomposerview.h>


class qgis_devComposer : public QMainWindow , public Ui_composerBase
{
    Q_OBJECT
public:
    qgis_devComposer( QWidget *parent = 0 );
    ~qgis_devComposer();

private:
    QgsComposerView* m_composerView;
};

#endif // QGIS_DEVCOMPOSER_H