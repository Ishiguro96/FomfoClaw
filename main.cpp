#include <print>
#include <optional>

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

// This is a constant to convert from Box2D's units (meters)
// to SFML pixels (pixels per meter).
constexpr size_t PPM = 100U;

constexpr size_t WINDOW_WIDTH = 1920;
constexpr size_t WINDOW_HEIGHT = 1080;

int main()
{
  sf::RenderWindow window{sf::VideoMode{{WINDOW_WIDTH, WINDOW_HEIGHT}}, "FomfoClaw"};

  if (not ImGui::SFML::Init(window))
  {
    return -1;
  }

  sf::Clock m_deltaClock;

  const auto onCloseEvent = [&window](const sf::Event::Closed&)
  {
    window.close();
  };

  // Box2D
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = b2Vec2{0.f, -10.f};

  b2WorldId worldId = b2CreateWorld(&worldDef);

  b2BodyDef groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position = b2Vec2{10.f, 1.5f};
  b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

  b2Polygon groundBox = b2MakeBox(10.f, 2.f);
  b2ShapeDef groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

  sf::RectangleShape groundRect{{10.f * PPM, 2.f * PPM}};
  groundRect.setOrigin({groundRect.getLocalBounds().size.x / 2.f, groundRect.getLocalBounds().size.y / 2.f});
  groundRect.setFillColor(sf::Color::Cyan);

  window.resetGLStates();
  while(window.isOpen())
  {
    window.clear(sf::Color{80, 80, 80});

    // Handle all window's events
    window.handleEvents(onCloseEvent);
    while(const auto event = window.pollEvent()){
      if (event.has_value())
      {
        // Dereferencing `event` when it has no value is UB.
        ImGui::SFML::ProcessEvent(window, *event);
      }
    }

    // Update ImGui
    ImGui::SFML::Update(window, m_deltaClock.restart());

    // Draw Box2D objects
    b2Vec2 groundPos = b2Body_GetPosition(groundId);
    groundRect.setPosition({groundPos.x * PPM, WINDOW_HEIGHT - groundPos.y * PPM});
    window.draw(groundRect);

    // Display window
    
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
