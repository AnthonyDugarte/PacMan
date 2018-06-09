# include <Window.hpp>

Window::Window()
{
  setup("window", sf::Vector2u(640, 480));
}

Window::Window(const std::string & l_title, const sf::Vector2u & l_size)
{
  setup(l_title, l_size);
}

Window::~Window()
{
  destroy();
}

void Window::Window::beginRender()
{
  m_window.clear(sf::Color::Black);
}

void Window::draw(sf::Drawable & l_drawable)
{
  m_window.draw(l_drawable);
}

void Window::endRender()
{
  m_window.display();
}

void Window::handleEvents()
{
  sf::Event event;

  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      m_isDone = true;

    else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::F5)
      toggleFullScreen();
  }
}

const bool & Window::isDone() const
{
  return m_isDone;
}

bool Window::isFullScreen() const
{
  return m_isFullScreen;
}

const sf::Vector2u & Window::getWindowSize() const
{
  return m_windowSize;
}

void Window::toggleFullScreen()
{
  m_isFullScreen = not m_isFullScreen;
  destroy();
  create();
}

void Window::setup(const std::string & l_title, const sf::Vector2u & l_size)
{
  m_windowTitle = l_title;
  m_windowSize = l_size;
  m_isFullScreen = m_isDone = false;

  create();
}

void Window::destroy()
{
  m_window.close();
}

void Window::create()
{
  auto style { m_isFullScreen ?
      sf::Style::Fullscreen : sf::Style::Default };

  m_window.create( { m_windowSize.x, m_windowSize.y, 32 },
      m_windowTitle, style);

  m_window.setFramerateLimit(30);
}
