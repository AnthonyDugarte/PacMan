# pragma once

# include <string>
# include <SFML/Graphics.hpp>

class Window : public sf::RenderWindow
{
public:
  Window();
  Window(const std::string & l_title, const sf::Vector2u & l_size);

  ~Window();

  void beginRender(const sf::Color & BGcolor = sf::Color::Black);
  void endRender(); // display changes

  void handleEvent(const sf::Event &);

  const bool & isDone() const;

  bool isFullScreen() const;
  void toggleFullScreen();

  sf::Vector2u 	getSize () const override;

private:
  void __setup(const std::string & l_title, const sf::Vector2u & l_size);
  void __create();

  sf::Vector2u m_windowSize;
  std::string m_windowTitle;
  bool m_isDone;
  bool m_isFullScreen;
};
