# pragma once

# include <AssetManager.hpp>

struct TilesInfo;

class Tile
{
public:
  enum class Type
  {
    upperLeft = 0,        upper = 1,       upperRight = 2,
    left = 3,                     path  = 4,         right = 5,
    lowerLeft = 6,        lower = 7,        lowerRight = 8,

    empty = 9
  };

  Tile () = delete;
  Tile (Type, const sf::IntRect &);

  Type getType () const;
  bool isPath () const;

  static TilesInfo getTilesInfo (const std::string &);

private:
  Type m_type;
  sf::IntRect m_globalBounds;
};

struct TilesInfo
{
  TilesInfo()
  {
    // empty
  }

  sf::Vector2i tileSize;
  sf::Vector2i tileNumber;
  int iterations; // number of tilex.info files

  std::map<Tile::Type, sf::IntRect> tailPosition;
};
