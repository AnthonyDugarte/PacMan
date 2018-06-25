# pragma once

# include <Scene.hpp>
# include <Player.hpp>
# include <TileMap.hpp>
# include <Food.hpp>
# include <Ghost.hpp>
# include <Globals.hpp>

# include <HUD.hpp>

class Gameplay : public Scene
{
public:
  Gameplay (Window & window);
  ~Gameplay ();

  Type run () override;

private:
  void updateMembers ();
  void drawMembers ();

  TileMap m_map;
  Food m_food;
  Player m_pacman;
  Ghost m_blueGhost, m_pinkGhost, m_redGhost, m_yellowGhost;
  HUD m_hud;

  sf::Music *m_eatingMusic, *m_deadMusic, *m_worldMusic, *m_deadGhostMusic;

  sf::Time m_deadTime = sf::seconds(2.5f);
  bool m_deadPacman = false;
};
