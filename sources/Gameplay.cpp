# include <Gameplay.hpp>

Gameplay::Gameplay(Window & window)
  : Scene(window, Scene::Type::gameplay)
{
  // empty
}

Scene::Type Gameplay::run()
{
  sf::RectangleShape rec(sf::Vector2f(10, 10));
  rec.setFillColor(sf::Color::Blue);


  restartClock();
  while(not m_window.isDone())
  {
    sf::Event event;
    while(m_window.pollEvent(event))
    {
      m_window.handleEvent(event);

      switch(event.type)
      {
        case sf::Event::KeyPressed:
        switch (event.key.code)
        {
          case sf::Keyboard::W:
          rec.move(0, -10);
          break;
          case sf::Keyboard::S:
          rec.move(0, 10);
          break;
          case sf::Keyboard::A:
          rec.move(-10, 0);
          break;
          case sf::Keyboard::D:
          rec.move(10, 0);
          break;

          default: break;
        }
        break;

        default:
        break;
      }
    }

    m_window.beginRender();
    m_window.draw(rec);
    m_window.endRender();

    restartClock();
  }

  /*virtual void handleEvents() = 0;
  virtual void update() = 0;
  virtual void render() = 0;*/
  return Scene::Type::end;
}
