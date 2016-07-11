#ifndef OMGMAINWINDOW_H
#define OMGMAINWINDOW_H

#include <qgsmapcanvas.h>
#include <qgsmaptool.h>

#include <QtGui>

//Local Includes
#include <ui_mainwindowbase.h>

class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
  Q_OBJECT;
    public:
      MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
      ~MainWindow();
      QString layerPath                 = "/home/minchuk/work/practice/BY_map_osmshp/data/";
      QString filenameExtention         = ".shp";
      QString myLayerBaseName           = "test";
      QString myProviderName            = "ogr";

    public slots:
      void zoomInMode();
      void zoomOutMode();
      void panMode();
      void locate();
      void addLayer(QgsVectorLayer &layer);
      void addLocationLayer();
      void addAllLayers();

    private:
     QgsMapCanvas * mpMapCanvas;
     QVBoxLayout  * mpLayout;
     QToolBar * mpMapToolBar;
     QgsMapTool * mpPanTool;
     QgsMapTool * mpZoomInTool;
     QgsMapTool * mpZoomOutTool;
     QList <QgsMapCanvasLayer> myLayerSet;
     QgsVectorLayer * locationLayer;


private slots:
     void readLayerNames(QVector<QString> &layerNames);
     void readLayerLabeling(QVector<bool> &layerLabeling);
     void enableLayerLables(QgsVectorLayer &layer, bool isLocation);
     double fRand(double fMin, double fMax);
};

#endif
