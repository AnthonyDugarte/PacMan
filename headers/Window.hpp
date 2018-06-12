# pragma once

# include <AssetManager.hpp>

class Window : public sf::RenderWindow
{
public:
  Window (const std::string & l_title = "Pacman", const sf::Vector2u & l_size = sf::Vector2u(640, 480));

  ~Window ();

  void beginRender (const sf::Color & BGcolor = sf::Color::Black);
  void endRender (); // display changes

  void handleEvent (const sf::Event &);

  const bool & isDone () const;

  bool isFullScreen () const;
  void toggleFullScreen ();
  void screenShot () const;

  sf::Vector2u 	getSize  () const override;

private:
  void __create ();

  sf::Vector2u m_windowSize;
  std::string m_windowTitle;
  bool m_isFullScreen = false;
  bool m_isDone = false;
};
