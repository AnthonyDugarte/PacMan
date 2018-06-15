# include <AssetManager.hpp>

AssetManager::AssetManager()
  : m_textures()
{
  // empty
}

AssetManager::~AssetManager()
{
  // empty
}

sf::Texture & AssetManager::getTexture(const std::string & name)
{
  //Manager instance
  auto && instance { get_instance() };

  auto && search{ instance.m_textures.find(name) };
  if (search != instance.m_textures.end())
    return search->second;

    std::string prefix{ "assets/" };

  sf::Texture new_texture;
  if (not new_texture.loadFromFile(prefix + name))
    throw std::invalid_argument("texture could not be loaded: " + prefix + name);

  new_texture.setSmooth(true);

  instance.m_textures[name] = std::move(new_texture);

  return *&instance.m_textures[name];
}

std::fstream AssetManager::getFile(const std::string & filename)
{
  std::string prefix{ "assets/" };

  std::fstream file( prefix + filename);

  if(not file.is_open())
    throw std::invalid_argument("file could not be opened: " + prefix + filename);

  return std::move(file);
}


namespace Helpers
{
  void __saveScreenShot (sf::Texture && texture, size_t capsN)
  {
    std::string prefix{ "assets/Captures/capture" }, sufix{ ".png" };

    sf::Image screenshot { texture.copyToImage() };
    screenshot.saveToFile(prefix + std::to_string(capsN) + sufix);
  }
}

void AssetManager::saveScreenShot (sf::Texture && texture, size_t capsN)
{
  auto && instance { get_instance() };

  // set it as a std::asyc  so the copy to image from a texture process don't slow down main loop updates
  // it could have optimizations and better implementation.
  instance.voidFutures.emplace_back(std::async(std::launch::async, Helpers::__saveScreenShot, texture, capsN));
}
