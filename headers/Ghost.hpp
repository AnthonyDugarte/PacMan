# pragma once

# include <Animation.hpp>
# include <Entity.hpp>
# include <Animator.hpp>

# include <cmath>

class Ghost;

namespace Helpers
{
  void updatePath(Ghost & ghost, TileMap& map, const sf::Vector2f & targetPos);
}

class Ghost : public Entity
{
public:
  // initPos = position to initiate behavior, originPos = position inside ghost house
  Ghost (const std::string &,sf::Vector2f, sf::Vector2f);

  void update (const sf::Time &, TileMap &, const Entity &);

private:
  void updateAnimation (const sf::Vector2f &);
  void updateAnimation (const sf::Time &);

  std::vector<sf::Vector2f> m_targetPositions;
  sf::Vector2f m_currentTarget;

  sf::Time deadTime;
  bool randomActive;
  int choice;

  Animator animator;
  sf::Vector2f m_initPos;
  sf::Vector2f m_originPos;

  void makeChoice();

  friend void Helpers::updatePath(Ghost &, TileMap&, const sf::Vector2f &);
};
