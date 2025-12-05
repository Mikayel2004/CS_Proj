#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <vector>
#include "quadtree.h"

class SimulationWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  SimulationWidget(QWidget *parent = nullptr);
  ~SimulationWidget();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  QOpenGLShaderProgram *m_program;
  QTimer m_timer;
  std::vector<Particle> m_particles;

         // GPU Memory Objects
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vbo;

         // Simulation Constants
  const int N_PARTICLES = 4000;
  const float G_CONST = 0.5f;
  const float THETA = 0.5f;
  const float EPSILON = 0.05f;
  const float DT = 0.005f;

  void initGalaxy();
  void stepPhysics();
};

#endif // SIMULATIONWIDGET_H
