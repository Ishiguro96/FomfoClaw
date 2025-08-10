#include "Claw.hpp"

#include "Constants/SfmlConstants.hpp"
#include "Shapes/Rect.hpp"
#include "Shapes/Circle.hpp"

#include <memory>
#include <limits>
#include <print>

using namespace ish::constants;

using namespace ish::game_objects;

Claw::Claw(b2WorldId worldId)
{
  enum CollisionBits : size_t {
    GROUND = (1U << 0U),
    CLAW = (1U << 1U),
    OTHER = (1U << 2U),

    ALL = std::numeric_limits<std::underlying_type_t<CollisionBits>>::max()
  };

  m_shapes.push_back(std::move(std::make_unique<shapes::Circle>(sf::Vector2f{WINDOW_WIDTH / 2.f, 50.f}, 10.f, worldId, shapes::Shape::Options {.type=shapes::Shape::TYPE::KINEMATIC})));
  m_shapes.push_back(std::move(std::make_unique<shapes::Rect>(sf::Vector2f{WINDOW_WIDTH / 2.f, 100.f}, sf::Vector2f{20.f, 100.f}, worldId)));
  m_shapes.push_back(std::move(std::make_unique<shapes::Rect>(sf::Vector2f{WINDOW_WIDTH / 2.f, 0.f}, sf::Vector2f{20.f, 100.f}, worldId)));
  m_shapes.push_back(std::move(std::make_unique<shapes::Rect>(sf::Vector2f{WINDOW_WIDTH / 2.f, -100.f}, sf::Vector2f{20.f, 100.f}, worldId)));
  m_shapes.push_back(std::move(std::make_unique<shapes::Rect>(sf::Vector2f{WINDOW_WIDTH / 2.f,-200.f}, sf::Vector2f{20.f, 100.f}, worldId)));

  b2Filter filter;
  filter.categoryBits = CollisionBits::ALL;
  filter.maskBits = CollisionBits::GROUND | CollisionBits::OTHER;
  for (const auto& shape : m_shapes)
  {
    shape->setFilter(filter);
  }

  m_shapes.push_back(std::move(std::make_unique<shapes::Rect>(sf::Vector2f{WINDOW_WIDTH / 2, 600.f}, sf::Vector2f{80.f, 10.f}, worldId, shapes::Shape::Options{.type=shapes::Shape::TYPE::STATIC, .isSensor = true})));

  b2RevoluteJointDef jointDef1 = b2DefaultRevoluteJointDef();
  jointDef1.bodyIdA = m_shapes[0]->getBodyId();
  jointDef1.bodyIdB = m_shapes[1]->getBodyId();
  jointDef1.localAnchorB = {0.f, -40.f / PPM};
  jointDef1.enableMotor = true;
  jointDef1.enableLimit = true;
  jointDef1.motorSpeed = -1.f;
  jointDef1.maxMotorTorque = 10000.f;
  jointDef1.lowerAngle = sf::degrees(-45.f).asRadians();
  jointDef1.upperAngle = sf::degrees(45.f).asRadians();
  b2CreateRevoluteJoint(worldId, &jointDef1);

  b2RevoluteJointDef jointDef2 = b2DefaultRevoluteJointDef();
  jointDef2.bodyIdA = m_shapes[0]->getBodyId();
  jointDef2.bodyIdB = m_shapes[2]->getBodyId();
  jointDef2.localAnchorB = {0.f, -40.f / PPM};
  jointDef2.enableMotor = true;
  jointDef2.enableLimit = true;
  jointDef2.motorSpeed = 1.f;
  jointDef2.maxMotorTorque = 1000.f;
  jointDef2.lowerAngle = sf::degrees(-45.f).asRadians();
  jointDef2.upperAngle = sf::degrees(45.f).asRadians();
  b2CreateRevoluteJoint(worldId, &jointDef2);

  b2RevoluteJointDef jointDef3 = b2DefaultRevoluteJointDef();
  jointDef3.bodyIdA = m_shapes[1]->getBodyId();
  jointDef3.bodyIdB = m_shapes[3]->getBodyId();
  jointDef3.localAnchorA = {0.f, 50.f / PPM};
  jointDef3.localAnchorB = {0.f, -50.f / PPM};
  jointDef3.enableMotor = true;
  jointDef3.enableLimit = true;
  jointDef3.motorSpeed = 1.f;
  jointDef3.maxMotorTorque = 1000.f;
  jointDef3.lowerAngle = sf::degrees(-45.f).asRadians();
  jointDef3.upperAngle = sf::degrees(45.f).asRadians();
  b2CreateRevoluteJoint(worldId, &jointDef3);

  b2RevoluteJointDef jointDef4 = b2DefaultRevoluteJointDef();
  jointDef4.bodyIdA = m_shapes[2]->getBodyId();
  jointDef4.bodyIdB = m_shapes[4]->getBodyId();
  jointDef4.localAnchorA = {0.f, 50.f / PPM};
  jointDef4.localAnchorB = {0.f, -50.f / PPM};
  jointDef4.enableMotor = true;
  jointDef4.enableLimit = true;
  jointDef4.motorSpeed = -1.f;
  jointDef4.maxMotorTorque = 1000.f;
  jointDef4.lowerAngle = sf::degrees(-45.f).asRadians();
  jointDef4.upperAngle = sf::degrees(45.f).asRadians();
  b2CreateRevoluteJoint(worldId, &jointDef4);

  b2Body_SetLinearVelocity(m_shapes[0]->getBodyId(), b2Vec2{0.f, 2.f});
}

void Claw::update(b2WorldId worldId)
{
  b2SensorEvents events = b2World_GetSensorEvents(worldId);
  for (size_t i = 0; i < events.beginCount; ++i)
  {
    if (B2_ID_EQUALS(events.beginEvents[i].sensorShapeId, m_shapes[5]->getBodyId()))
    {
      std::println("Sensor activated");
    }
  }

  for (size_t i = 0; i < events.endCount; ++i)
  {
    if (B2_ID_EQUALS(events.endEvents[i].sensorShapeId, m_shapes[5]->getBodyId()))
    {
      std::println("Sensor deactivated");
    }
  }
}
