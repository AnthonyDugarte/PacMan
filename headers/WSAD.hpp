# pragma once

# include <SFML/Graphics.hpp>

class WSAD
{
public:
  enum class Axis{ xy, x, y };

  bool W = false, S = false, A = false, D = false;

  WSAD ();

  void update ();

  void resetAx (Axis = Axis::xy);
  int rPressed () const; // are pressed?
  
private:
  void getInput ();
  void handleInput ();
};
