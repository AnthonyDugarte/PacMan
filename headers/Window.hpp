# pragma once

# include <string>
# include <SFML/Graphics.hpp>

class Window
{
public:
  Window();
  Window(const std::string & l_title, const sf::Vector2u & l_size);

  ~Window();

  void beginRender(); // clear window
  void draw(sf::Drawable & l_drawable);
  void endRender(); // display changes

  void handleEvents();

  const bool & isDone() const;

  bool isFullScreen() const;
  void toggleFullScreen();

  const sf::Vector2u & getWindowSize() const;

private:
  void setup(const std::string & l_title, const sf::Vector2u & l_size);
  void destroy();
  void create();

  sf::RenderWindow m_window;
  sf::Vector2u m_windowSize;
  std::string m_windowTitle;
  bool m_isDone;
  bool m_isFullScreen;
};
