# include <AssetManager.hpp>

AssetManager::AssetManager ()
: m_textures()
{
  // empty
}

AssetManager::~AssetManager ()
{
  while(not m_bitmasks.empty())
  {
    delete [] m_bitmasks.begin()->second;
    m_bitmasks.erase(m_bitmasks.begin());
  }
}

sf::Texture & AssetManager::getTexture (const std::string & name)
{
  //Manager instance
  auto && instance { get_instance() };

  auto && search{ instance.m_textures.find(name) };
  if (search != instance.m_textures.end())
    return search->second;

  sf::Texture new_texture;

  std::string prefix{ "assets/" };
  if (not new_texture.loadFromFile(prefix + name))
    throw std::invalid_argument("texture could not be loaded: " + prefix + name);

  new_texture.setSmooth(true);

  instance.m_textures[name] = std::move(new_texture);
  return *&instance.m_textures[name];
}

namespace Helpers
{
  const std::pair<const sf::Texture *, sf::Uint8 *> getMask (const sf::Texture *);
}

sf::Uint8 * AssetManager::getBitMask (const std::string & name)
{
  //Manager instance
  auto && instance { get_instance() };

  sf::Texture * bitMaskTexture;

  //we charge our texture on the Manager so we have just one instance of bitMask per Texture
  try
  {
    bitMaskTexture = &getTexture(name);
  }
  catch(...)
  {
    throw std::invalid_argument("bitMask could not be loaded: " + name);
  }

  auto && search{ instance.m_bitmasks.find(&*bitMaskTexture) };
  if(search != instance.m_bitmasks.end())
    return search->second;

  auto && new_mask{ Helpers::getMask(&*bitMaskTexture) };

  instance.m_bitmasks.insert(std::move(new_mask));
  return *&new_mask.second;
}

sf::Uint8 * AssetManager::getBitMask (const sf::Texture * bitMaskTexture)
{
  //Manager instance
  auto && instance { get_instance() };

  auto && search{ instance.m_bitmasks.find(&*bitMaskTexture) };
  if(search != instance.m_bitmasks.end())
    return &*search->second;

  auto && new_mask { Helpers::getMask(&*bitMaskTexture) };

  instance.m_bitmasks.insert(std::move(new_mask));
  return &*new_mask.second;
}

namespace Helpers
{
  const std::pair<const sf::Texture *, sf::Uint8 *> getMask (const sf::Texture * texture)
  {
    sf::Image img{ texture->copyToImage() };
    sf::Uint8 * new_mask{ new sf::Uint8[texture->getSize().y * texture->getSize().x] };

    for(size_t y{ 0 }; y < texture->getSize().y ; ++y)
      for(size_t x{ 0 }; x < texture->getSize().x; ++x)
        new_mask[x + y * texture->getSize().x] = img.getPixel(x, y).a;

    return std::pair<const sf::Texture *, sf::Uint8 *>(&*texture, std::move(&*new_mask));
  }
}

std::fstream AssetManager::getFile (const std::string & filename)
{
  std::string prefix{ "assets/" };
  std::fstream file(prefix + filename);

  if(not file.is_open())
    throw std::invalid_argument("file could not be opened: " + prefix + filename);

  return std::move(file);
}

namespace Helpers
{
  void __saveScreenShot (sf::Texture && texture, size_t capsN)
  {
    sf::Image screenshot { texture.copyToImage() };

    std::string prefix{ "assets/Captures/capture" + std::to_string(capsN) };
    screenshot.saveToFile(prefix + ".png");
  }
}

void AssetManager::saveScreenShot (sf::Texture && texture, size_t capsN)
{
  auto && instance { get_instance() };

  // set it as a std::asyc  so the copy to image from a texture process don't slow down main loop updates
  // it could have optimizations and better implementation.
  instance.voidFutures.emplace_back(std::async(std::launch::async, Helpers::__saveScreenShot, texture, capsN));
}
