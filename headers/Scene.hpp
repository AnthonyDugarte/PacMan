# pragma once

# include <Window.hpp>

class Scene
{
public:
  enum class Type { menu, pause, game };

  Scene(Window &, Type);
  ~Scene();

  const Type & getType() const;

  /*virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void render() = 0;*/

  const sf::Time & getElapsed() const;
  void restartClock();

protected:
  Window & m_window;

private:
  Type m_type;

  sf::Time m_elapsed;
  sf::Clock m_clock;
};
