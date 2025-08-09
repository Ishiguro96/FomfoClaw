#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace ish::shapes {

class Shape
{
  public:
    enum class TYPE : uint8_t
    {
      STATIC, KINEMATIC, DYNAMIC
    };

    struct Options
    {
      TYPE type = TYPE::DYNAMIC;
      float density = 1.f;
    };

    Shape() = default;
    virtual ~Shape();

    Shape(const Shape& other) = delete;
    Shape& operator=(const Shape& other) = delete;

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    b2BodyId getBodyId() { return m_bodyId; }

    bool operator == (const Shape& other)
    {
      uint64_t compareValueLhs = static_cast<uint64_t>(this->m_bodyId.index1) << 32U | static_cast<uint64_t>(this->m_bodyId.world0 << 16U) | this->m_bodyId.generation;
      uint64_t compareValueRhs = static_cast<uint64_t>(other.m_bodyId.index1) << 32U | static_cast<uint64_t>(other.m_bodyId.world0 << 16U) | other.m_bodyId.generation;
      return (compareValueLhs == compareValueRhs);
    }

    struct Hash
    {
      uint64_t operator() (const Shape& shape)
      {
        return static_cast<uint64_t>(shape.m_bodyId.index1) << 32U | static_cast<uint64_t>(shape.m_bodyId.world0) << 16U | shape.m_bodyId.generation;
      }
    };

  protected:
    b2BodyId m_bodyId;
    static constexpr float PPM = 30.f;
};

} // namespace
