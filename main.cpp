#include <print>
#include <optional>

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window{sf::VideoMode{{1920, 1080}, 32}, "FomfoClaw"};

  if (not ImGui::SFML::Init(window))
  {
    return -1;
  }

  sf::Clock m_deltaClock;

  const auto onCloseEvent = [&window](const sf::Event::Closed&)
  {
    window.close();
  };

  window.resetGLStates();
  while(window.isOpen())
  {
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

    // Display window
    window.clear(sf::Color{80, 80, 80});
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
