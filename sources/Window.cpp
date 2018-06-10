# include <Window.hpp>

Window::Window()
{
  __setup("window", sf::Vector2u(640, 480));
}

Window::Window(const std::string & l_title, const sf::Vector2u & l_size)
{
  __setup(l_title, l_size);
}

Window::~Window()
{
  close();
}

void Window::Window::beginRender(const sf::Color & BGcolor)
{
  clear(BGcolor);
}

void Window::endRender()
{
  display();
}

void Window::handleEvent(const sf::Event & event)
{
  if (event.type == sf::Event::Closed)
    m_isDone = true;

  else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::F5)
    toggleFullScreen();
}

const bool & Window::isDone() const
{
  return m_isDone;
}

bool Window::isFullScreen() const
{
  return m_isFullScreen;
}

void Window::toggleFullScreen()
{
  m_isFullScreen = not m_isFullScreen;
  close();
  __create();
}

sf::Vector2u Window::getSize () const
{
  return m_windowSize;
}

void Window::__setup(const std::string & l_title, const sf::Vector2u & l_size)
{
  m_windowTitle = l_title;
  m_windowSize = l_size;
  m_isFullScreen = m_isDone = false;

  __create();
}

void Window::__create()
{
  auto style { m_isFullScreen ?
      sf::Style::Fullscreen : sf::Style::Default };

  create( { m_windowSize.x, m_windowSize.y, 32 },
      m_windowTitle, style);

  setFramerateLimit(30);
}
