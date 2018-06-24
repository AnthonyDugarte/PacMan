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

  while(not m_files.empty())
  {
    m_files.begin()->second.close();
    m_files.erase(m_files.begin());
  }
}

sf::Texture & AssetManager::getTexture (const std::string & path)
{
  //Manager instance
  auto && instance { get_instance() };

  auto && search{ instance.m_textures.find(path) };
  if (search != instance.m_textures.end())
    return search->second;

  auto && new_texture { instance.m_textures[path] };

  std::string prefix{ "assets/" };
  if (not new_texture.loadFromFile(prefix + path))
  {
    instance.m_textures.erase(path);
    throw std::invalid_argument("texture could not be loaded: " + prefix + path);
  }

  new_texture.setSmooth(true);
  return new_texture;
}

sf::Font & AssetManager::getFont (const std::string & path)
{
  auto && instance { get_instance() };

  auto && search{ instance.m_fonts.find(path) };
  if(search != instance.m_fonts.end())
    return search->second;

  auto && new_font { instance.m_fonts[path] };

  std::string prefix("assets/font/");
  if(not new_font.loadFromFile(prefix + path))
  {
    instance.m_fonts.erase(path);
    throw std::invalid_argument("texture could not be loaded: " + prefix + path);
  }

  return new_font;
}

sf::Music & AssetManager::getMusic(const std::string & path, bool looping)
{
  auto && instance { get_instance() };

  auto && search{ instance.m_musics.find(path) };
  if(search != instance.m_musics.end())
    return search->second;

  auto && new_music { instance.m_musics[path] };

  std::string prefix{ "assets/music/"};

  if(not new_music.openFromFile(prefix + path))
  {
    instance.m_musics.erase(path);
    throw std::invalid_argument("music could not be loaded: " + prefix + path);
  }

  new_music.setLoop(looping);
  return new_music;
}

std::fstream & AssetManager::getFile (const std::string & path)
{
  auto && instance { get_instance() };

  auto && search{ instance.m_files.find(path) };
  if(search != instance.m_files.end())
  {
    search->second.clear();
    search->second.seekg(0, std::ios::beg);
    return search->second;
  }

  std::string prefix{ "assets/" };
  auto && file { instance.m_files[path] };
  file.open(prefix + path);

  if(not file.is_open())
  {
    instance.m_files.erase(path);
    throw std::invalid_argument("file could not be opened: " + prefix + path);
  }

  return file;
}

namespace Helpers
{
  const std::pair<const sf::Texture *, sf::Uint8 *> getMask (const sf::Texture *);
}

sf::Uint8 * AssetManager::getBitMask (const std::string & path)
{
  //Manager instance
  auto && instance { get_instance() };

  sf::Texture * bitMaskTexture;

  //we charge our texture on the Manager so we have just one instance of bitMask per Texture
  try
  {
    bitMaskTexture = &getTexture(path);
  }
  catch(...)
  {
    throw std::invalid_argument("bitMask could not be loaded: " + path);
  }

  auto && search{ instance.m_bitmasks.find(&*bitMaskTexture) };
  if(search != instance.m_bitmasks.end())
    return search->second;

  auto && new_mask{ Helpers::getMask(&*bitMaskTexture) };

  instance.m_bitmasks.insert(std::move(new_mask));
  return *&new_mask.second;
}

sf::Uint8 * AssetManager::getBitMask (const sf::Texture * texture)
{
  //Manager instance
  auto && instance { get_instance() };

  auto && search{ instance.m_bitmasks.find(&*texture) };
  if(search != instance.m_bitmasks.end())
    return &*search->second;

  auto && new_mask { Helpers::getMask(&*texture) };

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
