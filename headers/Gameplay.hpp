# pragma once

# include <Scene.hpp>
# include <Player.hpp>
# include <TileMap.hpp>
# include <Food.hpp>

class Gameplay : public Scene
{
public:
  Gameplay (Window &);

  Type run () override;

private:
  TileMap m_map;
  Food m_food;
  Player m_pacman;
};
