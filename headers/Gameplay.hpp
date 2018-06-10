# pragma once

# include <Scene.hpp>

class Gameplay : public Scene
{
public:
  Gameplay(Window &);

  Type run() override;

private:

};
