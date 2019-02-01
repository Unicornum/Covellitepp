
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
  using Rect_t = ::covellite::os::IWindow::Rect;
  using Vertex_t = ::covellite::api::Vertex::Gui;
  using VertexData_t = ::std::vector<Vertex_t>;
  using Object_t = ::std::vector<Component_t::ComponentPtr_t>;
  using Events_t = ::covellite::events::Events;

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
    };

  public:
    bool IsIntersect(const GameUnit &) const;

  public:
    Info X, Y;
    const Component_t::ComponentPtr_t m_pPosition;

  public:
    GameUnit(float, float, float, float, float);
  };

private:
  float GetScreenX(float) const;
  float GetScreenY(float) const;
  float GetCenterGameFieldUnitX(float) const;
  float GetCenterGameFieldUnitY(float) const;
  Id BuildCamera(void);
  Id BuildRectangle(const Component_t::ComponentPtr_t &, 
    float, float, const String_t &);
  Id BuildRectangle(const Component_t::ComponentPtr_t &, 
    float, float, uint32_t);
  Id BuildRectangle(const Component_t::ComponentPtr_t &, 
    const Object_t &, const VertexData_t &);

private:
  Events_t m_Events;
  bool m_IsTouch = false;
  float m_MouseX = 0.0f;
  float m_MouseY = 0.0f;
  const float m_Xo;
  const float m_Yo;
  const float m_Width;
  const float m_Height;
  const float m_GameFieldSize;
  GameUnit m_UserUnit;
  ::std::vector<GameUnit> m_Enemies;
  float m_Speed = 0.1f;
  ::std::function<void(void)> m_Updater;
  ::std::chrono::system_clock::time_point m_LastTime;
  ::std::chrono::system_clock::time_point m_BeginRenderFrameTime;
  ::std::chrono::system_clock::time_point m_BeginGameTime;

public:
  Simple2DGame(const Events_t &, const RendersPtr_t &, const Rect_t &);
};

} // namespace basement
