# include <Player.hpp>

Player::Player (const std::string & animPack, sf::Vector2f initPos)
: walking(sf::seconds(3.f / 16), true), // it'll be looping
  dying(sf::seconds(1.5f), false),
  m_initPos(initPos)
{
  walking.addFrames(animPack + "/walking");
  dying.addFrames(animPack + "/dying");

  setTexture(walking.getFrame(0), true);

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
    pad.resetAx(); // not moving
    setRotation(getRotation() + 90); // dying animation is rotated
  }

  if(not attackable() and not dead() and dying.getElapsed() - sf::seconds(1) > dying.getDuration())
  {
    restoreAttackable();
    setRotation(-90); // restore dying rotation
    setPosition(m_initPos);

    dying.reset();
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
    walking.update(dt);
    setTexture(walking.getFrame(), true);
  }
  else if(not attackable())
  {
    setTexture(dying.getFrame(), true);
    dying.update(dt);
  }
  else setTexture(walking.getFrame(0), true); // not moving
}

void Player::updatedRotation ()
{
  if(pad.W)
    setRotation(-90.f);
  else if(pad.S)
    setRotation(90.f);
  else if(pad.A)
    setRotation(180.f);
  else if(pad.D)
    setRotation(0.f);
}
