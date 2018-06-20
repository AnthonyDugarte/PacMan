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
  static std::fstream getFile (const std::string & path);

  static sf::Uint8 * getBitMask (const std::string & path);
  static sf::Uint8 * getBitMask (const sf::Texture * texture);

  // TODO: save files and store them into m_textures, instead of just pictures
  static void saveScreenShot (sf::Texture, size_t);

private:
  std::map<std::string, sf::Texture> m_textures;
  std::map<std::string, sf::Font> m_fonts;

  std::map<const sf::Texture *, sf::Uint8 *> m_bitmasks;

  std::list <std::future<void>> m_voidFutures;

  friend class Singleton<AssetManager>;
};
