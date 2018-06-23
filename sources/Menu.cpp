# include <Menu.hpp>

Menu::Menu (Window & window)
: Scene(window, Scene::Type::menu),
  m_currentButton(Button::play),
  m_playButton("PLAY", AssetManager::getFont("KenneyPixel.ttf"), 70),
  m_quitButton("QUIT", AssetManager::getFont("KenneyPixel.ttf"), 70),
  m_highSore("High Score: " + std::to_string(Globals::getHighScore()), AssetManager::getFont("KenneyPixel.ttf"), 30)
{
  // setting play button position
  auto && playButtonBounds = m_playButton.getLocalBounds();
  m_playButton.setOrigin(playButtonBounds.left + playButtonBounds.width / 2, playButtonBounds.top + playButtonBounds.height / 2);
  m_playButton.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 3);

  // setting quit button position
  auto && quitButtonBounds = m_quitButton.getLocalBounds();
  m_quitButton.setOrigin(quitButtonBounds.left + quitButtonBounds.width / 2, quitButtonBounds.top + quitButtonBounds.height / 2);
  m_quitButton.setPosition(m_window.getSize().x / 2, m_playButton.getPosition().y + playButtonBounds.height * 2);

  // setting high score position
  auto && hsBounds = m_highSore.getLocalBounds();
  m_highSore.setOrigin(hsBounds.left + hsBounds.width / 2, hsBounds.top + hsBounds.height / 2);
  m_highSore.setPosition(m_window.getSize().x / 2, m_quitButton.getPosition().y + quitButtonBounds.height);

  m_selected.setSize({ playButtonBounds.width, playButtonBounds.height });
  m_selected.setFillColor({ 0, 0, 0, 255 });
  m_selected.setOutlineThickness(10.f);
  m_selected.setOutlineColor({ 125, 15, 125, 255 });
  m_selected.setOrigin(playButtonBounds.width / 2, playButtonBounds.height / 2);

  m_selected.setPosition(m_playButton.getPosition());
}

Scene::Type Menu::run ()
{
  restartClock();
  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
    {
      m_window.handleEvent(event); // window stuff

      switch (event.type)
      {
        case sf::Event::KeyPressed:
          switch (event.key.code)
          {
            case sf::Keyboard::S:
            case sf::Keyboard::Down:
            if(m_currentButton == Button::play)
            {
              m_currentButton = Button::quit;
              m_selected.setPosition(m_quitButton.getPosition());
            }
            break;

            case sf::Keyboard::W:
            case sf::Keyboard::Up:
            if(m_currentButton == Button::quit)
            {
              m_currentButton = Button::play;
              m_selected.setPosition(m_playButton.getPosition());
            }
            break;

            case sf::Keyboard::Return:
            switch (m_currentButton)
            {
              case Button::play:
                return Scene::Type::gameplay;
              case Button::quit:
                return Scene::Type::end;
            }
            break;

            default: break;
          }
        break;

        default: break;
      }
    }



    m_window.beginRender(sf::Color::Black);
    m_window.draw(m_selected);
    m_window.draw(m_playButton);
    m_window.draw(m_quitButton);
    m_window.draw(m_highSore);
    m_window.endRender();

    restartClock();
  }

  return Scene::Type::end;
}
