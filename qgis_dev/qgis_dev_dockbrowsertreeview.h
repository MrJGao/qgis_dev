#ifndef QGIS_DEV_DOCKBROWSERTREEVIEW_H
#define QGIS_DEV_DOCKBROWSERTREEVIEW_H

#include <QWidget>
#include <qgsbrowsertreeview.h>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

class qgis_dev_dockBrowserTreeView : public QgsBrowserTreeView
{
    Q_OBJECT

public:
    qgis_dev_dockBrowserTreeView( QWidget *parent );
    ~qgis_dev_dockBrowserTreeView();

    void dragEnterEvent( QDragEnterEvent* e ) override;
    void dragMoveEvent( QDragMoveEvent* e ) override;

private:

};

#endif // QGIS_DEV_DOCKBROWSERTREEVIEW_H
