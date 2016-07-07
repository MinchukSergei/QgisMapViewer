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

    QString myLayerPath         = "/home/minchuk/Downloads/placept.shp";
    QString myLayerBaseName     = "italy";
    QString myProviderName      = "ogr";

    // Instantiate Provider Registry


    QgsVectorLayer * mypLayer =  new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);

    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(QgsSymbolV2::defaultSymbol(mypLayer->geometryType()));


    QList <QgsMapCanvasLayer> myLayerSet;

    mypLayer->setRendererV2(mypRenderer);
    if (mypLayer->isValid())
    {
        qDebug("Layer is valid");
    }
    else
    {
        qDebug("Layer is NOT valid");
    }

    // Add the Vector Layer to the Layer Registry
    QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    myLayerSet.append(QgsMapCanvasLayer(mypLayer, TRUE));

    QgsMapCanvas *map = new QgsMapCanvas(0, 0);
    return app.exec();
}
