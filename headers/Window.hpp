# pragma once

# include <AssetManager.hpp>

class Window : public sf::RenderWindow
{
public:
  // "pacmon for possible default construction forgotten"
  Window (const std::string & title = "Pacmom", const sf::Vector2u & size = sf::Vector2u(640, 480));
  ~Window ();

  void handleEvent (const sf::Event & event);

  //  this is a wrapper for clear() fuction
  void beginRender (const sf::Color & BGcolor = sf::Color::Black);
  // display() fuction wrapper
  void endRender ();

  const bool & isDone () const;
  bool isFullScreen () const;
  sf::Vector2u 	getSize () const override;

private:
  void __create ();

  void toggleFullScreen ();
  void screenShot ();

  sf::Vector2u m_windowSize;
  std::string m_windowTitle;
  bool m_isFullScreen = false;
  bool m_isDone = false;

  size_t m_capsN = 0; // number of captures
  bool screenShootTaken = false; // to know if we need to updated assets/Captures/ctrol.file
};
