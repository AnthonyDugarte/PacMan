# pragma once

# include <AssetManager.hpp>
# include <Attributes.hpp>

class HUD : public sf::Drawable
{
public:
  enum class healtState
  {
    empty = 0, almost_empty = 1, almost_full = 2, full = 3
  };
  HUD(const Attributes &);

  void fitInWindow(sf::RenderWindow & window);

  void update(int score);

private:
  virtual void draw(sf::RenderTarget &, sf::RenderStates) const;

  void drawSprite(int count);
  void updateLives();

  const Attributes & m_attributes;
  sf::Texture & m_hearthTexture;
  sf::Sprite m_livesSprite;
  sf::RenderTexture m_drawingLives;

  sf::Text m_text;

  int m_score = 0;
  int multiplier = 1;
  int m_lives;
};
