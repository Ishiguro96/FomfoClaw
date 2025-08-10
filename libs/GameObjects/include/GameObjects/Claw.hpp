#pragma once

#include "box2d/box2d.h"

#include "Shapes/Shape.hpp"

#include <memory>
#include <vector>

namespace ish::game_objects {

class Claw
{
  public:
    explicit Claw(b2WorldId worldId);

    void update(b2WorldId worldId);
  private:
    std::vector<std::unique_ptr<shapes::Shape>> m_shapes;
};

} // namespace
