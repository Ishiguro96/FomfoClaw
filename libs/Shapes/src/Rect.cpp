#include "Rect.hpp"

#include "Constants/SfmlConstants.hpp"

using namespace ish::constants;

namespace ish::shapes
{

Rect::Rect(const sf::Vector2f& pos, const sf::Vector2f& size, b2WorldId worldId, Options options)
  : m_sfmlShape(size)
{
  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.position = b2Vec2{pos.x / PPM, pos.y / PPM};
  bodyDef.type = static_cast<b2BodyType>(options.type);

  m_bodyId = b2CreateBody(worldId, &bodyDef);

  // Box2D uses half-width and half-height as rect definition, thus we need to
  // divide our size by half.
  b2Polygon bodyShape = b2MakeBox(size.x / (2.f * PPM), size.y / (2.f * PPM));
  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = options.density;

  if (options.isSensor)
  {
    shapeDef.isSensor = options.isSensor;
    shapeDef.enableSensorEvents = true;
  }

  m_shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &bodyShape);

  // Create SFML rectangle shape to represent Box2D's body.
  m_sfmlShape.setOrigin({m_sfmlShape.getGlobalBounds().size.x / 2.f, m_sfmlShape.getGlobalBounds().size.y / 2.f});
  m_sfmlShape.setFillColor(sf::Color::Yellow);
}

Rect::~Rect()
{
  b2DestroyShape(m_shapeId, false);
}

void Rect::update()
{
  b2Vec2 bodyPos = b2Body_GetPosition(m_bodyId);
  m_sfmlShape.setPosition({bodyPos.x * PPM, bodyPos.y * PPM});
}

void Rect::draw(sf::RenderWindow& window)
{
  window.draw(m_sfmlShape);
}

} // namespace
