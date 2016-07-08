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
    QgsApplication app(argc, argv, TRUE);
    MainWindow * mypMainWindow = new MainWindow();
    mypMainWindow->showFullScreen();
      // Start the Application Event Loop
    return app.exec();
}
