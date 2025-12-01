#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector2D>
#include <vector>
#include <cmath>

struct Particle {
  QVector2D pos;
  QVector2D vel;
  QVector2D acc;
  float mass;
};

class Quadtree
{
public:
  float x, y;
  float size;
  float totalMass;
  QVector2D centerOfMass;
  Particle* particle;

  Quadtree* nw;
  Quadtree* ne;
  Quadtree* sw;
  Quadtree* se;

  Quadtree(float _x, float _y, float _size);
  ~Quadtree();

  void insert(Particle* p);
  void calculateForce(Particle* p, float theta, float G, float epsilon);

private:
  bool isLeaf() const;
  void subdivide();
};

#endif
