# pragma once

# include <SFML/Graphics.hpp>

class WSAD
{
public:
  enum class Direction { Up, Down, Left, Right, none };

  enum class Axis{ xy, x, y };

  bool W, S, A, D;

  WSAD (bool = false);

  void update ();

  void resetAx (Axis = Axis::xy);
  int rPressed () const; // are pressed? or should I say, rMoving?

private:
  void getInput ();
  void handleInput ();
};
