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
  Ghost (const std::string & colorPath, sf::Vector2f initPos, sf::Vector2f originPos);

  void update (const sf::Time & dt, TileMap & map, const Entity & pacman);

private:
  void updateAnimation (const sf::Vector2f & movement);
  void updateAnimation (const sf::Time & dt);

  std::vector<sf::Vector2f> m_targetPositions;
  sf::Vector2f m_currentTarget;

  sf::Time m_deadTime;
  bool m_randomActive;
  int m_choice;

  Animator m_animator;
  sf::Vector2f m_initPos;
  sf::Vector2f m_originPos;

  void makeChoice();

  friend void Helpers::updatePath(Ghost &, TileMap&, const sf::Vector2f &);
};
