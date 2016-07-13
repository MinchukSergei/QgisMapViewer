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

#include "qgsmaptoolzoomevent.h"


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
    addLocationLayer();
    addAllLayers();



    //create a little toolbar
    mpMapToolBar = addToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
    mpMapToolBar->addAction(mpLocate);

    //create the maptools
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoomEvent(mpMapCanvas, FALSE, myLayerSet); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoomEvent(mpMapCanvas, TRUE, myLayerSet); //true = out
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

    //need to swap coorinates x with y
    double x = 27.558808;
    double y = 53.902049;
    if (!xLineEditCoordinate->text().isEmpty() && !xLineEditCoordinate->text().isEmpty())
    {
        y = xLineEditCoordinate->text().toDouble();
        x = yLineEditCoordinate->text().toDouble();
    }

    double h = mpMapCanvas->scale();

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
    QgsPoint point = QgsPoint(x, y);
    QgsRectangle rect = QgsRectangle(point, point);

    mpMapCanvas->setExtent(rect);

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

    QgsVectorLayer * mypLayer;

    for (int i = 0; i < layerNames.count(); ++i) {
        mypLayer = new QgsVectorLayer(layerPath + layerNames.at(i) + filenameExtention,
                                      myLayerBaseName, myProviderName);
        addLayer(*mypLayer, layerNames.at(i));
        enableLayerLables(mypLayer, false, layerLabeling.at(i));
    }


    mpMapCanvas->setExtent(mypLayer->extent());
    mpMapCanvas->setLayerSet(myLayerSet);
}

void MainWindow::addLocationLayer()
{
    QString locationLayerName = "location-point";
    locationLayer = new QgsVectorLayer(layerPath + locationLayerName + filenameExtention,
                                       myLayerBaseName, myProviderName);
    addLayer(*locationLayer, "loc");
    enableLayerLables(locationLayer, true, true);
    mpMapCanvas->setLayerSet(myLayerSet);
}


void MainWindow::addLayer(QgsVectorLayer &layer, QString name)
{
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(
                QgsSymbolV2::defaultSymbol(layer.geometryType()));

    layer.setRendererV2(mypRenderer);
    layer.setName(name);
    QgsMapLayerRegistry::instance()->addMapLayer(&layer, TRUE);
    myLayerSet.append(QgsMapCanvasLayer(&layer, TRUE));
}

void MainWindow::enableLayerLables(QgsVectorLayer *layer, bool isLocation, bool isVisible)
{
    QgsLabel * mypLabel;
    mypLabel = layer->label();
    //and the label attributes associated with the label
    QgsLabelAttributes * mypLabelAttributes;
    mypLabelAttributes = mypLabel->labelAttributes();
    //note in QGIS 1.4 and up you should use mypLabel->labelAttributes rather

    //get the field list associated with the layer
    //we'll print the names out to console for diagnostic purposes

    QgsFields myFields = layer->fields();
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

    layer->enableLabels(isVisible);
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


