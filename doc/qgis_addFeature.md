```sequence
title: 添加矢量要素
note right of QgsMapCanvas:用户点击一个地图点位置
QgsMapCanvas->QgsMapTool:canvasReleaseEvent\n(QMouseEvent* e)
QgsMapTool->QgsMapToolAdvancedDigitizing:canvasReleaseEvent\n(QMouseEvent* e)
note over QgsMapToolAdvancedDigitizing:将事件封装成 \n QgsMapMouseEvent \n 带上地理坐标
QgsMapToolAdvancedDigitizing->QgsMapToolEdit: canvasMapReleaseEvent\n( QgsMapMouseEvent* e )
QgsMapToolEdit->QgsMapToolCapture:canvasMapReleaseEvent\n( QgsMapMouseEvent* e )
QgsMapToolCapture->QgsMapToolAddFeature:canvasMapReleaseEvent\n( QgsMapMouseEvent* e )
QgsMapToolAddFeature-->QgsMapCanvas:get current layer
QgsMapCanvas-->QgsMapToolAddFeature:current layer
QgsMapToolAddFeature-->QgsVectorDataProvider:data provider
QgsVectorDataProvider-->QgsMapToolAddFeature:data provider
QgsMapToolAddFeature-->QgsMapToolAddFeature:addFeature( vlayer, &feature, false );
QgsMapToolAddFeature-->QgsFeatureAction:addFeature( QgsAttributeMap(), showModal );

```
