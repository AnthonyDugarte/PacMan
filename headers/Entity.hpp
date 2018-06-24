# pragma once

# include <TileMap.hpp>
# include <WSAD.hpp>
# include <Helpers.hpp>

# include <Attributes.hpp>

class Entity : public sf::Sprite, public Attributes
{
public:
  bool moving () const;

  virtual void update (const sf::Time & dt, TileMap &map);
protected:
  float m_velocity = 100.f;
  WSAD m_pad;

  virtual void moveUp (const sf::Time & dt);
  virtual void moveDown (const sf::Time & dt);
  virtual void moveLeft (const sf::Time & dt);
  virtual void moveRight (const sf::Time & dt);
};
