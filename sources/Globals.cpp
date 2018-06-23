# include <Globals.hpp>

Globals::Globals ()
: m_score(0)
{
  auto && highScoreFile{ AssetManager::getFile(".highScore.info") };

  std::string str;
  std::getline(highScoreFile, str, '\n');
  m_highScore = std::stoi(str);
}

Globals::~Globals ()
{
  auto && highScoreFile{ AssetManager::getFile(".highScore.info") };

  highScoreFile << getHighScore();
}

int & Globals::Score ()
{
  auto && instance { get_instance() };

  return instance.m_score;
}

int Globals::getHighScore ()
{
  auto && instance { get_instance() };

  if(instance.m_score > instance.m_highScore)
    instance.m_highScore = instance.m_score;

  return instance.m_highScore;
}
