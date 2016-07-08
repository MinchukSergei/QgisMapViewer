#include "mainwindow.h"

#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
  setupUi(this);

  QString pluginsDir            = "/usr/local/lib/qgis/plugins";

  QgsProviderRegistry::instance(pluginsDir);

  // Create the Map Canvas

  mpMapCanvas= new QgsMapCanvas(0, 0);
  mpMapCanvas->enableAntiAliasing(true);
  mpMapCanvas->useImageToRender(false);
  mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mpMapCanvas->freeze(false);
  mpMapCanvas->setVisible(true);
  mpMapCanvas->refresh();
  mpMapCanvas->show();

  // Lay our widgets out in the main window
  mpLayout = new QVBoxLayout(frameMap);
  mpLayout->addWidget(mpMapCanvas);

  //create the action behaviours
  connect(mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
  connect(mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
  connect(mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));

  //addLayer immidietly
  addLayer();

  //create a little toolbar
  mpMapToolBar = addToolBar(tr("File"));
  mpMapToolBar->addAction(mpActionZoomIn);
  mpMapToolBar->addAction(mpActionZoomOut);
  mpMapToolBar->addAction(mpActionPan);

  //create the maptools
  mpPanTool = new QgsMapToolPan(mpMapCanvas);
  mpPanTool->setAction(mpActionPan);
  mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
  mpZoomInTool->setAction(mpActionZoomIn);
  mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
  mpZoomOutTool->setAction(mpActionZoomOut);
}

MainWindow::~MainWindow()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMapCanvas;
  delete mpLayout;
}

void MainWindow::panMode()
{
  mpMapCanvas->setMapTool(mpPanTool);
}

void MainWindow::zoomInMode()
{
  mpMapCanvas->setMapTool(mpZoomInTool);
}

void MainWindow::zoomOutMode()
{
  mpMapCanvas->setMapTool(mpZoomOutTool);
}

void MainWindow::addLayer()
{
    QString layerPath               = "/home/minchuk/Downloads/AllTypes/";
    QString layerPathPolylines      = layerPath + "polylines.shp";
    QString layerPathPolygons       = layerPath + "polygons.shp";
    QString layerPathPoints         = layerPath + "points.shp";

    QString myLayerBaseName     = "test";
    QString myProviderName      = "ogr";

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
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPoints, TRUE));
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPolygons, TRUE));
    myLayerSet.append(QgsMapCanvasLayer(mypLayerPolyLines, TRUE));

    // Create the Map Canvas


    mpMapCanvas->setExtent(mypLayerPolygons->extent());
    mpMapCanvas->setLayerSet(myLayerSet);
}


