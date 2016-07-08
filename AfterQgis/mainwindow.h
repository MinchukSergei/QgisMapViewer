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
    public slots:
      void zoomInMode();
      void zoomOutMode();
      void panMode();
      void addLayer();

    private:
     QgsMapCanvas * mpMapCanvas;
     QVBoxLayout  * mpLayout;
     QToolBar * mpMapToolBar;
     QgsMapTool * mpPanTool;
     QgsMapTool * mpZoomInTool;
     QgsMapTool * mpZoomOutTool;

};

#endif
