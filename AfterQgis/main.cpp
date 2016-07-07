#include "mainwindow.h"
#include <QApplication>
#include <qgsapplication.h>
#include <qgsmapcanvas.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>



int main(int argc, char *argv[])
{
    QgsApplication app(argc, argv, true);

    QString layerPath           = "/home/minchuk/Downloads/AllTypes/";
    QString layerPathPolylines  = layerPath + "polylines.shp";
    QString layerPathPolygons   = layerPath + "polygons.shp";
    QString layerPathPoints     = layerPath + "points.shp";
    QString myPluginsDir        = "/usr/local/lib/qgis/plugins";
    QString myLayerBaseName     = "italy";
    QString myProviderName      = "ogr";

    // Instantiate Provider Registry
    QgsProviderRegistry * preg = QgsProviderRegistry::instance(myPluginsDir);

    QgsVectorLayer * mypLayerPolyLines =  new QgsVectorLayer(layerPathPolylines, myLayerBaseName, myProviderName);
    QgsVectorLayer * mypLayerPolygons =  new QgsVectorLayer(layerPathPolygons, myLayerBaseName, myProviderName);
    QgsVectorLayer * mypLayerPoints =  new QgsVectorLayer(layerPathPoints, myLayerBaseName, myProviderName);


    QgsSingleSymbolRendererV2 *mypRendererPolylines = new QgsSingleSymbolRendererV2(
                QgsSymbolV2::defaultSymbol(mypLayerPolyLines->geometryType()));
    QgsSingleSymbolRendererV2 *mypRendererPolygons = new QgsSingleSymbolRendererV2(
                QgsSymbolV2::defaultSymbol(mypLayerPolygons->geometryType()));
    QgsSingleSymbolRendererV2 *mypRendererPoints = new QgsSingleSymbolRendererV2(
                QgsSymbolV2::defaultSymbol(mypLayerPoints->geometryType()));


    QList <QgsMapCanvasLayer> myLayerSet;

    mypLayerPolyLines->setRendererV2(mypRendererPolylines);
    mypLayerPolygons->setRendererV2(mypRendererPolygons);
    mypLayerPoints->setRendererV2(mypRendererPoints);

    // Add the Vector Layer to the Layer Registry
    QgsMapLayerRegistry::instance()->addMapLayer(mypLayerPoints, 1);
    QgsMapLayerRegistry::instance()->addMapLayer(mypLayerPolygons, 1);
    QgsMapLayerRegistry::instance()->addMapLayer(mypLayerPolyLines, 1);
    // Add the Layer to the Layer Set
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPoints, 1));
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPolygons, 1));
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPolyLines, 1));

    // Create the Map Canvas
    QgsMapCanvas *mypMapCanvas = new QgsMapCanvas(0, 0);

    mypMapCanvas->setExtent(mypLayerPolygons->extent());
    mypMapCanvas->enableAntiAliasing(true);
    mypMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mypMapCanvas->freeze(false);

    mypMapCanvas->setLayerSet(myLayerSet);
    mypMapCanvas->setVisible(true);
    mypMapCanvas->refresh();


    return app.exec();
}
