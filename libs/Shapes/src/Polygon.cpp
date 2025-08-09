#include "Polygon.hpp"

#include "mapbox/earcut.hpp"

#include <algorithm>
#include <print>
#include <cassert>

namespace ish::shapes
{

Polygon::Polygon(const sf::Vector2f& pos, const std::vector<sf::Vector2f>& points, b2WorldId worldId, Options options)
{
  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.position = b2Vec2{pos.x / PPM, pos.y / PPM};
  bodyDef.type = static_cast<b2BodyType>(options.type);

  m_bodyId = b2CreateBody(worldId, &bodyDef);

  // Translate polygon to be center-wise
  const auto [minX, maxX] = std::minmax_element(std::begin(points), std::end(points), [](const sf::Vector2f& point1, const sf::Vector2f& point2){
    return point1.x < point2.x;
  });
  float diffX = (maxX->x - minX->x) / 2.f;

  const auto [minY, maxY] = std::minmax_element(std::begin(points), std::end(points), [](const sf::Vector2f& point1, const sf::Vector2f& point2){
    return point1.y < point2.y;
  });
  float diffY = (maxY->y - minY->y) / 2.f;
  std::println("DiffY {}", diffY);

  std::vector<sf::Vector2f> translatedPoints = points;
  std::for_each(std::begin(translatedPoints), std::end(translatedPoints), [diffX, diffY](sf::Vector2f& point){
    // Translate points
    point.x -= diffX;
    point.y -= diffY;
  });

  // It is possible to extend earcut with sf::Vector, TODO: maybe do it later?
  std::vector<std::vector<std::pair<float, float>>> polygon;
  polygon.push_back({});
  std::for_each(std::begin(translatedPoints), std::end(translatedPoints), [&polygon](const sf::Vector2f& point){
    polygon[0].push_back({point.x, point.y});
  });

  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = options.density;

  std::vector<size_t> indices = mapbox::earcut<size_t>(polygon);
  assert(indices.size() % 3 == 0);
  std::println("Mapbox::earcut");
  for (size_t idx = 0; idx < indices.size(); idx += 3)
  {
    std::array<b2Vec2, 3> hullData = {b2Vec2{translatedPoints[indices[idx + 0]].x / PPM, translatedPoints[indices[idx + 0]].y / PPM},
                                      b2Vec2{translatedPoints[indices[idx + 1]].x / PPM, translatedPoints[indices[idx + 1]].y / PPM},
                                      b2Vec2{translatedPoints[indices[idx + 2]].x / PPM, translatedPoints[indices[idx + 2]].y / PPM}};
    b2Hull hull = b2ComputeHull(hullData.data(), hullData.size());
    b2Polygon triangle = b2MakePolygon(&hull, 0.f);

    m_shapeIds.push_back(b2CreatePolygonShape(m_bodyId, &shapeDef, &triangle));

    sf::ConvexShape sfmlTriangle {3};
    sfmlTriangle.setPoint(0, {translatedPoints[indices[idx + 0]].x, translatedPoints[indices[idx + 0]].y});
    sfmlTriangle.setPoint(1, {translatedPoints[indices[idx + 1]].x, translatedPoints[indices[idx + 1]].y});
    sfmlTriangle.setPoint(2, {translatedPoints[indices[idx + 2]].x, translatedPoints[indices[idx + 2]].y});
    sfmlTriangle.setFillColor(sf::Color{120, 120, 120, 255});
    sfmlTriangle.setPosition(pos);
    m_sfmlShapes.push_back(std::move(sfmlTriangle));
  }
}

Polygon::~Polygon()
{
  for (const auto& shapeId : m_shapeIds)
  {
    b2DestroyShape(shapeId, false);
  }
}

void Polygon::update()
{
  b2Vec2 bodyPos = b2Body_GetPosition(m_bodyId);
  // for (const auto& triangle : m_sfmlShapes)
  // {
  //   triangle.setPosition({bodyPos.x * PPM})
  // }
  // m_polygonShape.setPosition({bodyPos.x * PPM, bodyPos.y * PPM});
}

void Polygon::draw(sf::RenderWindow& window)
{
  for (const auto& triangle : m_sfmlShapes)
  {
    window.draw(triangle);
  }
}

} // namespace
