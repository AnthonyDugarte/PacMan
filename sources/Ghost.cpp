# include <Ghost.hpp>

Ghost::Ghost (const std::string & color, sf::Vector2f initPos, sf::Vector2f originPos)
: m_initPos(initPos), m_originPos(originPos)
{
  std::string animPack = "Ghosts/" + color + "/";

  animator.createAnimation("W", sf::seconds(.5f), true);
  animator.addFrames("W", animPack + "Up");

  animator.createAnimation("S", sf::seconds(.5f), true);
  animator.addFrames("S", animPack + "Down");

  animator.createAnimation("A", sf::seconds(.5f), true);
  animator.addFrames("A", animPack + "Left");

  animator.createAnimation("D", sf::seconds(.5f), true);
  animator.addFrames("D", animPack + "Right");

  setTexture(animator.getFrame(), true);
  setOrigin(Helpers::getSpriteLocalCenter(*this));
  setPosition(m_initPos);

  makeChoice();
}

namespace Helpers
{
  int ManhattanDistance(const sf::Vector2i & origin, const sf::Vector2i & destiny)
  {
    return std::abs(origin.x - destiny.x) + std::abs(origin.y - destiny.y);
  }

  void decrease(int & n)
  {
    if(n > 0)
      --n;
    else if(n < 0)
      ++n;
  }

  template<int N>
  void updatePathFinding(Ghost & ghost, TileMap& map, const Entity & player)
  {
    std::cerr << "invalid updatePathFinding: " << N << std::endl;
  }

  template<> // follow player
  void updatePathFinding<0>(Ghost & ghost, TileMap& map, const Entity & player)
  {
    updatePath(ghost, map, player.getPosition());
  }

  template<> // tacle player, 4 steps in front of him
  void updatePathFinding<1>(Ghost & ghost, TileMap& map, const Entity & player)
  {
    sf::Vector2i tileSize = map.getTileSize();

    float rotation = player.getRotation();

    Tile *origin = map.getTile(ghost.getPosition().x / tileSize.x, ghost.getPosition().y / tileSize.y);
    Tile *end = map.getTile(player.getPosition().x / tileSize.x, player.getPosition().y / tileSize.y);

    // if it is in front of him, then tacle him xDDD
    if(std::abs(origin->getPos().x - end->getPos().x) <= 4 and std::abs(origin->getPos().y - end->getPos().y) <= 4)
      updatePathFinding<0>(ghost, map, player);
    else
    {
      int steps = 4;

      sf::Vector2i sum { 0, 0 };
      if(rotation == 0.f)
        sum.x = steps;
      else if(rotation == 180.f)
        sum.x = -steps;
      else if(rotation == 90.f)
        sum.y = steps;
      else if(rotation == 270.f)
        sum.y = -steps;

      Tile* goalNode  = nullptr;

      do
      {
        goalNode = map.getTile(player.getPosition().x / tileSize.x + sum.x, player.getPosition().y / tileSize.y + sum.y);

        decrease(sum.x);
        decrease(sum.y);

        if(goalNode != nullptr and not goalNode->isPath()) // if the goalNode is not a path, we try it again
          goalNode = nullptr;
      }while(goalNode == nullptr);

      if(sum.x == 0 and sum.y == 0) // if not in front, then we just have to do de usual
        updatePathFinding<0>(ghost, map, player);
      else updatePath(ghost, map, sf::Vector2f(goalNode->getPos().x * tileSize.x, goalNode->getPos().y * tileSize.y ));
    }
  }

  template<> // random positions across the map
  void updatePathFinding<2>(Ghost & ghost, TileMap& map, const Entity & player)
  {
    sf::Vector2i mapSize = map.getTileCount();
    sf::Vector2i tileSize = map.getTileSize();

    Tile *newPos = nullptr;

    while(newPos == nullptr)
    {
      newPos = map.getTile(random(0, mapSize.x - 1),random(0, mapSize.y - 1));

      if(not newPos->isPath())
        newPos = nullptr;
    }

    updatePath(ghost, map, sf::Vector2f(newPos->getPos().x * tileSize.x, newPos->getPos().y * tileSize.y ));
  }
}

void Ghost::update (const sf::Time & dt, TileMap & map, const Entity & player)
{
  deadTime -= dt;

  if(choice >= 2 and (randomActive or m_targetPositions.empty()))
  {
    Helpers::updatePathFinding<2>(*this, map, player);
    randomActive = false;
  }
  else if(choice == 0)
  {
    if(not player.attackable())
      choice = 2;
    else Helpers::updatePathFinding<0>(*this, map, player);
  }
  else if(choice == 1)
  {
    if(not player.attackable())
      choice = 2;
    else Helpers::updatePathFinding<1>(*this, map, player);
  }

  if(deadTime < sf::seconds(0))
    void makeChoice();

  if(not m_targetPositions.empty())
  {
    sf::Vector2f targetLocation = m_targetPositions.front();
    sf::Vector2f distance { targetLocation.x - getPosition().x,
    targetLocation.y - getPosition().y };

    if(std::abs(distance.x) < 5.f and std::abs(distance.y) < 5.f)
      m_targetPositions.erase(m_targetPositions.begin());
    else
    {
      float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);
      distance.x /= length;
      distance.y /= length;

      sf::Vector2f movement { distance.x * m_velocity * dt.asSeconds(),
                                                distance.y * m_velocity * dt.asSeconds() };

      updateAnimation(movement);
      move(movement);
    }
  }
  updateAnimation(dt);
}

void Ghost::makeChoice()
{
  choice = Helpers::random(0, 4); // 0 and 1 are player following behaviors
  deadTime = sf::seconds(Helpers::random(2.f, 5.f)); // each behavior will last 2 ~ 5 seconds
  randomActive = true;
}


void Ghost::updateAnimation (const sf::Vector2f & movement)
{
  if(movement.y < -1.f)
    animator.switchAnimation("W", true);
  else if(movement.y > 1.f)
    animator.switchAnimation("S", true);
  else if(movement.x < -1.f)
    animator.switchAnimation("A", true);
  else if(movement.x > 1.f)
    animator.switchAnimation("D", true);
}

void Ghost::updateAnimation (const sf::Time & dt)
{
  animator.update(dt);
  setTexture(animator.getFrame(), true);
}

namespace Helpers
{
  // A* implementation
  void updatePath(Ghost & ghost, TileMap& map, const sf::Vector2f & targetPos)
  {
    // get the target node and the origin node;
    sf::Vector2i tileSize = map.getTileSize();
    Tile* startNode = map.getTile(ghost.getPosition().x / tileSize.x, ghost.getPosition().y / tileSize.y);
    Tile* goalNode = map.getTile(targetPos.x / tileSize.x, targetPos.y / tileSize.y);

    if(map.getTile(ghost.m_currentTarget.x / tileSize.x, ghost.m_currentTarget.y / tileSize.y) == goalNode)
      return;

    ghost.m_currentTarget = targetPos;

    // we need to reset all map nodes
    map.resetNodes();

    // if we are at the same node, there is nothing to do
    if(startNode == goalNode)
    {
      ghost.m_targetPositions.clear();
      return;
    }

    std::vector<Tile*> openList;
    std::vector<Tile*> closedList;
    std::vector<Tile*> pathList;
    std::vector<Tile*>::iterator position;
    Tile* currentNode;

    // we are calculating the distances between each node and the goalNode
    sf::Vector2i mapSize = map.getTileCount();
    for (int y = 0; y < mapSize.y; ++y)
      for (int x = 0; x < mapSize.x; ++x)
      {
        Tile* node = map.getTile(x, y);
        node->H = ManhattanDistance(node->getPos(), goalNode->getPos());
      }

    // Add the start node to the open list.
    openList.push_back(startNode);

    while(not openList.empty())
    {
      int lowestF = 9999999;

      for(auto && node : openList)
        if(node->F < lowestF)
        {
          lowestF = node->F;
          currentNode = node;
        }

      position = std::find(openList.begin(), openList.end(), currentNode);
      // if currentNode is in the open list, we take it out because it is already checked
      if (position != openList.end())
        openList.erase(position);
      closedList.push_back(currentNode);

      // getting adjacent Tiles
      std::vector<Tile*> adjacentTiles;
      Tile* node;

      // top
      node = map.getTile(currentNode->getPos().x, currentNode->getPos().y - 1);
      if (node != nullptr and node->isPath())
        adjacentTiles.push_back(&*node);

      // down
      node = map.getTile(currentNode->getPos().x, currentNode->getPos().y + 1);
      if (node != nullptr and node->isPath())
        adjacentTiles.push_back(&*node);

      // left
      node = map.getTile(currentNode->getPos().x - 1, currentNode->getPos().y);
      if (node != nullptr and node->isPath())
        adjacentTiles.push_back(&*node);

      // right
      node = map.getTile(currentNode->getPos().x + 1, currentNode->getPos().y);
      if (node != nullptr and node->isPath())
        adjacentTiles.push_back(&*node);


      for(auto && node : adjacentTiles)
      {
        if(node == goalNode) // here we end
        {
          node->parentNode = currentNode;
          while (node->parentNode != nullptr)
          {
            // we add our list of nodes with the parents we set
            pathList.push_back(node);
            node = node->parentNode;
          }
          // we clear this list because that's the one that is being iterated in the main loop
          openList.clear();
          // and we break to get out of adjacent tiles loops
          break;
        }
        else // if it is not the goal node then...
        {
          // if node is not in the closed list, there is nothing to do
          position = std::find(closedList.begin(), closedList.end(), node);
          if (position == closedList.end())
          {
            // but If the node is not in the open list, we have to get their F and G values
            position = std::find(openList.begin(), openList.end(), node);
            if (position == openList.end())
            {
              // Add the node to the open list.
              openList.push_back(node);

              // Set the parent of the node to the current node.
              node->parentNode = currentNode;

              node->G = currentNode->G + 10;
              node->F = node->H + node->H;
            }
            else // if the node is in the open list
            {
              // Check if this path is quicker that the other.
              int tempG = currentNode->G + 10;
              // Check if tempG is faster than the other. I.e, whether it's faster to go A->C->B that A->C.
              if (tempG < node->G)
                node->parentNode = currentNode;
            } // node in open list end
          } // node not in closed list end
        } // not the goal node end
      } // adjacentTiles loop end
    } // while not open list empty end

    // we clear our targets
    ghost.m_targetPositions.clear();

    // we then add our path to the targets
    for(auto && tile : pathList)
      ghost.m_targetPositions.push_back(sf::Vector2f(tile->getPos().x * tileSize.x + (tileSize.x >> 1), tile->getPos().y * tileSize.y + (tileSize.y >> 1)));

    // path is in the wrong direction
    std::reverse(ghost.m_targetPositions.begin(), ghost.m_targetPositions.end());
  }
}
