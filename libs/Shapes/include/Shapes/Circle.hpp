#pragma once

#include "Shape.hpp"

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace ish::shapes {

class Circle : public Shape
{
  public:
    Circle(const sf::Vector2f& pos, float radius, b2WorldId worldId, Options options = {});
    ~Circle();

    void update() override;
    void draw(sf::RenderWindow& window) override;

  private:
    b2ShapeId m_shapeId;
    sf::CircleShape m_sfmlShape;
};

} // namespace
