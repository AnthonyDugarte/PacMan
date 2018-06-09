# include <Scene.hpp>

Scene::Scene(Window & l_window, Type l_type)
  : m_window(l_window), m_type(l_type)
{
  // empty
}

Scene::~Scene()
{
  // empty
}

const Scene::Type & Scene::getType() const
{
  return m_type;
}

const sf::Time & Scene::getElapsed() const
{
  return m_elapsed;
}

void Scene::restartClock()
{
  m_elapsed = m_clock.restart();
}
