#ifndef QGSMAPTOOLZOOMEVENT_H
#define QGSMAPTOOLZOOMEVENT_H

#include <qgsmaptoolzoom.h>
#include <qgsmapcanvas.h>


class QgsMapToolZoomEvent : public QgsMapToolZoom
{
public:
    QgsMapToolZoomEvent(QgsMapCanvas *canvas, bool zoomOut, QList <QgsMapCanvasLayer> &layerSet);

private:
    QList <QgsMapCanvasLayer> layerSet;

signals:

public slots:
    void setVisibleLablesLayers(bool isVisible);
    void canvasReleaseEvent(QgsMapMouseEvent *e);

};

#endif // QGSMAPTOOLZOOMEVENT_H
