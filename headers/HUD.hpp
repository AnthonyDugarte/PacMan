# pragma once

# include <AssetManager.hpp>
# include <Attributes.hpp>

class HUD : public sf::drawable
{
  public:
    HUD(const Attributes & );

private:
  virtual void draw(sf::RenderTarget &, sf::RenderStates) const;

  const Attributes & m_attributes;
}
