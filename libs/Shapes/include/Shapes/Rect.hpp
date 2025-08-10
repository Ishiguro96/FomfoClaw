#pragma once

#include "Shape.hpp"

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace ish::shapes {

class Rect : public Shape
{
  public:
    Rect(const sf::Vector2f& pos, const sf::Vector2f& size, b2WorldId worldId, Options options = {});
    ~Rect();

    void update() override;
    void draw(sf::RenderWindow& window) override;

    b2ShapeId getShape() { return m_shapeId; }

  private:
    b2ShapeId m_shapeId;
    sf::RectangleShape m_sfmlShape;
};

} // namespace
