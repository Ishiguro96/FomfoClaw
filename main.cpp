#include <print>
#include <optional>

#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window{sf::VideoMode{{1920, 1080}, 32}, "FomfoClaw"};

  const auto onCloseEvent = [&window](const sf::Event::Closed&)
  {
    window.close();
  };

  while(window.isOpen())
  {
    window.handleEvents(onCloseEvent);

    window.clear(sf::Color{80, 80, 80});
    window.display();
  }

  return 0;
}
