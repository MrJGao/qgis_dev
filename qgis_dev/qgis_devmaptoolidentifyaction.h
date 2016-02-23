#ifndef QGIS_DEVMAPTOOLIDENTIFYACTION_H
#define QGIS_DEVMAPTOOLIDENTIFYACTION_H

#include <QObject>
#include <QMouseEvent>

#include <qgsmaptoolidentify.h>

//! 用于实现属性识别工具
class qgis_devMapToolIdentifyAction : public QgsMapToolIdentify
{
    Q_OBJECT

public:
    qgis_devMapToolIdentifyAction( QgsMapCanvas * canvas );
    ~qgis_devMapToolIdentifyAction();

    //! 重写鼠标键释放事件
    virtual void canvasReleaseEvent( QMouseEvent * e );

};

#endif // QGIS_DEVMAPTOOLIDENTIFYACTION_H
