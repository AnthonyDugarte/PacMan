# include <Animator.hpp>

Animator::Animator ()
: m_current(nullptr)
{
  // empty
}

Animator::Animator (const Animator & copy)
: m_current(nullptr)
{
  for(auto && animation : copy.m_animations)
    m_animations[animation.first] = new Animation(*animation.second);

  switchAnimation(copy.getCurrentAnimationName(), true);
}

Animator::~Animator ()
{
  // TODO: Optimize this, deleteting it and erasing it from the map
  for(auto && it : m_animations)
    delete it.second;
}

void Animator::createAnimation (const std::string & name, const sf::Time & duration, bool looping)
{
  if(m_animations.find(name) == m_animations.end())
    m_animations[name] = new Animation(duration, looping);

  if(m_current == nullptr)
    switchAnimation(*m_animations[name], name, true);
}

bool Animator::switchAnimation (const std::string & name, bool reset)
{
  auto && animation{ findAnimation(name) };

  if(animation)
  {
    switchAnimation(*animation, name, reset);
    return true;
  }

  return false;
}

Animation * Animator::findAnimation (const std::string & name) const
{
  auto && animation{ m_animations.find(name) };

  if(animation != m_animations.end())
    return &*animation->second;

  return nullptr;
}

const std::string & Animator::getCurrentAnimationName () const
{
  return m_currentName;
}

sf::Texture & Animator::getFrame () const
{
  return m_current->getFrame();
}

void Animator::addFrames(const std::string & name, const std::string & path)
{
  auto && animation { findAnimation(name) };

  if(animation == nullptr)
  {
    std::cerr << "animation couldn't be found: " << name << std::endl;
    return;
  }

  animation->addFrames(path + "/");
}

void Animator::update (const sf::Time & dt)
{
  if(m_current)
    m_current->update(dt);
}

void Animator::switchAnimation (Animation & newAnimation,
  const std::string & name, bool reset)
{
  m_current = &newAnimation;
  if(reset and name != m_currentName)
    m_current->reset();

  m_currentName = name;
}
