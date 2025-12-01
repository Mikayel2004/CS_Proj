// Не забудьте подключить QDebug в самом верху файла simulationwidget.cpp:
#include <QDebug>

void SimulationWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
  glEnable(GL_PROGRAM_POINT_SIZE);

  m_program = new QOpenGLShaderProgram();

  const char *vshader = R"(
        #version 330 core
        layout(location = 0) in vec2 position;
        layout(location = 1) in vec2 velocity;
        out float speed;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
            float velMag = length(velocity);
            gl_PointSize = max(1.0, min(4.0, velMag * 0.1));
            speed = velMag;
        }
    )";

  const char *fshader = R"(
        #version 330 core
        in float speed;
        out vec4 fragColor;
        void main() {
            float t = clamp(speed * 0.005, 0.0, 1.0);
            vec3 cold = vec3(0.1, 0.3, 0.9);
            vec3 hot = vec3(1.0, 0.9, 0.4);
            fragColor = vec4(mix(cold, hot, t), 1.0);
        }
    )";

         // --- КОМПИЛЯЦИЯ С ПРОВЕРКОЙ ОШИБОК ---
  if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader)) {
      qDebug() << "Vertex Shader Error:" << m_program->log();
    }
  if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader)) {
      qDebug() << "Fragment Shader Error:" << m_program->log();
    }
  if (!m_program->link()) {
      qDebug() << "Link Error:" << m_program->log();
    }
  // -------------------------------------

  m_vao.create();
  m_vao.bind();

  m_vbo.create();
  m_vbo.bind();
  m_vbo.allocate(N_PARTICLES * sizeof(Particle));

  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, offsetof(Particle, pos), 2, sizeof(Particle));

  m_program->enableAttributeArray(1);
  m_program->setAttributeBuffer(1, GL_FLOAT, offsetof(Particle, vel), 2, sizeof(Particle));

  m_vao.release();
  m_vbo.release();
}
