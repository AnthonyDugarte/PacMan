# include <Entity.hpp>

bool Entity::moving () const
{
  return m_pad.rPressed() != 0;
}

void Entity::update (const sf::Time & dt, TileMap & map)
{
  m_pad.update();
  map.updateWSAD(m_pad, getPosition());

  if(m_pad.W)
  {
    moveUp(dt);

    if(Helpers::hasCollision(*this, map))
    {
      m_pad.W = false;
      moveUp(-dt);
    }
  }
  else if(m_pad.S)
  {
    moveDown(dt);

    if(Helpers::hasCollision(*this, map))
    {
      m_pad.S = false;
      moveDown(-dt);
    }
  }

  if(m_pad.A)
  {
    moveLeft(dt);

    if(Helpers::hasCollision(*this, map))
    {
      m_pad.A = false;
      moveLeft(-dt);
    }
  }
  else if(m_pad.D)
  {
    moveRight(dt);

    if(Helpers::hasCollision(*this, map))
    {
      m_pad.D = false;
      moveRight(-dt);
    }
  }
}

void Entity::moveUp (const sf::Time & dt)
{
  move(0, -m_velocity * dt.asSeconds());
}

void Entity::moveDown (const sf::Time & dt)
{
  move(0, m_velocity * dt.asSeconds());
}

void Entity::moveLeft (const sf::Time & dt)
{
  move(-m_velocity * dt.asSeconds(), 0);
}

void Entity::moveRight (const sf::Time & dt)
{
  move(m_velocity * dt.asSeconds(), 0);
}
