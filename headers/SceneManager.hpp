# pragma once

# include <Gameplay.hpp>

# include <Scene.hpp>

class SceneManager
{
public:
  SceneManager ();

  void startGame ();

private:
  Window m_window;

  Scene::Type m_type;
};
