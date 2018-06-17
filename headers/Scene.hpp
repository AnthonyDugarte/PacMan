# pragma once

# include <Window.hpp>

class Scene
{
public:
  enum class Type { gameplay, end };

  Scene (Window &, Type);
  virtual ~Scene ();

  const Type & getType () const;

  const sf::Time & getElapsed () const;
  void restartClock ();

  virtual Type run () = 0;

protected:
  Window & m_window;

private:
  Type m_type;

  sf::Time m_elapsed;
  sf::Clock m_clock;
};
