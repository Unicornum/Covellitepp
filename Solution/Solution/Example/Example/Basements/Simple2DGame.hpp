
#pragma once
#include <Covellite/Covellite.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "Common.hpp"

namespace events
{

class Simple2DGame_t
{
public:
  enum Id
  {
    Start = 0,
    Finish,
  };
};

namespace { Simple2DGame_t Simple2DGame; }

} // namespace events

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс отрисовки простой 2D игры дл€ сло€ layers::Simple2DGame.
*
* \version
*  1.0.0.0        \n
* \date
*  29 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Simple2DGame final :
  public Common
{
  /// [Vertex format]
  using Vertex_t = ::covellite::api::Vertex::Polygon;
  /// [Vertex format]
  using VertexData_t = ::std::vector<Vertex_t>;
  using Object_t = ::std::vector<Component_t::ComponentPtr_t>;
  using Events_t = ::covellite::events::Events;
  using Updater_t = ::std::function<void(void)>;
  using TimePoint_t = ::std::chrono::system_clock::time_point;

public:
  class Rect final
  {
  public:
    float Left;
    float Top;
    float Right;
    float Bottom;
  };

public:
  void Render(void) override;

private:
  class GameUnit final
  {
  public:
    class Info
    {
    public:
      float Position;
      float Size;
      float SpeedFactor;

    public:
      inline float GetMin(void) const { return Position - Size / 2.0f; }
      inline float GetMax(void) const { return Position + Size / 2.0f; }
      void Update(float);
    };

  public:
    bool IsIntersect(const GameUnit &) const;
    void Update(float, float);
    void Update(float);

  public:
    Info X, Y;
    const Component_t::ComponentPtr_t m_pPosition;

  public:
    GameUnit(float, float, float, float, float);
  };

private:
  void AddCommonComponents(void);
  void AddCamera(void);
  void AddBackground(void);
  void AddClock(void);
  void AddActors(void);
  Id BuildRectangle(const Object_t &, float, float, const String_t &);
  Id BuildRectangle(const Object_t &, float, float, uint32_t);
  Id BuildRectangle(const Rect &, uint32_t, const Rect &,
    const String_t &, const Object_t &);
  Id BuildRectangle(const Rect &, uint32_t, const Rect &, 
    const Object_t &, const Object_t &);

private:
  Events_t m_Events;
  bool m_IsTouch = false;
  float m_MouseX = 0.0f;
  float m_MouseY = 0.0f;

private:
  const float m_Xo;
  const float m_Yo;
  const float m_Width;
  const float m_Height;
  const float m_GameFieldSize;

private:
  GameUnit                  m_UserUnit;
  ::std::vector<GameUnit>   m_Enemies;
  float                     m_EnemiesSpeed = 0.1f;
  Updater_t                 m_ClockUpdater;
  ::std::vector<Updater_t>  m_Updaters;
  TimePoint_t               m_LastTime;
  TimePoint_t               m_BeginRenderFrameTime;
  TimePoint_t               m_BeginGameTime;

public:
  Simple2DGame(const Events_t &, const RendersPtr_t &, const Rect &);
  ~Simple2DGame(void);
};

} // namespace basement
