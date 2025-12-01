#include "quadtree.h"

Quadtree::Quadtree(float _x, float _y, float _size)
    : x(_x), y(_y), size(_size), totalMass(0), centerOfMass(0, 0),
      particle(nullptr), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr)
{
}

Quadtree::~Quadtree()
{
  if (nw) delete nw;
  if (ne) delete ne;
  if (sw) delete sw;
  if (se) delete se;
}

bool Quadtree::isLeaf() const {
  return nw == nullptr;
}

void Quadtree::insert(Particle* p)
{
  if (isLeaf()) {
      if (particle == nullptr) {
          particle = p;
          totalMass = p->mass;
          centerOfMass = p->pos;
        } else {
          Particle* oldP = particle;
          particle = nullptr;
          subdivide();
          insert(oldP);
          insert(p);
        }
    } else {
      totalMass += p->mass;
      centerOfMass = (centerOfMass * (totalMass - p->mass) + p->pos * p->mass) / totalMass;

      if (p->pos.x() < x) {
          if (p->pos.y() < y) sw->insert(p);
          else nw->insert(p);
        } else {
          if (p->pos.y() < y) se->insert(p);
          else ne->insert(p);
        }
    }
}

void Quadtree::subdivide()
{
  float half = size / 2.0f;
  float q = size / 4.0f;
  nw = new Quadtree(x - q, y + q, half);
  ne = new Quadtree(x + q, y + q, half);
  sw = new Quadtree(x - q, y - q, half);
  se = new Quadtree(x + q, y - q, half);
}

void Quadtree::calculateForce(Particle* p, float theta, float G, float epsilon)
{
  if (totalMass == 0) return;

  QVector2D dir = centerOfMass - p->pos;
  float distSq = dir.lengthSquared();
  float dist = std::sqrt(distSq);

  if (isLeaf()) {
      if (particle != p && particle != nullptr) {
          float f = (G * p->mass * particle->mass) / (distSq + epsilon);
          p->acc += dir.normalized() * f;
        }
    } else {
      if (size / dist < theta) {
          float f = (G * p->mass * totalMass) / (distSq + epsilon);
          p->acc += dir.normalized() * f;
        } else {
          nw->calculateForce(p, theta, G, epsilon);
          ne->calculateForce(p, theta, G, epsilon);
          sw->calculateForce(p, theta, G, epsilon);
          se->calculateForce(p, theta, G, epsilon);
        }
    }
}
