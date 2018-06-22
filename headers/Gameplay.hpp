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

  Type run () override;

private:
  void updateMembers();
  void drawMembers();

  TileMap m_map;
  Food m_food;
  Player m_pacman;
  Ghost m_blueGhost, m_pinkGhost, m_redGhost, m_yellowGhost;

  HUD m_hud;
};
