# include <SceneManager.hpp>

SceneManager::SceneManager ()
: m_window("Pacman", sf::Vector2u(28 * 16, 31 * 16)),
  m_type(Scene::Type::menu)
{
  // empty
}

void SceneManager::startGame ()
{
  while(m_type != Scene::Type::end) switch(m_type)
  {
    case Scene::Type::menu:
      m_type = Menu(m_window).run();
    break;

    case Scene::Type::gameplay:
      m_type = Gameplay(m_window).run();
    break;

    case Scene::Type::gameOver:
      m_type = GameEnd(m_window, "Game Over").run();
    break;

    case Scene::Type::gameWon:
      m_type = GameEnd(m_window, "You Won").run();
    break;

    case Scene::Type::end:
    default:
    break;
  }
}
