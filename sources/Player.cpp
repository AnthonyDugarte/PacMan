# include <Player.hpp>

Player::Player (const std::string & animPack, sf::Vector2f initPos)
: m_walking(sf::seconds(3.f / 16), true), // it'll be looping
  m_dying(sf::seconds(1.5f), false),
  m_initPos(initPos)
{
  m_walking.addFrames(animPack + "/walking");
  m_dying.addFrames(animPack + "/dying");

  setTexture(m_walking.getFrame(0), true);

  float scale = 14.f / 65.f;
  setScale(scale, scale);
  setOrigin(Helpers::getSpriteLocalCenter(*this));
  setRotation(-90);
  setPosition(m_initPos);
}

void Player::update (const sf::Time & dt, TileMap & map)
{
  if(recentlyAttacked())
  {
    m_pad.resetAx(); // not moving
    setRotation(getRotation() + 90); // m_dying animation is rotated
  }

  if(not attackable() and not dead() and m_dying.getElapsed() - sf::seconds(1) > m_dying.getDuration())
  {
    restoreAttackable();
    setRotation(-90); // restore m_dying rotation
    setPosition(m_initPos);

    m_dying.reset();
  }

  // for(size_t i { 4 }; --i; healted()); // restore lifes

  if(attackable())
    Entity::update(dt, &map);

  updateAnimation(dt);
}

void Player::updateAnimation (const sf::Time & dt)
{
  updatedRotation();

  if(moving())
  {
    m_walking.update(dt);
    setTexture(m_walking.getFrame(), true);
  }
  else if(not attackable())
  {
    setTexture(m_dying.getFrame(), true);
    m_dying.update(dt);
  }
  else setTexture(m_walking.getFrame(0), true); // not moving
}

void Player::updatedRotation ()
{
  if(m_pad.W)
    setRotation(-90.f);
  else if(m_pad.S)
    setRotation(90.f);
  else if(m_pad.A)
    setRotation(180.f);
  else if(m_pad.D)
    setRotation(0.f);
}
