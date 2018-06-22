# pragma once

# include <list>
# include <map>
# include <string>
# include <future>
# include <iostream>
# include <fstream>
# include <exception>

# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>

# include <Singleton.hpp>

class AssetManager : public Designar::Singleton<AssetManager>
{
protected:
  AssetManager ();

public:
  ~AssetManager ();

  // All files are taken from a path inside assets folder
  static sf::Texture & getTexture (const std::string & path);
  static sf::Font & getFont (const std::string & path);
  static std::fstream & getFile (const std::string & path);

  static sf::Uint8 * getBitMask (const std::string & path);
  static sf::Uint8 * getBitMask (const sf::Texture * texture);

  template<typename T>
  static void saveTexture (T&& texture, const std::string & path);

private:
  std::map<std::string, sf::Texture> m_textures;
  std::map<std::string, sf::Font> m_fonts;
  std::map<std::string, std::fstream> m_files;

  std::map<const sf::Texture *, sf::Uint8 *> m_bitmasks;

  /* async calls sit their return at the bottom of the scope they are called.
   * In eyes to avoid this, I simply created this little list that stores those guys who
   * sit in the bottom on the scope and wait until they are available, and then they are
   * destroyed. This way, they are going to be destroyed, of course, but at the end of
   * the main function and not inside little scopes inside all the mess I have
   */
  std::list <std::future<void>> m_voidFutures;

  friend class Singleton<AssetManager>;
};

namespace Helpers
{
  template<typename T>
  void __saveTexture (T&& texture, const std::string & path)
  {
    sf::Image img{ texture.copyToImage() };
    img.saveToFile(path);
  }
}

template<typename T>
void AssetManager::saveTexture (T&& texture, const std::string & path)
{
  auto && instance { get_instance() };

  std::string prefix("assets/");

  // set it as a std::asyc  so the copy to image from a texture process don't slow down main loop updates
  instance.m_voidFutures.emplace_back(std::async(std::launch::async,
    Helpers::__saveTexture<T>, std::forward<T>(texture), prefix + path));
}
