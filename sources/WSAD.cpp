# include <WSAD.hpp>

WSAD::WSAD (bool state)
: W(state), S(state), A(state), D(state)
{
  // empty
}

void WSAD::update ()
{
  getInput();
  handleInput();
}

void WSAD::resetAx (Axis axis)
{
  switch (axis)
  {
    case Axis::x:
      A = D =false;
    break;

    case Axis::y:
      W = S = false;
    break;

    case Axis::xy:
      resetAx(Axis::x);
      resetAx(Axis::y);
    break;
  }
}

int WSAD::rPressed () const
{
  int N = 0;
  if(W or S)
    ++N;
  if(A or D)
    ++N;

  return N;
}

void WSAD::getInput ()
{
  W = sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
  S = sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
  A = sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  D = sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

void WSAD::handleInput ()
{
  if(W == S == true)
    W = S = false;

  if(A == D == true)
    A = D = false;
}
