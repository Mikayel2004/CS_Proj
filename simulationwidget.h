#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>            // <--- Добавлено
#include <QOpenGLVertexArrayObject> // <--- Добавлено
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

  // Объекты для правильной работы с памятью видеокарты
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vbo;

  const int N_PARTICLES = 4000; // Можно увеличить число частиц
  const float G_CONST = 0.5f;
  const float THETA = 0.5f;
  const float EPSILON = 0.05f;
  const float DT = 0.005f; // Уменьшил шаг времени для плавности

  void initGalaxy();
  void stepPhysics();
};

#endif
