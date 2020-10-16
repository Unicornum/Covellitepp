
#pragma once
#include <Covellite/Covellite.hpp>
#include "Common.hpp"

namespace events
{

class Simple2DGame_t
{
public:
  enum Id
  {
    Rect = 0,
    Start,
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
*  2.0.0.0        \n
* \date
*  29 январь 2019    \n
*  22 »юнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019 - 2020
*/
class Simple2DGame final :
  public Common
{
  using WindowExpanse_t = ::covellite::expanse::IWindow;
  using Events_t = ::covellite::events::Events;
  /// [Vertex format]
  using Vertex_t = ::covellite::api::Vertex;
  /// [Vertex format]
  using VertexData_t = ::std::vector<Vertex_t>;

public:
  class Rect final
  {
  public:
    float Left;
    float Top;
    float Right;
    float Bottom;
  };

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
      inline float GetMin(void) const noexcept { return Position - Size / 2.0f; }
      inline float GetMax(void) const noexcept { return Position + Size / 2.0f; }
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
  ObjectId_t AddCamera(float, float);
  ObjectId_t AddBackground(void);
  ObjectId_t AddClock(void);
  void AddActors(void);
  GameObject_t BuildRectangle(const GameObject_t &, float, float, const String_t &);
  GameObject_t BuildRectangle(const GameObject_t &, float, float, float, float,
    float, float);
  GameObject_t BuildRectangle(const Rect &, const Rect &, float, float, float,
    float, const String_t &, const GameObject_t &);
  GameObject_t BuildRectangle(const Rect &, const Rect &, float, float, float, float,
    const GameObject_t &, const GameObject_t &);

private:
  Events_t m_Events;
  bool m_IsTouch = false;
  float m_MouseX = 0.0f;
  float m_MouseY = 0.0f;

private:
  const float m_Width;
  const float m_Height;
  const float m_GameFieldSize;

private:
  ObjectId_t                m_CameraId;
  ObjectId_t                m_BackGroundId;
  ObjectId_t                m_ClockId;
  ObjectId_t                m_EnemiesId;
  ObjectId_t                m_GreenSquareId;
  ObjectId_t                m_RedSquareId;
  GameUnit                  m_UserUnit;
  ::std::vector<GameUnit>   m_Enemies;
  float                     m_EnemiesSpeed = 0.1f;
  float                     m_BeginGameTime;
  float                     m_GameTime;
  float                     m_LastTime;

public:
  Simple2DGame(WindowExpanse_t &, const Rect &);
  Simple2DGame(const Simple2DGame &) = delete;
  Simple2DGame(Simple2DGame &&) = delete;
  const Simple2DGame & operator= (const Simple2DGame &) = delete;
  Simple2DGame & operator= (Simple2DGame &&) = delete;
  ~Simple2DGame(void);
};

} // namespace basement
