#include "simulationwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
#include <QRandomGenerator>

SimulationWidget::SimulationWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_program(nullptr)
{
  connect(&m_timer, &QTimer::timeout, this, QOverload<>::of(&SimulationWidget::update));
  m_timer.start(16);
}

SimulationWidget::~SimulationWidget()
{
  makeCurrent();
  m_vbo.destroy();
  m_vao.destroy();
  delete m_program;
  doneCurrent();
}

void SimulationWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  m_program = new QOpenGLShaderProgram();

  const char *vshader = R"(
        #version 330 core
        layout(location = 0) in vec2 position;
        layout(location = 1) in vec2 velocity;
        out float speed;
        void main() {
            gl_Position = vec4(position * 0.002, 0.0, 1.0);
            float velMag = length(velocity);
            gl_PointSize = max(1.0, min(5.0, velMag * 1.5));
            speed = velMag;
        }
    )";

  const char *fshader = R"(
        #version 330 core
        in float speed;
        out vec4 fragColor;
        void main() {
            float t = clamp(speed * 0.2, 0.0, 1.0);
            vec3 cold = vec3(0.2, 0.4, 1.0);
            vec3 hot = vec3(1.0, 0.8, 0.4);
            vec2 coord = gl_PointCoord - vec2(0.5);
            float dist = length(coord);
            if(dist > 0.5) discard;
            float alpha = 1.0 - smoothstep(0.3, 0.5, dist);
            fragColor = vec4(mix(cold, hot, t), alpha);
        }
    )";

  if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader)) qDebug() << m_program->log();
  if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader)) qDebug() << m_program->log();
  if (!m_program->link()) qDebug() << m_program->log();

  m_vao.create();
  m_vao.bind();

  m_vbo.create();
  m_vbo.bind();

  initGalaxy();

  m_vbo.allocate(m_particles.data(), m_particles.size() * sizeof(Particle));

  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, offsetof(Particle, pos), 2, sizeof(Particle));

  m_program->enableAttributeArray(1);
  m_program->setAttributeBuffer(1, GL_FLOAT, offsetof(Particle, vel), 2, sizeof(Particle));

  m_vao.release();
  m_vbo.release();
}

void SimulationWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void SimulationWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!m_program) return;

  stepPhysics();

  m_vao.bind();
  m_vbo.bind();
  m_vbo.write(0, m_particles.data(), m_particles.size() * sizeof(Particle));
  m_vbo.release();

  m_program->bind();
  glDrawArrays(GL_POINTS, 0, m_particles.size());
  m_program->release();
  m_vao.release();
}

void SimulationWidget::initGalaxy()
{
  m_particles.resize(N_PARTICLES);
  for (int i = 0; i < N_PARTICLES; ++i) {
      float dist = 50.0f + 350.0f * (float(i) / N_PARTICLES);
      float angle = (float)i * 0.1f;
      m_particles[i].pos.setX(cos(angle) * dist);
      m_particles[i].pos.setY(sin(angle) * dist);
      float vel = sqrt(G_CONST * (N_PARTICLES * 50.0f) / dist) * 0.05f;
      m_particles[i].vel.setX(-sin(angle) * vel);
      m_particles[i].vel.setY(cos(angle) * vel);
      m_particles[i].mass = 10.0f;
      m_particles[i].acc = QVector2D(0, 0);
    }
  m_particles[0].pos = QVector2D(0, 0);
  m_particles[0].vel = QVector2D(0, 0);
  m_particles[0].mass = 50000.0f;
}

void SimulationWidget::stepPhysics()
{
  Quadtree* root = new Quadtree(0, 0, 2000.0f);
  for (int i = 0; i < N_PARTICLES; ++i) {
      m_particles[i].acc = QVector2D(0, 0);
      if (m_particles[i].pos.x() > -1000 && m_particles[i].pos.x() < 1000 &&
          m_particles[i].pos.y() > -1000 && m_particles[i].pos.y() < 1000) {
          root->insert(&m_particles[i]);
        }
    }
  for (int i = 0; i < N_PARTICLES; ++i) {
      root->calculateForce(&m_particles[i], THETA, G_CONST, EPSILON);
    }
  for (int i = 0; i < N_PARTICLES; ++i) {
      if (i == 0) continue;
      m_particles[i].vel += m_particles[i].acc * DT;
      m_particles[i].pos += m_particles[i].vel * DT;
    }
  delete root;
}
