# include <Entity.hpp>

bool Entity::moving () const
{
  return pad.rPressed() != 0;
}

void Entity::update (const sf::Time & dt, TileMap * map)
{
  pad.update();
  if(map)
    map->updateWSAD(pad, getPosition());

  if(pad.W)
  {
    moveUp(dt);

    if(map and Helpers::hasCollision(*this, *map))
    {
      pad.W = false;
      moveUp(-dt);
    }
  }
  else if(pad.S)
  {
    moveDown(dt);

    if(map and Helpers::hasCollision(*this, *map))
    {
      pad.S = false;
      moveDown(-dt);
    }
  }

  if(pad.A)
  {
    moveLeft(dt);

    if(map and Helpers::hasCollision(*this, *map))
    {
      pad.A = false;
      moveLeft(-dt);
    }
  }
  else if(pad.D)
  {
    moveRight(dt);

    if(map and Helpers::hasCollision(*this, *map))
    {
      pad.D = false;
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
