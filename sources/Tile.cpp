# include <Tile.hpp>

Tile::Tile (Type type)
: m_type(type)
{
  // empty
}

const Tile::Type & Tile::getType () const
{
  return m_type;
}

void Tile::setType(Type type)
{
  m_type = type;
}

const sf::Vector2i & Tile::getPos() const
{
  return m_tilePos;
}

void Tile::setPos(const sf::Vector2i & newPos)
{
  m_tilePos = newPos;
}

bool Tile::isPath () const
{
  return m_type == Type::path ? true : false;
}

TilesInfo Tile::getTilesInfo (const std::string & fileName)
{
  std::fstream source { AssetManager::getFile("map/" + fileName + "/tiles.info") };
  std::string str;

  TilesInfo information;

  std::getline(source, str, '\n');
  information.tileSize.x = std::stoi(str.substr(0, str.find(",")));
  information.tileSize.y = std::stoi(str.substr(str.find(",") + 1));

  std::getline(source, str, '\n');
  information.tileNumber.x = std::stoi(str.substr(0, str.find(",")));
  information.tileNumber.y = std::stoi(str.substr(str.find(",") + 1));

  std::getline(source, str, '\n');
  information.iterations = std::stoi(str);

  for(size_t it { 10 }; --it; ) // it runs 9 times xD
  {
    std::getline(source, str, '\n');
    std::string tilePos = str.substr(str.find(":") + 1);

    information.tailPosition[static_cast<Tile::Type>(std::stoi(str.substr(0, str.find(":"))))]
      = sf::IntRect(std::stoi(tilePos.substr(0, tilePos.find(","))),
                           std::stoi(tilePos.substr(tilePos.find(",") + 1)),
                           information.tileSize.x,
                           information.tileSize.y);
  }
  source.close();

  return std::move(information);
}
