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

  static sf::Texture & getTexture (const std::string &);
  static std::fstream getFile (const std::string &);

  static sf::Uint8 * getBitMask (const std::string &);
  static sf::Uint8 * getBitMask (const sf::Texture *);

  // TODO: save files and store them into m_textures, instead of just pictures
  static void saveScreenShot (sf::Texture, size_t);

private:
  std::map <std::string, sf::Texture> m_textures;
  std::map<const sf::Texture *, sf::Uint8 *> m_bitmasks;

  std::list <std::future<void>> voidFutures;

  friend class Singleton<AssetManager>;
};
