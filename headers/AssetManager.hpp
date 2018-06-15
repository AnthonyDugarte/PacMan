# pragma once

# include <Singleton.hpp>

# include <map>
# include <string>
# include <list>
# include <exception>
# include <fstream>
# include <future>

# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>

class AssetManager : public Designar::Singleton<AssetManager>
{
protected:
  AssetManager();

public:
  ~AssetManager();

  static sf::Texture & getTexture(const std::string &);
  static std::fstream getFile(const std::string &);

  static void saveScreenShot (sf::Texture &&, size_t);

private:
  std::map <std::string, sf::Texture> m_textures;
  std::list <std::future<void>> voidFutures;

  friend class Singleton<AssetManager>;
};
