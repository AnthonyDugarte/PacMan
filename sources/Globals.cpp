# include <Globals.hpp>

Globals::Globals ()
: m_score(0)
{
  // empty
}

int & Globals::Score ()
{
  auto && instance { get_instance() };

  return instance.m_score;
}
