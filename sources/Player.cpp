# include <Player.hpp>

Player::Player ()
{
  // empty
}
Player::Player (const sf::Texture & texture, const sf::IntRect & rectangle)
  : sf::Sprite(texture, rectangle)
{
  // empty
}

void Player::update (const sf::Time & dt, TileMap * map_)
{
  pad.update();

  if(map_)
    map_->updateWSAD(pad, getPosition());

  if(pad.W)
  {
    moveUp(dt);

    if(map_ and Helpers::hasCollision(*this, *map_))
      moveUp(-dt);
  }
  else if(pad.S)
  {
    moveDown(dt);

    if(map_ and Helpers::hasCollision(*this, *map_))
      moveDown(-dt);
  }

  if(pad.A)
  {
    moveLeft(dt);

    if(map_ and Helpers::hasCollision(*this, *map_))
      moveLeft(-dt);
  }
  else if(pad.D)
  {
    moveRight(dt);

    if(map_ and Helpers::hasCollision(*this, *map_))
      moveRight(-dt);
  }
}

void Player::moveUp (const sf::Time & dt)
{
  move(0, -m_velocity * dt.asSeconds());
}

void Player::moveDown (const sf::Time & dt)
{
  move(0, m_velocity * dt.asSeconds());
}

void Player::moveLeft (const sf::Time & dt)
{
  move(-m_velocity * dt.asSeconds(), 0);
}

void Player::moveRight (const sf::Time & dt)
{
  move(m_velocity * dt.asSeconds(), 0);
}
