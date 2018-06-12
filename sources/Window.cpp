# include <Window.hpp>

Window::Window (const std::string & l_title, const sf::Vector2u & l_size)
  : m_windowTitle(l_title),
    m_windowSize(l_size)
{
  __create();
}

Window::~Window ()
{
  close();
}

void Window::Window::beginRender (const sf::Color & BGcolor)
{
  clear(BGcolor);
}

void Window::endRender ()
{
  display();
}

void Window::handleEvent (const sf::Event & event)
{
  switch(event.type)
  {
    case sf::Event::Closed:
      m_isDone = true;
    break;

    case sf::Event::KeyPressed:

    switch (event.key.code)
    {
      case sf::Keyboard::F5:
        toggleFullScreen();
      break;

      case sf::Keyboard::F8:
        screenShot();
      break;

      default: break;
    }

    break;

    default: break;
  }
}

void Window::screenShot() const
{
  std::fstream ctrol { AssetManager::getFile("Captures/ctrol.file") };

  std::string str;
  std::getline(ctrol, str , '\n');

  int number { std::stoi(str) };

  sf::Vector2u windowSize { getSize() };

  sf::Texture texture;
  texture.create(windowSize.x, windowSize.y);
  texture.update(*this);

  sf::Image screenshot { texture.copyToImage() };
  screenshot.saveToFile("assets/Captures/capture" + std::to_string(number++) + ".png");

  ctrol.seekg(0);
  ctrol << number;
  
  ctrol.close();
}

const bool & Window::isDone () const
{
  return m_isDone;
}

bool Window::isFullScreen () const
{
  return m_isFullScreen;
}

void Window::toggleFullScreen ()
{
  m_isFullScreen = not m_isFullScreen;
  close();
  __create();
}

sf::Vector2u Window::getSize  () const
{
  return m_windowSize;
}

void Window::__create ()
{
  auto style { m_isFullScreen ?
      sf::Style::Fullscreen : sf::Style::Default };

  create( { m_windowSize.x, m_windowSize.y, 32 },
      m_windowTitle, style);

  setFramerateLimit(60);
}
