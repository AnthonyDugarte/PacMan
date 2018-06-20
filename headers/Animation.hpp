# pragma once

# include <AssetManager.hpp>

//Animation has the animation's frames, it's duration and it's loop conditional.
// if the animation is looping, it means that the animation is going to repite itself
// in the pass of time but, if it is not looping, it is going to show it last frame after the
// total duration has been reached

class Animation
{
public:
  // constructor takes as parameters the total time to get to the last frame
  // and if it is going to loop
  Animation(const sf::Time & duration, bool looping = false);
  // copy cstor
  Animation (const Animation & cpy);
  ~Animation ();

  //sets animation time elapsed to zero
  void reset ();

  // methods identifiers are pretty clear I believe
  void setDuration (const sf::Time & duration);
  void setLooping (bool looping);

  // current animation frame, the one our sprites are going to use
  sf::Texture & getFrame () const;
  sf::Texture & getFrame (size_t pos) const;

  const sf::Time & getDuration () const;
  const sf::Time & getElapsed () const;
  bool getLooping () const;
  size_t size () const;

  // it's meaning is to work as a manual setter of frames getting as parameter
  // the complete path of it's texture on assets folder
  void addFrame (const std::string & path);

  // path to the folder containing the animation's folder with it's frames and .info
  void addFrames (const std::string & pre_path);

  // @note: This method is setted virtual so we can define different types of interactions
  // from derived classes
  virtual void update (const sf::Time & dt);

private:
  std::vector<sf::Texture *> m_frames;
  sf::Time m_duration;
  bool m_looping;

  sf::Time m_elapsed = sf::Time::Zero;
  size_t m_pos = 0;
};
