#include <QApplication>
#include <QSurfaceFormat>
#include "simulationwidget.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  SimulationWidget w;
  w.resize(800, 800);
  w.setWindowTitle("N-Body Barnes-Hut Simulation");
  w.show();

  return a.exec();
}
