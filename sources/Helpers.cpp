# include <Helpers.hpp>

namespace Helpers
{
  sf::Vector2f GetSpriteCenter (const sf::Sprite & sprite)
  {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    return sf::Vector2f (bounds.left+bounds.width * .5f, bounds.top + bounds.height * .5f);
  }

  sf::Vector2f getSpriteLocalCenter (const sf::Sprite & sprite)
  {
    return sf::Vector2f(static_cast<sf::Vector2f>(sprite.getTexture()->getSize()) * .5f);
  }

  sf::Uint8 getPixel (const sf::Uint8 * mask, const sf::Texture * texture, size_t x, size_t y)
  {
    if (x > texture->getSize().x or y > texture->getSize().y)
      return 0;

    return mask[x + y * texture->getSize().x];
  }

  bool hasCollision (const sf::Sprite & Ob1, const sf::Sprite & Ob2, sf::Uint8 alphaLimit)
  {
    sf::FloatRect IntersectionRect;

    if(Ob1.getGlobalBounds().intersects(Ob2.getGlobalBounds(), IntersectionRect))
    {
      auto && Ob1subRect{ Ob1.getTextureRect() };
      auto && Ob2subRect{ Ob2.getTextureRect() };

      auto && Ob1mask{ AssetManager::getBitMask(Ob1.getTexture()) };
      auto && Ob2mask{ AssetManager::getBitMask(Ob2.getTexture()) };

      for(size_t x = IntersectionRect.left; x < IntersectionRect.left + IntersectionRect.width; ++x)
        for(size_t y = IntersectionRect.top; y < IntersectionRect.top + IntersectionRect.height; ++y)
        {
          sf::Vector2f Ob1v{ Ob1.getInverseTransform().transformPoint(x, y) };
          sf::Vector2f Ob2v{ Ob2.getInverseTransform().transformPoint(x, y) };

          if(Ob1v.x > 0 and Ob1v.y > 0 and
              Ob2v.x > 0 and Ob2v.y > 0 and
              Ob1v.x < Ob1subRect.width and Ob1v.y < Ob1subRect.height and
              Ob2v.x < Ob2subRect.width and Ob2v.y < Ob2subRect.height)
          {
            if(getPixel(&*Ob1mask, &*Ob1.getTexture(), Ob1v.x + Ob1subRect.left, Ob1v.y + Ob1subRect.top) > alphaLimit and
                getPixel(&*Ob2mask, &*Ob2.getTexture(), Ob2v.x + Ob2subRect.left, Ob2v.y + Ob2subRect.top) > alphaLimit)
      return true;
          }
        }
    }

    return false;
  }

  std::random_device seed_device;
  std::default_random_engine engine(seed_device());

  int random (int l, int r)
  {
    assert(l < r);
    std::uniform_int_distribution<uint> distribution(l, r);

    return distribution(engine);
  }

  float random (float l, float r)
  {
    assert(l < r);
    std::uniform_real_distribution<float> distribution(l, r);

    return distribution(engine);
  }
} // Helpers
