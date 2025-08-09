#pragma once

#include "Shape.hpp"

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include <vector>

namespace ish::shapes {

class Polygon : public Shape
{
  public:
    Polygon(const sf::Vector2f& pos, const std::vector<sf::Vector2f>& points, b2WorldId worldId, Options options = {});
    ~Polygon();

    void update() override;
    void draw(sf::RenderWindow& window) override;

  private:
    std::vector<b2ShapeId> m_shapeIds;
    std::vector<sf::ConvexShape> m_sfmlShapes;
};

} // namespace
