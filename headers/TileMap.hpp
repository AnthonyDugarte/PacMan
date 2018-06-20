# pragma once

# include <Tile.hpp>
# include <WSAD.hpp>

class TileMap;

namespace Helpers
{
  bool isMapAvailable (TileMap &);
  void generateMap (TileMap &, const TilesInfo &);
}

class TileMap : public sf::Sprite
{
public:
  TileMap (const std::string &, bool showPath = false);

  const std::string & name () const;

  void updateWSAD (WSAD &, const sf::Vector2f &);

  sf::Vector2i getTileSize () const;
  sf::Vector2i getTileCount () const;
  Tile* getTile(int , int);

  bool isWalkable (float, float) const; // global coordinates
  bool _walkable (float, float) const; // grid coordinates
  sf::Vector2f getTileCenter (const sf::Vector2f &) const; // global cords to center of tile

  // reset A* information: H, G, F and parentNode
  void resetNodes();

private:
  std::string m_name;
  sf::Vector2i tileSize;
  bool m_showPath;

  sf::RenderTexture m_texture;
  std::map<int, std::map<int, Tile>> tiles; // it is accesed [y][x];

  bool validPos(float x, float y) const;

  friend void Helpers::generateMap(TileMap &, const TilesInfo &);
  friend bool Helpers::isMapAvailable(TileMap &);
};
