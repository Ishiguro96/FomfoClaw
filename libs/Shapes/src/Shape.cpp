#include "Shape.hpp"

namespace ish::shapes {

Shape::~Shape()
{
  b2DestroyBody(m_bodyId);
}

} // namespace
