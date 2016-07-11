#include "mainwindow.h"

#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>
#include <qgsvectordataprovider.h>

#include <qgslabel.h>
#include <qgslabelattributes.h>
#include <qgsfield.h>


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
    connect(mpLocate, SIGNAL(triggered()), this, SLOT(locate()));

    //add layers recently
    addAllLayers();
    addLocationLayer();

    //create a little toolbar
    mpMapToolBar = addToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
    mpMapToolBar->addAction(mpLocate);

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

void MainWindow::locate()
{
    locationLayer->startEditing();
    QgsVectorDataProvider *lDataProvider;
    lDataProvider = locationLayer->dataProvider();


    int xMin = 25;
    int xMax = 26;
    int yMin = 54;
    int yMax = 55;

    double x = fRand(xMin, xMax);
    double y = fRand(yMin, yMax);

    QgsGeometry *geom;
    geom = QgsGeometry::fromPoint(QgsPoint(x, y));

    QgsFeature feature;
    QgsFeatureIterator iter = lDataProvider->getFeatures();
    iter.nextFeature(feature);
    feature.setGeometry(*geom);

    QgsFeatureId featureId;
    featureId = feature.id();
    QgsGeometryMap geometryMap;
    geometryMap.insert(featureId, *geom);
    lDataProvider->changeGeometryValues(geometryMap);

    locationLayer->commitChanges();
    //mpMapCanvas->refresh();
}

double MainWindow::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void MainWindow::addAllLayers()
{
    QVector<QString> layerNames;
    readLayerNames(layerNames);
    QVector<bool> layerLabeling;
    readLayerLabeling(layerLabeling);

    QString layerName;

    QgsVectorLayer * mypLayer;

    for (int i = 0; i < layerNames.count(); ++i) {
        mypLayer = new QgsVectorLayer(layerPath + layerNames.at(i) + filenameExtention,
                                      myLayerBaseName, myProviderName);;
        addLayer(*mypLayer);
        if (layerLabeling.at(i)) {
            enableLayerLables(*mypLayer, false);
        }
    }


    mpMapCanvas->setExtent(mypLayer->extent());
    mpMapCanvas->setLayerSet(myLayerSet);
}

void MainWindow::addLocationLayer()
{
    QString locationLayerName = "location-point";
    locationLayer = new QgsVectorLayer(layerPath + locationLayerName + filenameExtention,
                                                   myLayerBaseName, myProviderName);
    addLayer(*locationLayer);
    enableLayerLables(*locationLayer, true);
    mpMapCanvas->setLayerSet(myLayerSet);
}


void MainWindow::addLayer(QgsVectorLayer &layer)
{
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(
                QgsSymbolV2::defaultSymbol(layer.geometryType()));
    layer.setRendererV2(mypRenderer);
    QgsMapLayerRegistry::instance()->addMapLayer(&layer, TRUE);
    myLayerSet.append(QgsMapCanvasLayer(&layer, TRUE));
}

void MainWindow::enableLayerLables(QgsVectorLayer &layer, bool isLocation)
{
    QgsLabel * mypLabel;
    mypLabel = layer.label();
    //and the label attributes associated with the label
    QgsLabelAttributes * mypLabelAttributes;
    mypLabelAttributes = mypLabel->labelAttributes();
    //note in QGIS 1.4 and up you should use mypLabel->labelAttributes rather

    //get the field list associated with the layer
    //we'll print the names out to console for diagnostic purposes

    QgsFields myFields = layer.fields();
    int indexFieldName = 0;

    for (unsigned int i = 0; i < myFields.size(); i++ )
    {
        qDebug() << "Field Name: " +  QString(myFields[i].name()).toLocal8Bit();
        if (QString(myFields[i].name()).toLocal8Bit() == "NAME")
        {
            indexFieldName = i;
            break;
        }
    }
    //just use the last field's name in the fields list as the label field!
    qDebug() << "set label field to " + QString(myFields[myFields.size()-1].name()).toLocal8Bit();
    mypLabel->setLabelField( QgsLabel::Text,  indexFieldName );
    //set the colour of the label text
    mypLabelAttributes->setColor(Qt::black);

    //create a 'halo' effect around each label so it
    //can still be read on dark backgrounds
    if (isLocation)
    {
        mypLabelAttributes->setColor(Qt::red);
        mypLabelAttributes->setBold(TRUE);
        mypLabelAttributes->setSize(15, QgsLabelAttributes::PointUnits);
        mypLabelAttributes->setBufferColor(Qt::black);
    }
    mypLabelAttributes->setBufferEnabled(true);
    int myType = QgsLabelAttributes::PointUnits;
    mypLabelAttributes->setBufferSize(1,myType);

    layer.enableLabels(TRUE);
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

// read the file which contains boolean values for activating layers listed in layers_names.txt
void MainWindow::readLayerLabeling(QVector<bool> &layerLabeling)
{
    QFile inputFile(QDir::currentPath() + "/layers_lableling.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            layerLabeling.append(!!in.readLine().toInt());
        }
        inputFile.close();
    }
}


