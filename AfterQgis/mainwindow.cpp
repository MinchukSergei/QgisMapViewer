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
  //mpMapCanvas->useImageToRender(false);
  mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mpMapCanvas->freeze(false);
//  mpMapCanvas->setVisible(true);
//  mpMapCanvas->refresh();
//  mpMapCanvas->show();

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
    QString layerPath                 = "/home/minchuk/work/practice/BY_map_osmshp/data/";
    QString myLayerBaseName     = "test";
    QString myProviderName      = "ogr";

    QList <QgsMapCanvasLayer> myLayerSet;

    QVector<QString> layerNames;
    readLayerNames(layerNames);

    QString layerName;
    QString filenameExtention = ".shp";
    QgsVectorLayer * mypLayer;
    foreach (layerName, layerNames) {
        mypLayer =  new QgsVectorLayer(layerPath + layerName + filenameExtention, myLayerBaseName, myProviderName);
        QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(
                    QgsSymbolV2::defaultSymbol(mypLayer->geometryType()));
        mypLayer->setRendererV2(mypRenderer);
        QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, 1);
        myLayerSet.append(QgsMapCanvasLayer(mypLayer, TRUE));
    }

    mpMapCanvas->setExtent(mypLayer->extent());
    mpMapCanvas->setLayerSet(myLayerSet);
}

void MainWindow::readLayerNames(QVector<QString> &layerNames)
{
    QFile inputFile(QDir::currentPath() + "/layers_names.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          layerNames.append(in.readLine());
       }
       inputFile.close();
    }
}


