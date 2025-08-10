#include <print>
#include <optional>
#include <unordered_set>
#include <memory>
#include <cmath>

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>

#include "Shapes/Shape.hpp"
#include "Shapes/Rect.hpp"
#include "Shapes/Circle.hpp"
#include "Shapes/Polygon.hpp"

#include "GameObjects/Claw.hpp"

#include "Constants/SfmlConstants.hpp"

using namespace ish;
using namespace ish::constants;

int main()
{
  sf::RenderWindow window{sf::VideoMode{{WINDOW_WIDTH, WINDOW_HEIGHT}}, "FomfoClaw"};

  if (not ImGui::SFML::Init(window))
  {
    return -1;
  }

  sf::View view{sf::FloatRect{{0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT}}};
  window.setView(view);

  float zoomFactor = 1.f;

  b2DebugDraw debugDraw = b2DefaultDebugDraw();
  auto contextToPass = std::make_pair(&window, zoomFactor);
  debugDraw.context = &contextToPass;
  debugDraw.drawShapes = true;
  debugDraw.drawJoints = true;
  debugDraw.DrawPointFcn = [](b2Vec2 p, float size, b2HexColor color, void* context) {
    assert(context);
    auto ctx = static_cast<std::pair<sf::RenderWindow*, float>*>(context);

    sf::CircleShape point {size};
    point.setOrigin({point.getGlobalBounds().size.x / 2.f, point.getGlobalBounds().size.x / 2.f});
    point.setPosition({p.x * PPM, p.y * PPM});
    point.setFillColor(sf::Color::Transparent);
    point.setOutlineColor(sf::Color::Magenta);

    point.setOutlineThickness(std::floor(-1.f / ctx->second));
    ctx->first->draw(point);
  };
  debugDraw.DrawSolidPolygonFcn = [](b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context) {
    assert(context);
    auto ctx = static_cast<std::pair<sf::RenderWindow*, float>*>(context);

    sf::ConvexShape polygon{static_cast<size_t>(vertexCount)};
    for (int i = 0; i < vertexCount; ++i) {
      polygon.setPoint(i, {vertices[i].x * PPM, vertices[i].y * PPM});
    }

    polygon.setPosition({transform.p.x * PPM, transform.p.y * PPM});
    polygon.setRotation(sf::radians(b2Rot_GetAngle(transform.q)));
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(sf::Color::Cyan);
    polygon.setOutlineThickness(std::floor(-1.f / ctx->second));
    ctx->first->draw(polygon);
  };
  debugDraw.DrawSolidCircleFcn = [](b2Transform transform, float radius, b2HexColor color, void* context) {
    assert(context);
    auto ctx = static_cast<std::pair<sf::RenderWindow*, float>*>(context);

    sf::CircleShape circle(radius * PPM);
    circle.setPosition({transform.p.x * PPM, transform.p.y * PPM});
    circle.setOutlineThickness(std::floor(-1.f / ctx->second));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Yellow);
    circle.setOrigin({circle.getLocalBounds().size.x / 2.f, circle.getLocalBounds().size.y / 2.f});
    ctx->first->draw(circle);
  };

  debugDraw.DrawSegmentFcn = [](b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context){
    assert(context);
    auto ctx = static_cast<std::pair<sf::RenderWindow*, float>*>(context);

    sf::VertexArray lines(sf::PrimitiveType::LineStrip, 2);
    lines[0].position = {p1.x * PPM, p1.y * PPM};
    lines[1].position = {p2.x * PPM, p2.y * PPM};
    ctx->first->draw(lines);
  };

  sf::Clock m_deltaClock;

  // Box2D
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = b2Vec2{0.f, 10.f};

  b2WorldId worldId = b2CreateWorld(&worldDef);

  const auto onCloseEvent = [&window](const sf::Event::Closed&)
  {
    window.close();
  };

  std::unordered_set<std::unique_ptr<shapes::Shape>> dynamicShapes;

  bool keyLatch {false};
  const auto onKeyPressed = [=, &window, &keyLatch, &dynamicShapes](const sf::Event::KeyPressed& key){
    if (key.scancode == sf::Keyboard::Scancode::G && !keyLatch) {
      //keyLatch = true;

      auto shapePtr = std::make_unique<shapes::Circle>(
        sf::Vector2f{WINDOW_WIDTH / 2.f, 0.f},
        15.f,
        worldId
      );

      dynamicShapes.insert(std::move(shapePtr));
    }
  };
  const auto onKeyReleased = [&window, &keyLatch](const sf::Event::KeyReleased& key){
    if (key.scancode == sf::Keyboard::Scancode::G && keyLatch) {
      keyLatch = false;
    }
  };

  constexpr float groundWidth = 100.f;
  shapes::Polygon groundBox {
    {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f},
    {
      {0.f, 0.f},
      {groundWidth, 0.f},
      {groundWidth, WINDOW_HEIGHT - 200.f},
      {WINDOW_WIDTH - 200.f, WINDOW_HEIGHT - 200.f},
      {WINDOW_WIDTH - 200.f, 0.f},
      {WINDOW_WIDTH - 200.f + groundWidth, 0.f},
      {WINDOW_WIDTH - 200.f + groundWidth, WINDOW_HEIGHT - 200.f + groundWidth},
      {0.f, WINDOW_HEIGHT - 200.f + groundWidth}
    },
    worldId, shapes::Shape::Options{shapes::Shape::TYPE::STATIC}
  };

  bool updatePhysics {false};
  bool mousePressed {false};
  const auto onMousePressed = [&mousePressed](const sf::Event::MouseButtonPressed& button) {
    if (button.button == sf::Mouse::Button::Right)
    {
      mousePressed = true;
    }
  };
  const auto onMouseReleased = [&mousePressed](const sf::Event::MouseButtonReleased& button) {
    if (button.button == sf::Mouse::Button::Right)
    {
      mousePressed = false;
    }
  };
  const auto onMouseMovedRaw = [&mousePressed, &window, &view](const sf::Event::MouseMovedRaw& newPos) {
    if (mousePressed)
    {
      view.setCenter(view.getCenter() - static_cast<sf::Vector2f>(newPos.delta * 3));
      window.setView(view);
    }
  };
  const auto onMouseScroll = [&zoomFactor, &view, &window](const sf::Event::MouseWheelScrolled& scroll) {
    if ((zoomFactor > 0.1f && scroll.delta < 0) || (zoomFactor < 4.f && scroll.delta > 0)) {
      zoomFactor += (scroll.delta / 25.f);
      std::println("{} {}", scroll.delta, zoomFactor);
      view.setSize(static_cast<sf::Vector2f>(window.getSize()) * zoomFactor);
      window.setView(view);
    }
  };

  // Game objects
  game_objects::Claw claw {worldId};

  window.resetGLStates();
  while(window.isOpen())
  {
    updatePhysics = false;

    // Update physics with 60 FPS
    if (m_deltaClock.getElapsedTime().asMilliseconds() > 1000.f / 60.f) {
      m_deltaClock.restart();
      updatePhysics = true;
    }

    window.clear(sf::Color{80, 80, 80});

    // Handle all window's events
    window.handleEvents(onCloseEvent, onKeyPressed, onKeyReleased, onMousePressed, onMouseReleased, onMouseMovedRaw, onMouseScroll);
    while(const auto event = window.pollEvent()){
      if (event.has_value())
      {
        // Dereferencing `event` when it has no value is UB.
        ImGui::SFML::ProcessEvent(window, *event);
      }
    }

    // Update ImGui
    ImGui::SFML::Update(window, m_deltaClock.getElapsedTime());

    if (updatePhysics) {
      b2World_Step(worldId, 1.f / 60.f, 4);
    }

    // Update shapes
    groundBox.update();

    claw.update(worldId);

    // // Draw shapes
    groundBox.draw(window);

    b2World_Draw(worldId, &debugDraw);

    // Display window
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
