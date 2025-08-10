#include "Circle.hpp"

#include "Constants/SfmlConstants.hpp"

using namespace ish::constants;

namespace ish::shapes
{

Circle::Circle(const sf::Vector2f& pos, float radius, b2WorldId worldId, Options options)
  : m_sfmlShape(radius)
{
  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.position = b2Vec2{pos.x / PPM, pos.y / PPM};
  bodyDef.type = static_cast<b2BodyType>(options.type);

  m_bodyId = b2CreateBody(worldId, &bodyDef);

  b2Circle bodyShape (b2Vec2(0.f, 0.f), radius / PPM);
  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = options.density;
  m_shapeId = b2CreateCircleShape(m_bodyId, &shapeDef, &bodyShape);

  // Create SFML rectangle shape to represent Box2D's body.
  m_sfmlShape.setOrigin({m_sfmlShape.getGlobalBounds().size.x / 2.f, m_sfmlShape.getGlobalBounds().size.y / 2.f});
  m_sfmlShape.setFillColor(sf::Color::Yellow);
}

Circle::~Circle()
{
  b2DestroyShape(m_shapeId, false);
}

void Circle::update()
{
  b2Vec2 bodyPos = b2Body_GetPosition(m_bodyId);
  m_sfmlShape.setPosition({bodyPos.x * PPM, bodyPos.y * PPM});
}

void Circle::draw(sf::RenderWindow& window)
{
  window.draw(m_sfmlShape);
}

} // namespace
