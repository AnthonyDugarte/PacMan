# pragma once

# include <Animation.hpp>
# include <Entity.hpp>

class Player : public Entity
{
public:
  Player (const std::string &, sf::Vector2f);

  void update (const sf::Time &, TileMap &);

private:
  void updateAnimation (const sf::Time &);
  void updatedRotation ();

  Animation walking;
  Animation dying;

  sf::Vector2f m_initPos;
};
