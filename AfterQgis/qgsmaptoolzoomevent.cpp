#include "qgsmaptoolzoomevent.h"
#include <qgsmapcanvas.h>
#include <qgsvectorlayer.h>
#include "mainwindow.h"

QgsMapToolZoomEvent::QgsMapToolZoomEvent(QgsMapCanvas *canvas, bool zoomOut,
                                         QList <QgsMapCanvasLayer> &layerSet) : QgsMapToolZoom(canvas, zoomOut)
{
    this->layerSet = QList<QgsMapCanvasLayer>(layerSet);
}

void QgsMapToolZoomEvent::canvasReleaseEvent(QgsMapMouseEvent *e)
{
    if ( e->button() != Qt::LeftButton )
        return;

    // We are not really dragging in this case. This is sometimes caused by
    // a pen based computer reporting a press, move, and release, all the
    // one point.
    if ( mDragging && ( mZoomRect.topLeft() == mZoomRect.bottomRight() ) )
    {
        mDragging = false;
        delete mRubberBand;
        mRubberBand = nullptr;
    }

    if ( mDragging )
    {
        mDragging = false;
        delete mRubberBand;
        mRubberBand = nullptr;

        // store the rectangle
        mZoomRect.setRight( e->pos().x() );
        mZoomRect.setBottom( e->pos().y() );

        //account for bottom right -> top left dragging
        mZoomRect = mZoomRect.normalized();

        // set center and zoom
        const QSize& zoomRectSize = mZoomRect.size();
        const QgsMapSettings& mapSettings = mCanvas->mapSettings();
        const QSize& canvasSize = mapSettings.outputSize();
        double sfx = ( double )zoomRectSize.width() / canvasSize.width();
        double sfy = ( double )zoomRectSize.height() / canvasSize.height();
        double sf = qMax( sfx, sfy );

        const QgsMapToPixel* m2p = mCanvas->getCoordinateTransform();
        QgsPoint c = m2p->toMapCoordinates( mZoomRect.center() );

        mCanvas->zoomByFactor( mZoomOut ? 1.0 / sf : sf, &c );

        mCanvas->refresh();
    }
    else // not dragging
    {
        // change to zoom in/out by the default multiple
        mCanvas->zoomWithCenter( e->x(), e->y(), !mZoomOut );
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
