# pragma once

# include <Scene.hpp>
# include <Globals.hpp>

class GameEnd : public Scene
{
public:
  GameEnd (Window & window, const std::string & message);

  Type run () override;

private:
  std::string m_message;
};
