
#pragma once

#ifdef GetObject
# undef GetObject
#endif

#include <vector>
#include <Covellite/Api/Renders.hpp>

namespace boost { class any; }

namespace basement
{

using Id_t = size_t;
using Objects_t = ::std::vector<Id_t>;
using Component_t = ::covellite::api::Component;
using ComponentPtr_t = ::std::shared_ptr<Component_t>;
using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;
using Object_t = ::std::vector<ComponentPtr_t>;
using Render_t = ::std::function<void(void)>;
using Renders_t = ::std::vector<Render_t>;
using Updater_t = ::std::function<void(void)>;
using Updaters_t = ::std::vector<Updater_t>;
using Any_t = ::boost::any;

namespace support { class GameScene; }

using GameScenePtr_t = ::std::shared_ptr<support::GameScene>;

class Point final
{
public:
  inline Point operator + (const Point & _Point) const
  {
    return Point{ x + _Point.x, y + _Point.y, z + _Point.z };
  }

public:
  const float x;
  const float y;
  const float z;

public:
  explicit Point(
    const float _X = 0.0f, 
    const float _Y = 0.0f, 
    const float _Z = 0.0f) :
    x(_X),
    y(_Y),
    z(_Z)
  {

  }
};

class Rect final
{
public:
  float left, top, right, bottom;
};

} // namespace basement