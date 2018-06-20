# pragma once

# include <Scene.hpp>
# include <Player.hpp>
# include <TileMap.hpp>
# include <Food.hpp>
# include <Ghost.hpp>

class Gameplay : public Scene
{
public:
  Gameplay (Window &);

  Type run () override;

private:
  void updateMembers();
  void drawMembers();

  TileMap m_map;
  Food m_food;
  Player m_pacman;
  Ghost blueGhost, pinkGhost, redGhost, yellowGhost;
};
