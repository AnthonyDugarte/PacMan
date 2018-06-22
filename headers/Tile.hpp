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

  Tile ()
  : m_type(Type::empty)
  {
    // empty
  }

  Tile (Type type);

  const Type & getType () const;
  void setType (Type newType);

  const sf::Vector2i & getPos () const;
  // local Position, in grid coordinates
  void setPos (const sf::Vector2i & pos);

  bool isPath () const;

  /* A* stuff*/
  int H; // movement cost to goal
  int G; // movement cost of entire path
  int F; // H + G
  Tile* parentNode;
  /**/

  static TilesInfo getTilesInfo (const std::string &);

private:
  sf::Vector2i m_tilePos;
  Type m_type;
};

struct TilesInfo
{
  TilesInfo ()
  {
    // empty
  }

  sf::Vector2i tileSize;
  sf::Vector2i tileNumber;
  int iterations; // number of tilex.info files

  std::map<Tile::Type, sf::IntRect> tailPosition;
};
