#include "quad.h"

using namespace std;

Quad::Quad() {

  _indices = {0, 1, 3, 1, 2, 3};
  _vertices.resize(4);

  _vertices[0] = Vertex(
      Eigen::Vector4f(-1.f, -1.f, 0.f, 1.f), Eigen::Vector4f::Constant(1.f),
      Eigen::Vector3f(0.f, 0.f, 1.f), Eigen::Vector2f(0.f, 1.f));

  _vertices[1] = Vertex(
      Eigen::Vector4f(1.f, -1.f, 0.f, 1.f), Eigen::Vector4f::Constant(1.f),
      Eigen::Vector3f(0.f, 0.f, 1.f), Eigen::Vector2f(1.f, 1.f));

  _vertices[2] = Vertex(
      Eigen::Vector4f(1.f, 1.f, 0.f, 1.f), Eigen::Vector4f::Constant(1.f),
      Eigen::Vector3f(0.f, 0.f, 1.f), Eigen::Vector2f(1.f, 0.f));

  _vertices[3] = Vertex(
      Eigen::Vector4f(-1.f, 1.f, 0.f, 1.f), Eigen::Vector4f::Constant(1.f),
      Eigen::Vector3f(0.f, 0.f, 1.f), Eigen::Vector2f(1.f, 0.f));

  _bbox.extend(Eigen::Vector3f(-1, -1, 0));
  _bbox.extend(Eigen::Vector3f(1, 1, 0));
}

Quad::~Quad() {}