# include <SceneManager.hpp>

SceneManager::SceneManager ()
: m_window("Pacman", sf::Vector2u(28 * 16, 31 * 16)),
  m_type(Scene::Type::gameplay)
{
  // empty
}

void SceneManager::startGame ()
{
  while(m_type != Scene::Type::end) switch(m_type)
  {
    case Scene::Type::gameplay:
      m_type = Gameplay(m_window).run();
    break;

    case Scene::Type::end:
    default:
    break;
  }
}
