# include <Ghost.hpp>

Ghost::Ghost (const std::string & colorPath, sf::Vector2f initPos, sf::Vector2f originPos)
: m_initPos(initPos), m_originPos(originPos)
{
  std::string defaultPath = "Ghosts/" + colorPath + "/";
  std::string deathPath = "Ghosts/EyeWalking/";

  // normal movement animation
  m_animator.createAnimation("W", sf::seconds(.5f), true);
  m_animator.addFrames("W", defaultPath + "Up");
  m_animator.createAnimation("S", sf::seconds(.5f), true);
  m_animator.addFrames("S", defaultPath + "Down");
  m_animator.createAnimation("A", sf::seconds(.5f), true);
  m_animator.addFrames("A", defaultPath + "Left");
  m_animator.createAnimation("D", sf::seconds(.5f), true);
  m_animator.addFrames("D", defaultPath + "Right");

  // eye walking animation
  m_animator.createAnimation("W_dead", sf::seconds(.5f), true);
  m_animator.addFrames("W_dead", deathPath + "Up");
  m_animator.createAnimation("S_dead", sf::seconds(.5f), true);
  m_animator.addFrames("S_dead", deathPath + "Down");
  m_animator.createAnimation("A_dead", sf::seconds(.5f), true);
  m_animator.addFrames("A_dead", deathPath + "Left");
  m_animator.createAnimation("D_dead", sf::seconds(.5f), true);
  m_animator.addFrames("D_dead", deathPath + "Right");

  // ...
  m_animator.createAnimation("scared", sf::seconds(.5f), true);
  m_animator.addFrames("scared", "Ghosts/ScaredGhost/default");

  // this is the one setting when we are dead on base, this is not permanent
  m_animator.createAnimation("empty", sf::seconds(.5f), true);
  m_animator.addFrames("empty", "Ghosts/empty");

  // we get the first frame to correctly center our sprite
  setTexture(m_animator.getFrame(), true);
  setOrigin(Helpers::getSpriteLocalCenter(*this));
  setPosition(m_initPos);

  // which behavior to star with
  makeChoice();

  // we hare setting ghost life to 1
  attacked(); restoreAttackable();
  attacked(); restoreAttackable();
  // to desactivate attacked flag
  recentlyAttacked();
  // ghost can't be killed unless a big dot is eaten
  inmortal();
}

namespace Helpers
{
  int ManhattanDistance (const sf::Vector2i & origin, const sf::Vector2i & destiny)
  {
    return std::abs(origin.x - destiny.x) + std::abs(origin.y - destiny.y);
  }

  void decrease (int & n)
  {
    if(n > 0)
      --n;
    else if(n < 0)
      ++n;
  }

  template<int N>
  void updatePathFinding (Ghost & ghost, TileMap& map, const Entity & pacman)
  {
    std::cerr << "invalid updatePathFinding: " << N << std::endl;
  }

  template<> // follow pacman
  void updatePathFinding<0>(Ghost & ghost, TileMap& map, const Entity & pacman)
  {
    updatePath(ghost, map, pacman.getPosition());
  }

  template<> // tacle pacman, 4 steps in front of him
  void updatePathFinding<1>(Ghost & ghost, TileMap& map, const Entity & pacman)
  {
    sf::Vector2i tileSize = map.getTileSize();

    float rotation = pacman.getRotation();

    Tile *origin = map.getTile(ghost.getPosition().x / tileSize.x, ghost.getPosition().y / tileSize.y);
    Tile *end = map.getTile(pacman.getPosition().x / tileSize.x, pacman.getPosition().y / tileSize.y);

    // if it is in front of him, then tacle him xDDD
    if(std::abs(origin->getPos().x - end->getPos().x) <= 4 and std::abs(origin->getPos().y - end->getPos().y) <= 4)
      updatePathFinding<0>(ghost, map, pacman);
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
        goalNode = map.getTile(pacman.getPosition().x / tileSize.x + sum.x, pacman.getPosition().y / tileSize.y + sum.y);

        decrease(sum.x);
        decrease(sum.y);

        if(goalNode != nullptr and not goalNode->isPath()) // if the goalNode is not a path, we try it again
          goalNode = nullptr;
      }while(goalNode == nullptr);

      if(sum.x == 0 and sum.y == 0) // if not in front, then we just have to do de usual
        updatePathFinding<0>(ghost, map, pacman);
      else updatePath(ghost, map, sf::Vector2f(goalNode->getPos().x * tileSize.x, goalNode->getPos().y * tileSize.y ));
    }
  }

  template<> // random positions across the map
  void updatePathFinding<2>(Ghost & ghost, TileMap& map, const Entity & pacman)
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

void Ghost::update (const sf::Time & dt, TileMap & map, const Entity & pacman)
{
  m_deadTime -= dt;

  // if we got attacked, we move to our initial position
  if(recentlyAttacked())
  {
    m_velocity = 100.f;
    Helpers::updatePath(*this, map, m_initPos);
    m_gettingAttackableTime = false;
  }

  // we have just 1 hp so, getting hitted will alwys kill us
  if(dead())
  {
    // distance between us and our initial position
    sf::Vector2f distance { m_initPos.x - getPosition().x, m_initPos.y - getPosition().y };

    /* if we hare at home and havent setted m_gettingAttackableTime flag:
     * we set that flat and establish a dead time, this dead time is the one we are
     * going to be totally dead
     */
    if(std::abs(distance.x) < 3.f and std::abs(distance.y) < 3.f and not m_gettingAttackableTime)
    {
      m_gettingAttackableTime = true;
      m_deadTime = sf::seconds(5);
    }

    /* if our m_gettingAttackableTime was already setted(we are at home)
     * and our dead time has overed
     */
    if(m_gettingAttackableTime and m_deadTime <= sf::Time::Zero)
    {
      // restore velocity, our hp, our attackable status and we become inmortal once again
      healted();
      restoreAttackable();
      inmortal();

      // and of course, restore our timers
      m_deadTime = m_attackableTime = sf::Time::Zero;
    }

    movePos(dt);
    updateAnimation(dt);

    // we return because we don't want to do anything else here
    return;
  }

  // if we are attackable and our m_gettingAttackableTime flag is not setted
  if(attackable() and not m_gettingAttackableTime)
  {
    // we set it on
    m_gettingAttackableTime = true;
    // we set our dead time which says that pacman can attack us
    m_attackableTime = sf::seconds(3);
    // our dead time is zero
    m_deadTime = sf::Time::Zero;
    // and we slow ourself down a bit, because we are scarreeed buuuuhhh
    m_velocity = 70.f;
  }

  // if our attackable time has overed
  if(m_attackableTime <= sf::Time::Zero)
  {
    // we become inmortal once again
    inmortal();
    // restore our velocity
    m_velocity = 100.f;
    // restore our attackable clock and flag
    m_attackableTime = sf::Time::Zero;
    m_gettingAttackableTime = false;
  }

  // if our deadTime is overed, we change behavior
  if(m_deadTime <= sf::Time::Zero)
    makeChoice();

  /* if we are attackable, we don't wanna run to pacman xD
   * so we set ourself in random behavior
   * and update our attackable timer
   */
  if(attackable())
  {
    m_choice = 2;
    m_attackableTime -= dt;
  }

  /* if we are at random, we don't want to update our path at each fucking frame
   * so we do it when our behavior chagnes(with makeChoice()) or when we don't
   * have a target
   */
  if(m_choice >= 2 and (m_randomActive or m_targetPositions.empty()))
  {
    Helpers::updatePathFinding<2>(*this, map, pacman);

    // we make sure to unset our m_randomActive flag
    m_randomActive = false;
  }
  else if(m_choice == 0) // follow
  {
    // if pacman is not attackable, he is dead, we became random
    if(not pacman.attackable())
      m_choice = 2;
    else Helpers::updatePathFinding<0>(*this, map, pacman);
  }
  else if(m_choice == 1) // tackle
  {
    // if pacman is not attackable, he is dead, we became random
    if(not pacman.attackable())
      m_choice = 2;
    else Helpers::updatePathFinding<1>(*this, map, pacman);
  }

  movePos(dt);
  updateAnimation(dt);
}

void Ghost::movePos (const sf::Time & dt)
{
  if(not m_targetPositions.empty())
  {
    sf::Vector2f targetLocation = m_targetPositions.front();
    sf::Vector2f distance { targetLocation.x - getPosition().x,
    targetLocation.y - getPosition().y };

    // if we are really close to our target, we change position
    if(std::abs(distance.x) < 3.f and std::abs(distance.y) < 3.f)
      m_targetPositions.erase(m_targetPositions.begin());
    else // move
    {
      // we get our pitagoras formula for distance xD
      float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);

      // TODO: learn about normalizing a vector, the thing we are doing here
      distance.x /= length;
      distance.y /= length;

      sf::Vector2f movement { distance.x * m_velocity * dt.asSeconds(),
                                                distance.y * m_velocity * dt.asSeconds() };

      updateAnimation(movement);
      move(movement);
    }
  }
}

void Ghost::makeChoice ()
{
  m_choice = Helpers::random(0, 4); // 0 and 1 are pacman following behaviors
  m_deadTime = sf::seconds(Helpers::random(2.f, 5.f)); // each behavior will last 2 ~ 5 seconds
  m_randomActive = true;
}

void Ghost::updateAnimation (const sf::Vector2f & movement)
{
  if(dead() and not m_gettingAttackableTime)
  {
    if(movement.y < -1.f)
      m_animator.switchAnimation("W_dead", true);
    else if(movement.y > 1.f)
      m_animator.switchAnimation("S_dead", true);
    else if(movement.x < -1.f)
      m_animator.switchAnimation("A_dead", true);
    else if(movement.x > 1.f)
      m_animator.switchAnimation("D_dead", true);
  }

  else if(attackable())
    m_animator.switchAnimation("scared", true);

  else if(movement.y < -1.f)
    m_animator.switchAnimation("W", true);
  else if(movement.y > 1.f)
    m_animator.switchAnimation("S", true);
  else if(movement.x < -1.f)
    m_animator.switchAnimation("A", true);
  else if(movement.x > 1.f)
    m_animator.switchAnimation("D", true);
}

void Ghost::updateAnimation (const sf::Time & dt)
{
  /* we are putting this animation switching here because when standing,
   * we do not call to updateAnimation(sf::Vector2f), the one in charge of
   * switching animations.
   */
  if(dead() and m_gettingAttackableTime)
    m_animator.switchAnimation("empty", true);

  m_animator.update(dt);
  setTexture(m_animator.getFrame(), true);
}

namespace Helpers
{
  // A* implementation
  void updatePath (Ghost & ghost, TileMap& map, const sf::Vector2f & targetPos)
  {
    // get the target node and the origin node;
    sf::Vector2i tileSize = map.getTileSize();
    Tile* startNode = map.getTile(ghost.getPosition().x / tileSize.x, ghost.getPosition().y / tileSize.y);
    Tile* goalNode = map.getTile(targetPos.x / tileSize.x, targetPos.y / tileSize.y);

    // if we are at our destiny, we don't need to find a path to it
    if(map.getTile(ghost.m_currentTarget.x / tileSize.x, ghost.m_currentTarget.y / tileSize.y) == goalNode)
      return;

    // updating our target
    ghost.m_currentTarget = targetPos;

    // we need to reset all map nodes so we have fresh H, F and G values
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

    // we are calculating the distances between each node and the goalNode, the H value
    sf::Vector2i mapSize = map.getTileCount();
    for (int y = 0; y < mapSize.y; ++y)
      for (int x = 0; x < mapSize.x; ++x)
      {
        Tile* node = map.getTile(x, y);
        node->H = ManhattanDistance(node->getPos(), goalNode->getPos());
      }

    // Add the start node to the open list.
    openList.push_back(startNode);

    // while we have nodes to check
    while(not openList.empty())
    {
      /* we search four our lowest F value, and mark that one as our current.
       * with the first node, it'll be by default, the best F value, it's the only one and
       * it's f vaue is zero
       */
      int lowestF = 9999999;

      for(auto && node : openList)
        if(node->F < lowestF)
        {
          lowestF = node->F;
          currentNode = node;
        }

      position = std::find(openList.begin(), openList.end(), currentNode);
      /* if currentNode is in the open list, we take it out because it is already checked.
       * And move it to the closed list, nodes alredy checked
       */
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

      // we now check for adjacent nodes that are not in the open, or closed list
      for(auto && node : adjacentTiles)
      {
        // if this node is our goald node, we stop right here
        if(node == goalNode)
        {
          // we set the nodes parent tou our current
          node->parentNode = currentNode;

          // and start adding nodes to our path
          while (node->parentNode != nullptr)
          {
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
          position = std::find(closedList.begin(), closedList.end(), node);
          /* if node is in the closed list, there is nothing to do: node checked
           * but, if it is not in the closed list, it is a node that hasn't been checked
           */
          if (position == closedList.end())
          {
            // we start by searching it on our open list

            position = std::find(openList.begin(), openList.end(), node);

            // if the node is not in the open list, we have to get it's F and G values
            if (position == openList.end())
            {
              // Add the node to the open list.
              openList.push_back(node);

              /* Set the parent of the node to the current node.
               * because we are going from in to out, so we parent all adjacent nodes with
               * our currentNode, of course, if node is already checked, it means he has already
               * a father so we ca not do that, that's why we ensure the node we are checking
               * is not in the openlist
               */
              node->parentNode = currentNode;

              // every step we make, costs us 10 points
              node->G = currentNode->G + 10;

              // we sum up our step cost and our cost from going to this step to our destiny
              node->F = node->H + node->H;
            }
            // but if the node is in the open list, it means it's G and F value has been prepared already
            else
            {
              /* we create a temporal G value, that would mean the cost of moving from that
               * position
               */
              int tempG = currentNode->G + 10;
              /* if the cost of moving to that position from currentNode is cheaper than moving
               * from the one it is already setted
               * I.e, whether it's faster to go A->C->B that A->C.
               */
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
