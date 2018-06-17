# pragma once

# include <TileMap.hpp>
# include <Helpers.hpp>

class Player : public sf::Sprite
{
public:
  Player ();
 	Player (const sf::Texture & texture, const sf::IntRect & rectangle);

  void update (const sf::Time &, TileMap * = nullptr);

private:
  float m_velocity = 100.f;

  void moveUp (const sf::Time &);
  void moveDown (const sf::Time &);
  void moveLeft (const sf::Time &);
  void moveRight (const sf::Time &);

  WSAD pad;
};
