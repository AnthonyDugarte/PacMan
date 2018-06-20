# pragma once

# include <AssetManager.hpp>

# include <random>
# include <cassert>

namespace Helpers
{
  // This function takes a sprite and get's it's center in GLOBAL Coordinates
  sf::Vector2f GetSpriteCenter (const sf::Sprite & sprite);

  // This function takes a sprite and get's it's center in LOCAL Coordinates
  sf::Vector2f getSpriteLocalCenter (const sf::Sprite & sprite);

  /*@Description: this function uses the alpha value of the pixels that are overlapping to
   *      get an amazingly precise collision test of their textures
   *
   *  @note: The Uint8 parameter is used to set a sort of limit that defines the collision
   *       condition of those textures, it can be used to make different types of collisions
   *         creating a kind of level masks.
   */
  bool hasCollision (const sf::Sprite & spriteA, const sf::Sprite & spriteB, sf::Uint8 = 0);

  int random (int l, int r);
  float random (float l, float r);

  template <typename T>
  T getVector2Min (sf::Vector2<T> vec)
  {
      return vec.x < vec.y ? vec.x : vec.y;
  }
}
