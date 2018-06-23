# include <Gameplay.hpp>

Gameplay::Gameplay (Window & window)
: Scene(window, Scene::Type::gameplay),
  m_map("16x16"),
  m_pacman("Pacman", sf::Vector2f(224,376)),
  m_food(m_map),
  m_blueGhost("LightBlue", sf::Vector2f(216, 184), sf::Vector2f(202, 232)),
  m_pinkGhost("Pink", sf::Vector2f(216, 184), sf::Vector2f(216, 232)),
  m_redGhost("Red", sf::Vector2f(232, 184), sf::Vector2f(232, 232)),
  m_yellowGhost("Yellow", sf::Vector2f(232, 184), sf::Vector2f(250, 232)),
  m_hud(m_pacman)
{
  m_hud.fitInWindow(m_window);
}

Scene::Type Gameplay::run ()
{
  restartClock();

  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
    {
      m_window.handleEvent(event); // window stuff

      if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)
        return Scene::Type::menu;
    }

    if(m_pacman.dead() or m_food.over())
    {
      Globals::Score() = m_hud.getScore();

      if(m_food.over())
        return Scene::Type::gameWon;
      return Scene::Type::gameOver;
    }


    updateMembers();

    m_window.beginRender(sf::Color::Black);
    drawMembers();
    m_window.endRender();

    restartClock();
  }

  return Scene::Type::end;
}

void Gameplay::updateMembers ()
{
  // pacman stuff
  m_pacman.update(getElapsed(), m_map);
  m_hud.update(m_food.eatFood(m_pacman.getPosition()));
  int ghostPoints = 600;

  if(m_food.eatenSpecialFood())
  {
    m_blueGhost.restoreAttackable();
    m_pinkGhost.restoreAttackable();
    m_redGhost.restoreAttackable();
    m_yellowGhost.restoreAttackable();
  }

  // ghosts stuff
  m_blueGhost.update(getElapsed(), m_map, m_pacman);
  m_pinkGhost.update(getElapsed(), m_map, m_pacman);
  m_redGhost.update(getElapsed(), m_map, m_pacman);
  m_yellowGhost.update(getElapsed(), m_map, m_pacman);

  // trying to kill pacman or blue ghost
  if(not (m_pacman.dead() or m_blueGhost.dead()) and
    (m_pacman.attackable() or m_blueGhost.attackable()) and
    Helpers::hasCollision(m_blueGhost, m_pacman, 254))
  {
    if(m_blueGhost.attackable())
    {
      m_blueGhost.attacked();
      m_hud.update(ghostPoints);
    }
    else if(m_pacman.attackable())
      m_pacman.attacked();
  }
  // trying to kill pacman or pink ghost
  else if(not (m_pacman.dead() or m_pinkGhost.dead()) and
    (m_pacman.attackable() or m_pinkGhost.attackable()) and
    Helpers::hasCollision(m_pinkGhost, m_pacman, 254))
  {
    if(m_pinkGhost.attackable())
    {
      m_pinkGhost.attacked();
      m_hud.update(ghostPoints);
    }
    else if(m_pacman.attackable())
      m_pacman.attacked();
  }
  // trying to kill pacman or red ghost
  else if(not (m_pacman.dead() or m_redGhost.dead()) and
    (m_pacman.attackable() or m_redGhost.attackable()) and
    Helpers::hasCollision(m_redGhost, m_pacman, 254))
  {
    if(m_redGhost.attackable())
    {
      m_redGhost.attacked();
      m_hud.update(ghostPoints);
    }
    else if(m_pacman.attackable())
      m_pacman.attacked();
  }
  // trying to kill pacman or yellow ghost
  else if(not (m_pacman.dead() or m_yellowGhost.dead()) and
    (m_pacman.attackable() or m_yellowGhost.attackable()) and
    Helpers::hasCollision(m_yellowGhost, m_pacman, 254))
  {
    if(m_yellowGhost.attackable())
    {
      m_yellowGhost.attacked();
      m_hud.update(ghostPoints);
    }
    else if(m_pacman.attackable())
      m_pacman.attacked();
  }
}

void Gameplay::drawMembers ()
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

  // HUD in action
  m_window.draw(m_hud);
}
