# pragma once

# include <Animation.hpp>
# include <Entity.hpp>

class Player : public Entity
{
public:
  //  @Param1: path to animations folder
  Player (const std::string & path, sf::Vector2f initPos);

  void update (const sf::Time & dt, TileMap & map);

private:
  void updateAnimation (const sf::Time & dt);
  void updatedRotation ();

  Animation m_walking;
  Animation m_dying;

  sf::Vector2f m_initPos;
};
