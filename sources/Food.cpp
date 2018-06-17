# include <Food.hpp>

Food::Food (const TileMap & mapita)
  : tileCount(mapita.getTileCount()),
    tileSize(mapita.getTileSize())
{
  sf::Vector2f position(0, 0);

  auto && foodTexture = AssetManager::getTexture("map/" + mapita.name() + "/food.png");
  sf::Sprite foodSprite(foodTexture);

  std::vector<bool> XaxisInitData(tileCount.x, false);
  foodStatus.insert(foodStatus.begin(), tileCount.y, XaxisInitData);

  m_texture.create(tileCount.x * tileSize.x, tileCount.y * tileSize.y);
  m_texture.setSmooth(true);

  while(position.y < tileCount.y)
  {
    position.x = 0;
    while(position.x < tileCount.x)
    {
      if(mapita._walkable(position.x, position.y))
      {
        foodStatus[position.y][position.x] = true;
        foodSprite.setPosition(position.x * tileSize.x, position.y * tileSize.y);
        m_texture.draw(foodSprite);
      }

      ++position.x;
    }
    ++position.y;
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

void Food::updateTexture()
{
  // if don't, texture is flipped downside
  m_texture.display();
  setTexture(m_texture.getTexture());
}

bool Food::eatFood (const sf::Vector2f & floatPos)
{
  // relative position to the grid
  sf::Vector2i position(floatPos.x / tileSize.x, floatPos.y / tileSize.y);

  // if not in the grid, nothing to do
  if(position.x >= tileCount.x or position.y >= tileCount.y)
    return false;

  // if already eaten, nothing to do
  if(not _eat(position))
    return false;

  // our empty grid, POSIBBLE TODO: use a RectShape
  sf::Image emptyImg;
  emptyImg.create(tileSize.x, tileSize.y, sf::Color(0, 0, 0, 255));

  sf::Texture emptyTexture;
  emptyTexture.loadFromImage(emptyImg);

  sf::Sprite empty(emptyTexture);
  empty.setPosition(position.x * tileSize.x, position.y * tileSize.y);

  m_texture.draw(empty);
  updateTexture();

  return true;
}

bool Food::_eat(const sf::Vector2i & position)
{
  if(foodStatus[position.y][position.x])
  {
    foodStatus[position.y][position.x] = false;
    return true;
  }

  return false;
}