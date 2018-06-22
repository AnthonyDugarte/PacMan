# pragma once

# include <TileMap.hpp>

class Food : public sf::Sprite
{
public:
  // TODO: Add the big food type
  enum class Type { small = 100, big = 500 }; // points

  // take the path way of the map and set all along it the food
  Food (const TileMap & map);
  ~Food ();

  /* take out the food of that position, so, when intended to work with it,
   * one call is all you have for each piece of food
   */
  int eatFood (const sf::Vector2f & position);

  bool eatenSpecialFood();
  bool over() const;
private:
  void updateTexture ();

  bool _eat (const sf::Vector2i & position);

  // here we have our status
  std::vector<std::vector<bool>> m_foodStatus;
  std::vector<sf::Vector2i> m_specialFood;

  sf::RenderTexture m_texture;

  sf::Vector2i m_tileCount;
  sf::Vector2i m_tileSize;
  bool m_eatenSpecialFood = false;
  int m_totalFood;
};
