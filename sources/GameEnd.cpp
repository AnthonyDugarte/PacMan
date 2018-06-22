# include <GameEnd.hpp>

GameEnd::GameEnd (Window & window, const std::string & message)
: Scene(window, Scene::Type::gameOver),
  m_message(message)
{
  // empty
}

Scene::Type GameEnd::run ()
{
  sf::Text gameOver(m_message, AssetManager::getFont("KenneyPixel.ttf"), 50);
  sf::Text score("Score: " + std::to_string(Globals::Score()), AssetManager::getFont("KenneyPixel.ttf"), 30);

  auto && gameOverBounds = gameOver.getLocalBounds();
  gameOver.setOrigin(gameOverBounds.width / 2, gameOverBounds.height / 2);
  gameOver.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 2);

  auto && scoreBounds = score.getLocalBounds();
  score.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
  score.setPosition(m_window.getSize().x / 2, gameOver.getPosition().y + gameOverBounds.height * 2);

  sf::Time deadTime = sf::seconds(4);
  restartClock();
  while(not m_window.isDone())
  {
    deadTime -= getElapsed();
    if(deadTime <= sf::Time::Zero)
      break;

    sf::Event event;
    while(m_window.pollEvent(event))
      m_window.handleEvent(event); // window stuff

      m_window.beginRender(sf::Color::Black);
      m_window.draw(gameOver);
      m_window.draw(score);
      m_window.endRender();

      restartClock();
  }

  return Scene::Type::end;
}
