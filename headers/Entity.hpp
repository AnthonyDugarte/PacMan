# pragma once

# include <TileMap.hpp>
# include <WSAD.hpp>
# include <Helpers.hpp>

# include <Attributes.hpp>

class Entity : public sf::Sprite, public Attributes
{
public:
  Entity ()
  {
    // empty
  }

  bool moving () const;

  virtual void update (const sf::Time &, TileMap * = nullptr);
protected:
  float m_velocity = 100.f;
  WSAD pad;

  virtual void moveUp (const sf::Time &);
  virtual void moveDown (const sf::Time &);
  virtual void moveLeft (const sf::Time &);
  virtual void moveRight (const sf::Time &);
};
