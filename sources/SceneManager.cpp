# include <SceneManager.hpp>

SceneManager::SceneManager ()
  : m_window("Pacman", sf::Vector2u(28 * 40 * .4f, 31 * 40 * .4f)),
    type(Scene::Type::gameplay)
{
  // empty
}

void SceneManager::startGame ()
{
  while(type != Scene::Type::end) switch(type)
  {
    case Scene::Type::gameplay:
      type = Gameplay(m_window).run();
    break;

    case Scene::Type::end:
    default:
    break;
  }
}
