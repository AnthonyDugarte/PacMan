# pragma once

# include <Gameplay.hpp>
# include <GameEnd.hpp>

class SceneManager
{
public:
  SceneManager ();

  void startGame ();

private:
  Window m_window;

  Scene::Type m_type;
};
