# include <Gameplay.hpp>

Gameplay::Gameplay (Window & window)
: Scene(window, Scene::Type::gameplay),
  m_map("16x16"),
  m_pacman("Pacman", sf::Vector2f(224,376)),
  m_food(m_map)
{
  m_food.eatFood(sf::Vector2f(224,376));
  m_food.eatFood(sf::Vector2f(223,376));
}

Scene::Type Gameplay::run ()
{
  restartClock();
  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
    {
      m_window.handleEvent(event);

      if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space)
        m_pacman.attacked();
    }

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
