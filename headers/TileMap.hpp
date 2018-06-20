# pragma once

# include <Tile.hpp>
# include <WSAD.hpp>

class TileMap;

namespace Helpers
{
  bool isMapAvailable (TileMap & map);
  void generateMap (TileMap & map, const TilesInfo & inf);
}

class TileMap : public sf::Sprite
{
public:
  //@Param1: name inside assets/map/ folder
  TileMap (const std::string & name, bool showPath = false);

  const std::string & name () const;

  //@Param2: position of pad in global coordinates
  void updateWSAD (WSAD & pad, const sf::Vector2f & pos);

  sf::Vector2i getTileSize () const;
  sf::Vector2i getTileCount () const;

  //@Params: positions in grid coordinates
  Tile* getTile(int x, int y);


  bool isWalkable (float x, float y) const; // global coordinates
  bool _walkable (float x, float y) const; // grid coordinates
  sf::Vector2f getTileCenter (const sf::Vector2f & pos) const; // global cords to center of tile in pos

  // reset A* information: H, G, F and parentNode
  void resetNodes();

private:
  std::string m_name;
  sf::Vector2i m_tileSize;
  bool m_showPath;

  sf::RenderTexture m_texture;
  std::map<int, std::map<int, Tile>> m_tiles; // it is accesed [y][x];

  bool validPos(float x, float y) const;

  friend void Helpers::generateMap(TileMap & map, const TilesInfo & inf);
  friend bool Helpers::isMapAvailable(TileMap & map);
};
