# pragma once

# include <AssetManager.hpp>
# include <Animation.hpp>
class Animator
{
public:
  Animator ();
  Animator (const Animator & copy);

  ~Animator ();

  // @Parama 1: Animation name and folder on assets.
  void createAnimation (const std::string & name, const sf::Time & duration, bool looping);

  // @Param 2: Should animation start from the begining?
  bool switchAnimation (const std::string & name, bool reset = false);

  Animation * findAnimation (const std::string & name) const;
  const std::string & getCurrentAnimationName () const;

  // this one is the frame you want to place on your sprite
  sf::Texture & getFrame () const;

  /* @Param 1: Animation name
   * @Param 2: Path to it's folder
   */
  void addFrames(const std::string & name, const std::string & path);

  virtual void update (const sf::Time & dt);

private:
  void switchAnimation (Animation & newAnimation, const std::string & name, bool reset);

  Animation * m_current;
  std::string m_currentName;

  std::map<std::string, Animation *> m_animations;
};
