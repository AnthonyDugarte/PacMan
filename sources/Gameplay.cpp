# include <Gameplay.hpp>

Gameplay::Gameplay (Window & window)
: Scene(window, Scene::Type::gameplay),
  m_map("16x16"),
  m_pacman("Pacman", sf::Vector2f(224,376)),
  m_food(m_map),
  m_blueGhost("LightBlue", sf::Vector2f(218, 184), sf::Vector2f(202, 232)),
  m_pinkGhost("Pink", sf::Vector2f(218, 184), sf::Vector2f(218, 232)),
  m_redGhost("Red", sf::Vector2f(234, 184), sf::Vector2f(234, 232)),
  m_yellowGhost("Yellow", sf::Vector2f(234, 184), sf::Vector2f(250, 232))
{
  // we eat the foot that is around pacman initialPos so he don't get points from them
  for(size_t x = 223; x <= 224; ++x)
    m_food.eatFood(sf::Vector2f(x, 376));
}

Scene::Type Gameplay::run ()
{
  restartClock();

  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
      m_window.handleEvent(event); // window stuff

    updateMembers();

    m_window.beginRender(sf::Color::Black);
    drawMembers();
    m_window.endRender();

    restartClock();
  }

  return Scene::Type::end;
}

void Gameplay::updateMembers()
{
  // pacman stuff
  m_pacman.update(getElapsed(), m_map);
  m_food.eatFood(m_pacman.getPosition());

  // ghosts stuff
  m_blueGhost.update(getElapsed(), m_map, m_pacman);
  m_pinkGhost.update(getElapsed(), m_map, m_pacman);
  m_redGhost.update(getElapsed(), m_map, m_pacman);
  m_yellowGhost.update(getElapsed(), m_map, m_pacman);

  // trying to kill pacman
  if(not m_pacman.dead() and m_pacman.attackable() and Helpers::hasCollision(m_blueGhost, m_pacman, 254))
    m_pacman.attacked();
  else if(not m_pacman.dead() and m_pacman.attackable() and Helpers::hasCollision(m_pinkGhost, m_pacman, 254))
    m_pacman.attacked();
  else if(not m_pacman.dead() and m_pacman.attackable() and Helpers::hasCollision(m_redGhost, m_pacman, 254))
    m_pacman.attacked();
  else if(not m_pacman.dead() and m_pacman.attackable() and Helpers::hasCollision(m_yellowGhost, m_pacman, 254))
    m_pacman.attacked();
}

void Gameplay::drawMembers()
{
  // we draw map first
  m_window.draw(m_map);

  //then the food
  m_window.draw(m_food);

  // then pacman, or shoud ghosts be drawn first? I don't know
  m_window.draw(m_pacman);

  // now ghosts xD
  m_window.draw(m_blueGhost);
  m_window.draw(m_pinkGhost);
  m_window.draw(m_redGhost);
  m_window.draw(m_yellowGhost);
}
