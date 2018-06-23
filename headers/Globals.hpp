# pragma once

# include <AssetManager.hpp>

class Globals : public Designar::Singleton<Globals>
{
protected:
  Globals ();

public:
  ~Globals ();
  
  static int & Score ();
  static int getHighScore ();

private:
  int m_score;
  int m_highScore;

  friend class Singleton<Globals>;
};
