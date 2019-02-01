
#include "stdafx.h"
#include "Simple2DGame.hpp"
#include <chrono>
#include <thread>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>

// 26 Ноябрь 2018 17:54 (unicornum.verum@gmail.com)
TODO("Недопустимое обращение к файлам другого проекта.");
#include <Covellite.Api/Covellite.Api/Renderer/fx/Pixel.auto.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Vertex.auto.hpp>

using namespace basement;
namespace math = ::alicorn::extension::cpp::math;

Simple2DGame::GameUnit::GameUnit(float _X, float _Y, 
  float _SizeX, float _SizeY, float _AngleDegree) :
  X(Info{ _X, _SizeX, math::degree::Cos(_AngleDegree) }),
  Y(Info{ _Y, _SizeY, math::degree::Sin(_AngleDegree) }),
  m_pPosition(Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
    }))
{
}

bool Simple2DGame::GameUnit::IsIntersect(const GameUnit & _Unit) const
{
  if (X.GetMin() > _Unit.X.GetMax()) return false;
  if (X.GetMax() < _Unit.X.GetMin()) return false;
  if (Y.GetMin() > _Unit.Y.GetMax()) return false;
  if (Y.GetMax() < _Unit.Y.GetMin()) return false;

  return true;
}

Simple2DGame::Simple2DGame(const Events_t & _Events, 
  const RendersPtr_t & _pRenders, const Rect_t & _Rect) :
  Common(_pRenders),
  m_Events(_Events),
  m_Xo(static_cast<float>(_Rect.Width + _Rect.Left) / 2.0f),
  m_Yo(static_cast<float>(_Rect.Height + _Rect.Top) / 2.0f),
  m_Width(static_cast<float>(_Rect.Width - _Rect.Left)),
  m_Height(static_cast<float>(_Rect.Height - _Rect.Top)),
  m_GameFieldSize(0.9f * math::Min(m_Width, m_Height)),
  m_UserUnit(0.0f, 0.0f, 0.15f, 0.15f, 0.0f)
{
  LoadTexture("bricks.jpg", uT("Simple2DGame.Texture"));

  m_Scene.push_back(BuildCamera());

  m_Scene.push_back(BuildRectangle(
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), static_cast<float>(_Rect.Left) / m_GameFieldSize },
      { uT("y"), static_cast<float>(_Rect.Top) / m_GameFieldSize },
    }), 
    m_Width / m_GameFieldSize,
    m_Height / m_GameFieldSize,
    uT("Simple2DGame.Texture")));

  m_Scene.push_back(BuildRectangle(
    Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), GetScreenX(-0.5f) },
        { uT("y"), GetScreenY(-0.5f) },
      }), 
      1.0f,
      1.0f,
      0xFF000000));

  m_Enemies.push_back(GameUnit{ 0.25f, 0.25f, 0.15f, 0.15f, 30.0f });
  m_Enemies.push_back(GameUnit{ -0.25f, 0.25f, 0.3f, 0.15f, -115.0f });
  m_Enemies.push_back(GameUnit{ 0.25f, -0.25f, 0.15f, 0.3f, -60.0f });

  auto UpdatePosition = [&](const GameUnit & _Unit)
  {
    _Unit.m_pPosition->SetValue(uT("x"), GetScreenX(_Unit.X.GetMin()));
    _Unit.m_pPosition->SetValue(uT("y"), GetScreenY(_Unit.Y.GetMin()));
  };

  for (const auto & Enemy : m_Enemies)
  {
    m_Scene.push_back(BuildRectangle(
      Enemy.m_pPosition,
      Enemy.X.Size,
      Enemy.Y.Size,
      uT("Simple2DGame.Texture")));

    UpdatePosition(Enemy);
  }

  m_Scene.push_back(BuildRectangle(
    m_UserUnit.m_pPosition,
    m_UserUnit.X.Size,
    m_UserUnit.Y.Size,
    0xFF00FF00));

  UpdatePosition(m_UserUnit);

  auto EndGame = [&](void)
  {
    m_Scene.push_back(BuildRectangle(
      m_UserUnit.m_pPosition,
      m_UserUnit.X.Size,
      m_UserUnit.Y.Size,
      0xFF0000FF));

    using namespace ::std::chrono;

    const float GameTime = duration<float>{ 
      system_clock::now() - m_BeginGameTime }.count();

    m_Events[::events::Simple2DGame.Finish](GameTime);

    m_Updater = [](void) {};
  };

  auto ProcessGame = [&, UpdatePosition, EndGame](void)
  {
    using namespace ::std::chrono;

    const auto CurrentTime = system_clock::now();
    const float FrameTime = duration<float>{ CurrentTime - m_LastTime }.count();
    m_LastTime = CurrentTime;

    m_Speed += 0.02f * FrameTime;

    if (m_IsTouch)
    {
      m_UserUnit.X.Position = m_MouseX;
      m_UserUnit.Y.Position = m_MouseY;
    }

    UpdatePosition(m_UserUnit);

    if (m_UserUnit.X.GetMin() <= -0.5f ||
      m_UserUnit.X.GetMax() >= 0.5f || 
      m_UserUnit.Y.GetMin() <= -0.5f || 
      m_UserUnit.Y.GetMax() >= 0.5f)
    {
      m_Updater = EndGame;
      return;
    }

    auto UpdateCoord = [&](GameUnit::Info & _Coord)
    {
      _Coord.Position += _Coord.SpeedFactor * m_Speed * FrameTime;

      if ((_Coord.GetMin() <= -0.5f && _Coord.SpeedFactor < 0.0f) ||
        (_Coord.GetMax() >= 0.5f && _Coord.SpeedFactor > 0.0f))
      {
        _Coord.SpeedFactor = -_Coord.SpeedFactor;
      }
    };

    for (auto & Enemy : m_Enemies)
    {
      UpdateCoord(Enemy.X);
      UpdateCoord(Enemy.Y);

      UpdatePosition(Enemy);

      if (m_UserUnit.IsIntersect(Enemy))
      {
        m_Updater = EndGame;
        return;
      }
    }
  };

  auto WaitStartGame = [&, ProcessGame](void)
  {
    if (!m_IsTouch) return;
    if (m_UserUnit.X.GetMin() > m_MouseX) return;
    if (m_UserUnit.X.GetMax() < m_MouseX) return;
    if (m_UserUnit.Y.GetMin() > m_MouseY) return;
    if (m_UserUnit.Y.GetMax() < m_MouseY) return;

    m_BeginGameTime = ::std::chrono::system_clock::now();
    m_LastTime = ::std::chrono::system_clock::now();
    m_Updater = ProcessGame;
  };

  m_Updater = WaitStartGame;

  namespace events = ::covellite::events;

  m_Events[events::Cursor.Touch].Connect(
    [&](void)
  {
    m_IsTouch = true;
  });

  m_Events[events::Cursor.Release].Connect(
    [&](void)
  {
    m_IsTouch = false;
  });

  m_Events[events::Cursor.Motion].Connect(
    [&](const events::Cursor_t::Position & _Position)
  {
    m_MouseX = GetCenterGameFieldUnitX(_Position.X);
    m_MouseY = GetCenterGameFieldUnitY(_Position.Y);
  });

  m_BeginRenderFrameTime = ::std::chrono::system_clock::now();
}

void Simple2DGame::Render(void) /*override*/
{
  m_Updater();

  Common::Render();

  // Синхронизация в 60 fps 

  using namespace ::std::chrono;

  const auto Time = 
    duration_cast<microseconds>(system_clock::now() - m_BeginRenderFrameTime);

  if (microseconds{ 16667 } > Time)
  {
    ::std::this_thread::sleep_for(microseconds{ 16667 } - Time);
  }

  m_BeginRenderFrameTime = system_clock::now();
}

float Simple2DGame::GetScreenX(float _CenterGameFieldUnitX) const
{
  return m_Xo / m_GameFieldSize + _CenterGameFieldUnitX;
}

float Simple2DGame::GetScreenY(float _CenterGameFieldUnitY) const
{
  return m_Yo / m_GameFieldSize + _CenterGameFieldUnitY;
}

float Simple2DGame::GetCenterGameFieldUnitX(float _ScreenX) const
{
  return (_ScreenX - m_Xo) / m_GameFieldSize;
}

float Simple2DGame::GetCenterGameFieldUnitY(float _ScreenY) const
{
  return (_ScreenY - m_Yo) / m_GameFieldSize;
}

auto Simple2DGame::BuildCamera(void) -> Id
{
  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Camera.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Camera") },
      })
    });

  return Id;
}

auto Simple2DGame::BuildRectangle(
  const Component_t::ComponentPtr_t & _pPosition, 
  float _Width, float _Height, const String_t & _TextureId) -> Id
{
  const float TextureOneCellInPixel = GetScreenX(0.1f) - GetScreenX(-0.1f);

  const VertexData_t VertexData =
  {
    {      0,       0,  0xFFFFFFFF,    0.0f,    0.0f, }, // 0
    {      0, _Height,  0xFFFFFFFF,    0.0f, _Height / TextureOneCellInPixel, }, // 1
    { _Width,       0,  0xFFFFFFFF,  _Width / TextureOneCellInPixel,    0.0f, }, // 2
    { _Width, _Height,  0xFFFFFFFF,  _Width / TextureOneCellInPixel, _Height / TextureOneCellInPixel, }, // 3
  };

  const Object_t PixelData =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("ps_4_0") },
      { uT("entry"), uT("psTextured") },
      { uT("data"), ::Pixel.data() },
      { uT("count"), ::Pixel.size() }
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Simple2DGame.Shader.Pixel.Textured") },
      { uT("type"), uT("Shader") },
      { uT("kind"), uT("Pixel") },
    }),
    Component_t::Make(
    {
      { uT("id"), _TextureId },
    })
  };

  return BuildRectangle(_pPosition, PixelData, VertexData);
}

auto Simple2DGame::BuildRectangle(
  const Component_t::ComponentPtr_t & _pPosition,
  float _Width, float _Height, uint32_t _Color) -> Id
{
  const VertexData_t VertexData =
  {
    {      0,       0,  _Color,  0.0f, 0.0f, }, // 0
    {      0, _Height,  _Color,  0.0f, 0.0f, }, // 1
    { _Width,       0,  _Color,  0.0f, 0.0f, }, // 2
    { _Width, _Height,  _Color,  0.0f, 0.0f, }, // 3
  };

  const Object_t PixelData =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("ps_4_0") },
      { uT("entry"), uT("psColored") },
      { uT("data"), ::Pixel.data() },
      { uT("count"), ::Pixel.size() }
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Simple2DGame.Shader.Pixel.Colored") },
      { uT("type"), uT("Shader") },
      { uT("kind"), uT("Pixel") },
    }),
  };

  return BuildRectangle(_pPosition, PixelData, VertexData);
}

auto Simple2DGame::BuildRectangle(
  const Component_t::ComponentPtr_t & _pPosition,
  const Object_t & _PixelData,
  const VertexData_t & _VertexData) -> Id
{
  const Id Id;

  const ::std::vector<int> IndexData =
  {
     0,  1,  2,   2,  1,  3,
  };

  using namespace ::alicorn::extension::std;

  m_Objects[Id] = m_pRenders->Obtain(
    _PixelData +
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.State.Sampler") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Sampler") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("VS") },
        { uT("data"), ::Vertex.data() },
        { uT("count"), ::Vertex.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Vertex.Rectangle") },
        { uT("type"), uT("Shader") },
        { uT("kind"), Vertex_t::GetName() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Vertex.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("kind"), Vertex_t::GetName() },
        { uT("data"), _VertexData.data() },
        { uT("count"), _VertexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Index.Rectangle") },
        { uT("type"), uT("Buffer") },
        { uT("kind"), uT("Index") },
        { uT("data"), IndexData.data() },
        { uT("count"), IndexData.size() },
      }),
      _pPosition,
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Scale") },
        { uT("x"), m_GameFieldSize },
        { uT("y"), m_GameFieldSize },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Present.Rectangle.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    });

  return Id;
}
