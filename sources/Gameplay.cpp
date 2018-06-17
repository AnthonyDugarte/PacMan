# include <Gameplay.hpp>

Gameplay::Gameplay (Window & window)
  : Scene(window, Scene::Type::gameplay),
    m_map("16x16"),
    m_food(m_map)
{
  // empty
}

Scene::Type Gameplay::run ()
{
  m_pacman.setTexture(AssetManager::getTexture("Pacman/walk0.png"), true);
  float scale = 14.f / 65.f;
  m_pacman.setScale(scale, scale);
  m_pacman.setOrigin(Helpers::getSpriteLocalCenter(m_pacman));
  m_pacman.setPosition(222,376);

  restartClock();
  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
      m_window.handleEvent(event);

    m_pacman.update(getElapsed(), &m_map);
    m_food.eatFood(m_pacman.getPosition());

    m_window.beginRender(sf::Color::Black);
    m_window.draw(m_map);
    m_window.draw(m_food);
    m_window.draw(m_pacman);
    m_window.endRender();

    restartClock();
  }

  return Scene::Type::end;
}
