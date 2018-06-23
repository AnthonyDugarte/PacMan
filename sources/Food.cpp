# include <Food.hpp>

Food::Food (const TileMap & mapita)
: m_tileCount(mapita.getTileCount()),
  m_tileSize(mapita.getTileSize()),
  m_totalFood(0)
{
  sf::Vector2f position(0, 0);

  auto && foodTexture = AssetManager::getTexture("map/" + mapita.name() + "/food.png");
  sf::Sprite foodSprite(foodTexture);

  std::vector<bool> XaxisInitData(m_tileCount.x, false);
  m_foodStatus.insert(m_foodStatus.begin(), m_tileCount.y, XaxisInitData);

  m_texture.create(m_tileCount.x * m_tileSize.x, m_tileCount.y * m_tileSize.y);
  m_texture.clear(sf::Color(0, 0, 0, 0));
  m_texture.setSmooth(true);

  for(size_t y = 0; y < m_tileCount.y; ++y)
    for(size_t x = 0; x < m_tileCount.x; ++x)
    {
      if(mapita._walkable(x, y))
      {
        ++m_totalFood;
        m_foodStatus[y][x] = true;
        foodSprite.setPosition(x * m_tileSize.x, y * m_tileSize.y);
        m_texture.draw(foodSprite);
      }
    }

  // setting up big food
  auto && bigFood = AssetManager::getTexture("map/" + mapita.name() + "/bigFood.png");
  foodSprite.setTexture(bigFood, true);
  std::fstream & foodInfo { AssetManager::getFile("map/" + mapita.name() + "/.food.info") };
  std::string str;

  std::getline(foodInfo, str, '\n');
  if(str != "# special food")
    throw std::invalid_argument("posibble not a .info food file");

  for(std::getline(foodInfo, str, '\n'); str[0] != '#'; std::getline(foodInfo, str, '\n'))
  {
    sf::Vector2i pos(std::stoi(str.substr(0, str.find(","))), std::stoi(str.substr(str.find(",") + 1)));
    m_specialFood.push_back(pos);
    foodSprite.setPosition(pos.x * m_tileSize.x, pos.y * m_tileSize.y);
    m_texture.draw(foodSprite);
  }
  /***************/

  // eating baad placed food
  for(std::getline(foodInfo, str, '\n'); str != ""; std::getline(foodInfo, str, '\n'))
  {
    sf::Vector2f pos(std::stoi(str.substr(0, str.find(","))), std::stoi(str.substr(str.find(",") + 1)));
    eatFood({ pos.x * m_tileSize.x, pos.y * m_tileSize.y });
  }

  m_texture.display();
  setTexture(m_texture.getTexture(), true);
}

Food::~Food ()
{
  /* I think that sprite's destructor writes on it's texture (which is unwrittable thanks to RenderTexture)
   * so we make it writte in something else xD
   */
  sf::Texture empty;
  setTexture(empty);
}

void Food::updateTexture ()
{
  // if don't, texture is flipped downside
  m_texture.display();
  setTexture(m_texture.getTexture());
}

int Food::eatFood (const sf::Vector2f & floatPos)
{
  // relative position to the grid
  sf::Vector2i position(floatPos.x / m_tileSize.x, floatPos.y / m_tileSize.y);

  // if not in the grid, nothing to do
  if(position.x >= m_tileCount.x or position.y >= m_tileCount.y)
    return 0;

  // if already eaten, nothing to do
  if(not _eat(position))
    return 0;

  // our empty grid, POSIBBLE TODO: use a RectShape
  sf::Image emptyImg;
  emptyImg.create(m_tileSize.x, m_tileSize.y, sf::Color(0, 0, 0, 255));

  sf::Texture emptyTexture;
  emptyTexture.loadFromImage(emptyImg);

  sf::Sprite empty(emptyTexture);
  empty.setPosition(position.x * m_tileSize.x, position.y * m_tileSize.y);

  m_texture.draw(empty);
  updateTexture();

  for(auto && it = m_specialFood.begin(); it != m_specialFood.end(); ++it)
    if(*it == position)
    {
      m_eatenSpecialFood = true;
      m_specialFood.erase(it);
      return static_cast<int> (Type::big);
    }

  return static_cast<int>(Type::small);
}

bool Food::eatenSpecialFood ()
{
  if(m_eatenSpecialFood)
  {
    m_eatenSpecialFood = false;
    return true;
  }
  return false;
}

bool Food::over () const
{
  return m_totalFood == 0;
}


bool Food::_eat (const sf::Vector2i & position)
{
  if(m_foodStatus[position.y][position.x])
  {
    --m_totalFood;
    m_foodStatus[position.y][position.x] = false;
    return true;
  }

  return false;
}
