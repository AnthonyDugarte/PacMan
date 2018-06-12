# pragma once

# include <Singleton.hpp>

# include <map>
# include <string>
# include <exception>
# include <fstream>

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

private:
  std::map <std::string, sf::Texture> m_textures;

  friend class Singleton<AssetManager>;
};
