#include "Shape.hpp"

namespace ish::shapes {

Shape::~Shape()
{
  b2DestroyBody(m_bodyId);
}

void Shape::setFilter(b2Filter filter)
{
  constexpr size_t MAX_SHAPES = 64U;
  std::array<b2ShapeId, MAX_SHAPES> shapes;
  b2Body_GetShapes(m_bodyId, &shapes[0], MAX_SHAPES);
  for (const auto& shape : shapes)
  {
    if (not b2Shape_IsValid(shape)) {
      break;
    }

    b2Shape_SetFilter(shape, filter);
  }
}

} // namespace
