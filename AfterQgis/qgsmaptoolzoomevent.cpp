#include "qgsmaptoolzoomevent.h"
#include "qgsmaptoolzoom.h"
#include "qgsmapcanvas.h"
#include "qgsmaptopixel.h"
#include "qgscursors.h"
#include "qgsrubberband.h"

#include <QMouseEvent>
#include <QRect>
#include <QColor>
#include <QCursor>
#include <QPixmap>
#include <qgsvectorlayer.h>
#include "qgslogger.h"
#include "mainwindow.h"

QgsMapToolZoomEvent::QgsMapToolZoomEvent(QgsMapCanvas *canvas, bool zoomOut,
                                         QList <QgsMapCanvasLayer> &layerSet) : QgsMapToolZoom(canvas, zoomOut)
{
    this->layerSet = QList<QgsMapCanvasLayer>(layerSet);
}

void QgsMapToolZoomEvent::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    QgsMapToolZoom::canvasReleaseEvent(e);

    if (mCanvas->scale() < 2000)
    {
        setVisibleLablesLayers(true);
    } else
    {
        setVisibleLablesLayers(false);
    }
}

void QgsMapToolZoomEvent::setVisibleLablesLayers(bool isVisible)
{
    for (int i = 0; i < layerSet.count(); ++i) {
        QgsMapCanvasLayer canvasLayer = layerSet.at(i);
        QgsVectorLayer *layer = (QgsVectorLayer *)canvasLayer.layer();
        if (layer->name() != "loc")
        {
            ((MainWindow*)canvas()->parentWidget())->enableLayerLables(layer, 0, isVisible);
        }
    }
}
