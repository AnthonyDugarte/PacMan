# pragma once

# include <Animation.hpp>
# include <Entity.hpp>

class Player : public Entity
{
public:
  Player (const std::string &, sf::Vector2f);

  void update (const sf::Time &, TileMap * = nullptr);

private:
  void updateAnimation (const sf::Time &);

  void moveUp (const sf::Time &);
  void moveDown (const sf::Time &);
  void moveLeft (const sf::Time &);
  void moveRight (const sf::Time &);

  Animation walking;
  Animation dying;

  sf::Vector2f m_initPos;
};
