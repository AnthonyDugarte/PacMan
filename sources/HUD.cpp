# include <HUD.hpp>

HUD::HUD(const Attributes & attributes)
: m_attributes(attributes),
  m_hearthTexture(AssetManager::getTexture("health/hearth.png")),
  m_text("0", AssetManager::getFont("KenneyPixel.ttf"), 30),
  m_lives(m_attributes.get_health())
{
  updateLives();
}

void HUD::fitInWindow(sf::RenderWindow & window)
{
  m_text.setPosition(window.getSize().x, 0);
  m_text.move(-15, -15);
}

void HUD::update(int score)
{
  // updating lives
  if(m_attributes.get_health() != m_lives)
  {
     m_lives = m_attributes.get_health();
     updateLives();
  }

  // updating score
  if(score == 0)
    return;
  m_score += score;
  m_text.setString(std::to_string(m_score));
  if(m_score >= (100 * multiplier))
  {
    m_text.move(-15, 0);
    multiplier *= 10;
  }
}

int HUD::getScore() const
{
  return m_score;
}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
  if(m_lives)
    target.draw(m_livesSprite);
  target.draw(m_text, states);
}

void HUD::drawSprite(int count)
{
  if(count <= 0)
  {
    m_drawingLives.display();
    m_livesSprite.setTexture(m_drawingLives.getTexture(), true);
    return;
  }

  m_drawingLives.draw(m_livesSprite);
  m_livesSprite.move(m_hearthTexture.getSize().x, 0);
  drawSprite(--count);
}

void HUD::updateLives()
{
  if(m_lives == 0)
    return;

  m_livesSprite.setTexture(m_hearthTexture, true);
  m_drawingLives.create(m_hearthTexture.getSize().x * m_lives,
    m_hearthTexture.getSize().y);
  drawSprite(m_lives);
  m_livesSprite.setPosition(0,0);
}
