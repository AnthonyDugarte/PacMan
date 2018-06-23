# pragma once

# include <Scene.hpp>
# include <Globals.hpp>

class Menu : public Scene
{
  enum class Button { play, quit };
public:

  Menu (Window &);

  Type run () override;

private:
  Button m_currentButton;

  sf::Text m_playButton;
  sf::Text m_quitButton;
  sf::Text m_highSore;
  sf::RectangleShape m_selected;
};
