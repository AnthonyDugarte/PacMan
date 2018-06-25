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
  m_hud(m_pacman),
  m_deadMusic(&AssetManager::getMusic("pacman_death.wav", false)),
  m_worldMusic(&AssetManager::getMusic("ghosts_ambient.wav", true)),
  m_eatingMusic(&AssetManager::getMusic("wakka_wakka.wav", false)),
  m_deadGhostMusic(&AssetManager::getMusic("ghost_eaten.wav", false))
{
  m_worldMusic->setVolume(50.f);
  m_hud.fitInWindow(m_window);
}

Gameplay::~Gameplay ()
{
  m_deadMusic = m_worldMusic = m_eatingMusic = nullptr;
}

Scene::Type Gameplay::run ()
{
  m_worldMusic->play();

  restartClock();
  restartClock();

  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
    {
      m_window.handleEvent(event); // window stuff

      if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)
      {
        m_deadMusic->stop();
        m_worldMusic->stop();
        m_eatingMusic->stop();
        return Scene::Type::menu;
      }
    }

    if(m_food.over())
    {
      m_worldMusic->stop();
      m_eatingMusic->stop();
      Globals::Score() = m_hud.getScore();
      return Scene::Type::gameWon;
    }

    if(not m_pacman.attackable() and not m_deadPacman)
    {
      // if then is dead
      m_deadPacman = true;
      m_deadMusic->play();
    }

    if(m_deadTime < sf::Time::Zero)
    {
      if(m_pacman.dead())
      {
        m_worldMusic->stop();
        Globals::Score() = m_hud.getScore();
        return Scene::Type::gameOver;
      }
      else if(m_deadPacman)
      {
        m_deadTime = sf::seconds(2.5f);
        m_deadPacman = false;
      }
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
  if(m_food.justEaten())// and m_eatingMusic->getStatus() == sf::SoundSource::Stopped)
    m_eatingMusic->play();

  if(m_deadPacman)
    m_deadTime -= getElapsed();

  static int ghostPoints { 600 };

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
  static auto tryKill = [&](Ghost & ghost)->bool
  {
    if(not (m_pacman.dead() or ghost.dead())
        and (m_pacman.attackable() or ghost.attackable())
        and Helpers::hasCollision(ghost, m_pacman, 254))
    {
      if(ghost.attackable())
      {
        m_deadGhostMusic->play();
        ghost.attacked();
        m_hud.update(ghostPoints);
      }
      else if(m_pacman.attackable())
        m_pacman.attacked();
      return true;
    }
    return false;
  };
  tryKill(m_blueGhost);
  tryKill(m_pinkGhost);
  tryKill(m_redGhost);
  tryKill(m_yellowGhost);
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
