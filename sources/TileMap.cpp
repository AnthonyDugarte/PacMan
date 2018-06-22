# include <TileMap.hpp>

TileMap::TileMap (const std::string & mapName, bool showPath)
: m_name(mapName),
  m_showPath(showPath)
{
  if(not Helpers::isMapAvailable(*this))
    Helpers::generateMap(*this, Tile::getTilesInfo(mapName));

  m_texture.display();
  setTexture(m_texture.getTexture(), true);
}

const std::string & TileMap::name () const
{
  return m_name;
}

void TileMap::updateWSAD (WSAD & pad, const sf::Vector2f & floatPos)
{
  sf::Vector2i position(floatPos.x / m_tileSize.x, floatPos.y / m_tileSize.y);
  sf::Vector2i tilePosition;
  tilePosition.x = m_tileSize.x - (static_cast<int>(floatPos.x) % m_tileSize.x);
  tilePosition.y = m_tileSize.y - (static_cast<int>(floatPos.y) % m_tileSize.y);

  sf::Vector2i Tilehalf = m_tileSize / 2;

  /* Brief explanaton: we check our environment, if it's walkable, then we don't
   * uncheck them but, we are working directly on the path (TODO: check this),
   * so we just don't allow the player to move that way

   * second condition: we just disable movemente when player is in the middle of
   * the tile in that direction, so player cannot get further than the middle of the tile
   */

  if(not _walkable(position.x, position.y - 1) and tilePosition.y >= Tilehalf.y) // Up
    pad.W = false;

  if(not _walkable(position.x, position.y + 1) and tilePosition.y <= Tilehalf.y) // down
    pad.S = false;

  if(not _walkable(position.x - 1, position.y) and tilePosition.x >= Tilehalf.x) // left
    pad.A = false;

  if(not _walkable(position.x + 1, position.y) and tilePosition.x <= Tilehalf.x) // right
    pad.D = false;
}

sf::Vector2i TileMap::getTileSize () const
{
  return m_tileSize;
}

sf::Vector2i TileMap::getTileCount () const
{
  return { m_tiles.size() != 0 ? static_cast<int>(m_tiles.at(0).size()) : 0, static_cast<int>(m_tiles.size()) };
}

Tile* TileMap::getTile (int x, int y)
{
  if(not validPos(x, y))
    return nullptr;

  return &m_tiles[y][x];
}

bool TileMap::isWalkable (float x, float y) const
{
  return _walkable(x / m_tileSize.x, y / m_tileSize.y);
}

bool TileMap::_walkable (float x, float y) const
{
  if(not validPos(x, y))
    return false;

  return m_tiles.at(y).at(x).isPath();
}

sf::Vector2f TileMap::getTileCenter (const sf::Vector2f & pos) const
{
  return sf::Vector2f(static_cast<int>(pos.x / m_tileSize.x) * m_tileSize.x + (m_tileSize.x >> 1),static_cast<int>(pos.y / m_tileSize.y) * m_tileSize.y + (m_tileSize.y >> 1));
}

void TileMap::resetNodes ()
{
  for(auto && y : m_tiles)
    for(auto && x : y.second)
    {
      x.second.H = x.second.G = x.second.F = 0;
      x.second.parentNode = nullptr;
    }
}

bool TileMap::validPos (float x, float y) const
{
  if(y >= m_tiles.size() or m_tiles.size() == 0 or x < 0 or y < 0)
    return false;
  if(x >= m_tiles.at(0).size())
    return false;

  return true;
}

namespace Helpers
{
  bool isMapAvailable (TileMap & tile_map)
  {
    try // to see if map.info is build
    {
      auto && mapPath { AssetManager::getFile("map/" + tile_map.name() + "/map.info") };
      std::string str;

      std::getline(mapPath, str, '\n');
      if(str == "T" and not tile_map.m_showPath)
        return false;
      else if(str == "F" and tile_map.m_showPath)
        return false;

      std::getline(mapPath, str, '\n');
      {
        tile_map.m_tileSize.x = std::stoi(str.substr(0, str.find(",")));
        tile_map.m_tileSize.y = std::stoi(str.substr(str.find(",") + 1));
      }

      std::getline(mapPath, str, '\n');
      int xTiles = std::stoi(str.substr(0, str.find(",")));
      int yTiles = std::stoi(str.substr(str.find(",") + 1));

      auto && tiles = tile_map.m_tiles;
      for(int y = 0; y < yTiles; ++y)
        for(int x = 0; x < xTiles; ++x)
        {
          auto && lilTile = tiles[y][x];
          lilTile.setType(static_cast<Tile::Type>(mapPath.get() - '0')); // hell of a trick
          lilTile.setPos({ x, y});
          // lilTile.tilePos.x = x;
          // lilTile.tilePos.y = y;
          mapPath.get(); // bye comma, bye '\n'
        }
    }
    catch(...) // if not, then is not that available, isn't it?
    {
      return false;
    }

    try // now to take the texture
    {
      auto && mapTexture { AssetManager::getTexture("map/" + tile_map.name() + "/map.png") };

      sf::Vector2i size(tile_map.getTileCount().x * tile_map.getTileSize().x,
                                  tile_map.getTileCount().y * tile_map.getTileSize().y);

      tile_map.m_texture.create(size.x, size.y);
      tile_map.m_texture.setSmooth(true);

      sf::Sprite aux(mapTexture);
      tile_map.m_texture.draw(aux);
    }
    catch(...) // if wasn't taken, then it is not available... true logic
    {
      tile_map.m_tiles.clear();
      return false;
    }

    std::clog << "successful map load\n";
    return true; // it's available aksjdkasjdkas
  }

  void generateMap (TileMap & tile_map, const TilesInfo & inf)
  {
    // where we hare going to draw the map, for the sake of clarity

    // defining the size of the map according to number of tiles and their size
    tile_map.m_texture.create(inf.tileNumber.x * inf.tileSize.x, inf.tileNumber.y * inf.tileSize.y);
    // cute and smoothy picture.
    tile_map.m_texture.setSmooth(true);

    for(int y = 0; y < inf.tileNumber.y; ++y)
      for(int x = 0; x < inf.tileNumber.x; ++x)
        tile_map.m_tiles[y][x].setPos({ x, y });
    /* number of capes the world is going to have, what I say is: number of different
     * textures to render, their priotity goes from low->hight.
     */
    for(size_t i { 0 }; i <= inf.iterations; ++i)
    {
      /* tiles information and texture must be on map/map_name/
       * and the have to be prefixed as tile and postfixed with a value, their priority
       *
       * Excepctions aren't handled. Care with it.
       */
      std::string prefix("map/" + tile_map.name() + "/tile" + std::to_string(i));

      // both files are needed: .info and .png.
      std::fstream & tileFile { AssetManager::getFile(prefix + ".info" ) };
      auto && tileTexture { AssetManager::getTexture(prefix + ".png") };

      // auxiliar string
      std::string str;

      // in the map tile matrix
      sf::Vector2i currPos;

      while(tileFile) // don't care to much, could be a while(true) statement
      {
        std::getline(tileFile, str, '\n');

        if(str == "") // meet conditional, breaker of loops and master of progression
          break;

        // these are helpers for the input handle, we want to take numbers, not commas
        size_t pos { 0 };
        size_t newPos { str.find(",") };

        // if our first search for a comma wasn't successfull we just go up in the y-axis
        if(newPos == std::string::npos)
        {
          ++currPos.y;
          continue;
        }

        /* this is for a little trick made up by commas, if you are at the begining, no spaces backward
         * but then you start getting spaces, this should be handdled some other way
         */
        auto PosZero = [](size_t p)->size_t { return p == 0 ? p : p + 1; };

        // True def: while we can take numbers from the line, then take them, kinda greedy
        while(pos != std::string::npos)
        {
          // we get our tile type (which is a number, strings would be so damn awful to look at)
          Tile::Type type = static_cast<Tile::Type>(std::stoi(str.substr(PosZero(pos), newPos - PosZero(pos))));


          if(type != Tile::Type::empty) // if your tile is not an empty one
          {
            // we mark it in our vector
            auto && lilTile = tile_map.m_tiles[currPos.y][currPos.x];
            lilTile.setType(type);

            if(not (not tile_map.m_showPath and type == Tile::Type::path))
            {
              // and draw it on our map
              sf::Sprite tileSprite(tileTexture, inf.tailPosition.at(type));

              // uhmm variable names should be reworked
              tileSprite.setPosition(currPos.x * inf.tileSize.x, currPos.y * inf.tileSize.y);
              tile_map.m_texture.draw(tileSprite);
            }
          }

          // we advance on our line and of course, on our matrix also
          pos = newPos;
          newPos = str.find(",", pos + 1);
          ++currPos.x;
        } // end of X-axis for this Y-axis.

        //  we advance to the next Y-axis
        currPos.x = 0;
        ++currPos.y;
      } // end of Y-axis. We continue to the next tile drawing
    } // end of tiles. We now shall draw our map and set it uuwwu

   tile_map.m_texture.display(); // sounds tricky, isn't it? Just check sf::RenderTexture documentation (I haven't, not completly of course, gosh)

   std::string mapPath("map/" + tile_map.name() + "/map.png");
   AssetManager::saveTexture(tile_map.m_texture.getTexture(), mapPath);

    /* saving the map.info (tile division, this is going to be use for walking uwwu, we are gonna save the size of the map so we can
     *  reload our vector with the isMapAvailable function.
     */
    std::ofstream  mapInfo("assets/map/" + tile_map.name() + "/map.info");

    mapInfo << (tile_map.m_showPath ? "T" : "F") << "\n";

    mapInfo << inf.tileSize.x << "," << inf.tileSize.y << "\n"; // for map.size, I'm lazy why should we divide texture size with tiles number
    mapInfo << inf.tileNumber.x << "," << inf.tileNumber.y << "\n"; // number of tiles (for vector)

    for(auto && y : tile_map.m_tiles)
    {
      for(auto && x : y.second) // pure magic
        mapInfo << static_cast<int>(x.second.getType()) << ","; // ajaaaa, we have to take out the last coma

      mapInfo.seekp(static_cast<int>(mapInfo.tellp()) - 1);
      mapInfo << "\n";
    }

    tile_map.m_tileSize = inf.tileSize;

    std::clog << "successful map Creation\n"; // cute log report of success
  }
}  // namespace Helpers
