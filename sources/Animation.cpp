# include <Animation.hpp>

Animation::Animation (const sf::Time & duration, bool looping)
: m_duration(duration),
  m_looping(looping)
{
  // empty
}

Animation::Animation (const Animation & copy)
{
  for(auto && texture : copy.m_frames)
    m_frames.push_back(&*texture);

  m_duration = copy.m_duration;
  m_looping = copy.m_looping;
}

Animation::~Animation ()
{
  while(not m_frames.empty())
  {
    m_frames.back() = nullptr;
    m_frames.pop_back();
  }
}

void Animation::reset ()
{
  m_pos = 0;
  m_elapsed = sf::Time::Zero;
}

void Animation::setDuration (const sf::Time & duration)
{
  m_duration = duration;
}

void Animation::setLooping (bool looping)
{
  m_looping = looping;
}

sf::Texture & Animation::getFrame () const
{
  return *m_frames[m_pos];
}

sf::Texture & Animation::getFrame (size_t pos) const
{
  if(pos >= m_frames.size())
  {
    std::cerr << "wrong position, returing last";
    return *m_frames.back();
  }

  return *m_frames[pos];
}

const sf::Time & Animation::getDuration () const
{
  return m_duration;
}

const sf::Time & Animation::getElapsed () const
{
  return m_elapsed;
}

bool Animation::getLooping () const
{
  return m_looping;
}

size_t Animation::size () const
{
  return m_frames.size();
}

void Animation::addFrame (const std::string & path)
{
  m_frames.push_back(&AssetManager::getTexture(path));
}

void Animation::addFrames (const std::string & pre_path)
{
  std::fstream framesInfo { AssetManager::getFile(pre_path + "/frames.info") };

  std::string str;

  std::getline(framesInfo, str, '\n');
  size_t iterations = std::stoi(str);

  std::getline(framesInfo, str, '\n');
  bool reverse_animation;
  if(str == "T")
    reverse_animation = true;
  else if(str == "F")
    reverse_animation = false;
  else throw std::invalid_argument("not a valid condition: " + str + " on file: " + pre_path + "/frames.info");

  std::getline(framesInfo, str, '\n');
  std::string & sufix { str };

  for(size_t i { 0 }; i < iterations; ++i)
    addFrame(pre_path + "/" + std::to_string(i) + sufix);

  if(reverse_animation)
    for(size_t it{ size() }; --it; ) // frame 0 won't be setted twice, it's the starting point
      m_frames.push_back(&*m_frames[it]);
}

void Animation::update (const sf::Time & dt)
{
  m_elapsed += dt;

  float scaledTime { m_elapsed.asSeconds() / m_duration.asSeconds() };
  size_t numFrames { m_frames.size() };

  m_pos = scaledTime * numFrames;

  if(m_looping)
    m_pos %= numFrames;

  /* if the current frame is greater than the number of frames
   * show the last one
   */
  else if(m_pos >= numFrames)
    m_pos = numFrames - 1;
}
