# pragma once

# include <Singleton.hpp>

class Globals : public Designar::Singleton<Globals>
{
protected:
  Globals ();

public:
  static int & Score ();

private:
  int m_score;

  friend class Singleton<Globals>;
};
