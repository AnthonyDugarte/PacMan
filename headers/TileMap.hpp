# pragma once

# include <Tile.hpp>
# include <WSAD.hpp>

class TileMap;

namespace Helpers
{
  bool isMapAvailable(TileMap &);
  void generateMap(TileMap &, const TilesInfo &);
}

class TileMap : public sf::Sprite
{
public:
  TileMap (const std::string &, bool showPath = false);

  const std::string & name() const;

  void updateWSAD(WSAD &, const sf::Vector2f &);

  sf::Vector2i getTileSize() const;
  sf::Vector2i getTileCount() const;

  bool isWalkable(float, float) const; // global coordinates
  bool _walkable(float, float) const; // grid coordinates

private:
  std::string m_name;
  sf::Vector2i tileSize;
  bool m_showPath;

  std::vector<std::vector<Tile::Type>> tiles; // it is accesed [y][x];

  friend void Helpers::generateMap(TileMap &, const TilesInfo &);
  friend bool Helpers::isMapAvailable(TileMap &);
};
