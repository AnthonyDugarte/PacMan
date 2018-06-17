# pragma once

# include <TileMap.hpp>

class Food : public sf::Sprite
{
public:
  // TODO: Add the big food type
  enum class Type { small = 100, big = 500 }; // points

  // take the path way of the map and set all along it the food
  Food (const TileMap &);
  ~Food ();

  /* take out the food of that position, so, when intended to work with it,
   * one call is all you have for each piece of food
   */
  bool eatFood (const sf::Vector2f & position);

private:
  void updateTexture();

  bool _eat(const sf::Vector2i &);

  // here we have our status
  std::vector<std::vector<bool>> foodStatus;

  sf::RenderTexture m_texture;

  sf::Vector2i tileCount;
  sf::Vector2i tileSize;
};
