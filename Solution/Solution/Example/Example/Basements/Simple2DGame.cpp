
#include "stdafx.h"
#include "Simple2DGame.hpp"
#include <chrono>
#include <thread>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Component.inl>

using namespace basement;
namespace math = ::alicorn::extension::cpp::math;

void Simple2DGame::GameUnit::Info::Update(float _Moving)
{
  Position += SpeedFactor * _Moving;

  if ((GetMin() <= -0.5f && SpeedFactor < 0.0f) ||
    (GetMax() >= 0.5f && SpeedFactor > 0.0f))
  {
    SpeedFactor = -SpeedFactor;
  }
}

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
  m_pPosition->SetValue(uT("x"), X.Position);
  m_pPosition->SetValue(uT("y"), Y.Position);
}

void Simple2DGame::GameUnit::Update(float _X, float _Y)
{
  X.Position = _X;
  Y.Position = _Y;

  m_pPosition->SetValue(uT("x"), X.Position);
  m_pPosition->SetValue(uT("y"), Y.Position);
}

void Simple2DGame::GameUnit::Update(float _Moving)
{
  X.Update(_Moving);
  Y.Update(_Moving);

  m_pPosition->SetValue(uT("x"), X.Position);
  m_pPosition->SetValue(uT("y"), Y.Position);
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
  const RendersPtr_t & _pRenders, const Rect & _Rect) :
  Common(_pRenders),
  m_Events(_Events),
  m_Width(_Rect.Right - _Rect.Left),
  m_Height(_Rect.Bottom - _Rect.Top),
  m_GameFieldSize(0.9f * math::Min(m_Width, m_Height)),
  m_UserUnit(0.0f, 0.0f, 0.15f, 0.15f, 0.0f)
{
  const auto Xo = (_Rect.Right + _Rect.Left) / 2.0f;
  const auto Yo = (_Rect.Bottom + _Rect.Top) / 2.0f;

  AddCommonComponents();
  AddCamera(Xo, Yo);
  AddBackground();
  AddClock();
  AddActors();

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
    [&, Xo, Yo](const events::Cursor_t::Position & _Position)
  {
    m_MouseX = (static_cast<float>(_Position.X) - Xo) / m_GameFieldSize;
    m_MouseY = (static_cast<float>(_Position.Y) - Yo) / m_GameFieldSize;
  });

  m_BeginRenderFrameTime = ::std::chrono::system_clock::now();
}

Simple2DGame::~Simple2DGame(void)
{
  LOGGER(Info) << "Basements Simple2DGame destoyed.";

  // Рендеры не удаляются, т.к. объект класса Simple2DGame содержит уникальный
  // объект m_pRenders, который удаляется вместе со всеми рендерами.
  // m_pRenders->Remove(...);
}

void Simple2DGame::Render(void) /*override*/
{
  const auto Updaters = m_Updaters;
  for (auto & Updater : Updaters) Updater();

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

void Simple2DGame::AddCommonComponents(void)
{
  // Создание общих рендеров, которые будут использоваться всеми объектами
  // (объекты будут запрашивать эти рендеры по их id).

  LoadTexture("simple2dgame.bricks.jpg", uT("Simple2DGame.Texture"));

  /// [Common objects]
  const ::std::vector<int> IndexData = { 0,  1,  2,   2,  1,  3, };
    
  const auto Renders = 
    m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Buffer") },
        { uT("data"), IndexData.data() },
        { uT("count"), IndexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Index.Rectangle") },
        { uT("type"), uT("Buffer") },
        { uT("kind"), uT("Index") },
      }),
      /// [Common objects]
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Textured") },
        { uT("type"), uT("Shader") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psColored") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Colored") },
        { uT("type"), uT("Shader") },
      }),
    });

  ::boost::ignore_unused(Renders);
}

void Simple2DGame::AddCamera(float _Xo, float _Yo)
{
  const Id Id;

  /// [Create camera]
  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), -_Xo },
        { uT("y"), -_Yo },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Camera.") + Id.GetStringId() },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Orthographic") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.State.Blend") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Blend") },
      }),
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
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Vertex.Rectangle") },
        { uT("type"), uT("Shader") },
      }),
    });
    
  m_Scene.push_back(Id);
  /// [Create camera]
}

void Simple2DGame::AddBackground(void)
{
  m_Scene.push_back(BuildRectangle({},
    m_Width / m_GameFieldSize, m_Height / m_GameFieldSize,
    uT("Simple2DGame.Texture")));

  m_Scene.push_back(BuildRectangle({},
    1.0f, 1.0f,
    0xFF000000));
}

void Simple2DGame::AddClock(void)
{
  LoadTexture("simple2dgame.clock.png", uT("Simple2DGame.Texture.Clock"));

  // Циферблат

  m_Scene.push_back(BuildRectangle(
    Rect{ -0.5f, -0.5f, 0.5f, 0.5f },
    0x7FFFFFFF,
    Rect{ 0.00f, 0.00f, 0.72f, 0.72f },
    uT("Simple2DGame.Texture.Clock"),
    {}));

  // Секундная стрелка

  auto pSecondArrowRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  m_Scene.push_back(BuildRectangle(
    Rect{ -0.05f, -0.22f, 0.05f, 0.22f },
    0x7FFFFFFF,
    Rect{ 0.85f, 0.14f, 0.99f, 0.70f },
    uT("Simple2DGame.Texture.Clock"),
    {
      Component_t::Make(
        {
          { uT("type"), uT("Data") },
          { uT("kind"), uT("Position") },
          { uT("y"), -0.161f },
        }),
      pSecondArrowRotation
    }));

  // Миллисекундная стрелка

  auto pMillisecondArrowRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  m_Scene.push_back(BuildRectangle(
    Rect{ -0.04f, -0.27f, 0.04f, 0.27f },
    0x7F0000FF,
    Rect{ 0.738f, 0.00f, 0.848f, 0.70f },
    uT("Simple2DGame.Texture.Clock"),
    {
      Component_t::Make(
        {
          { uT("type"), uT("Data") },
          { uT("kind"), uT("Position") },
          { uT("y"), -0.22f },
        }),
      pMillisecondArrowRotation
    }));

  // Заглушка над центром стрелок

  m_Scene.push_back(BuildRectangle(
    Rect{ -0.03f, -0.03f, 0.03f, 0.03f },
    0x7F000000,
    Rect{ 0.755f, 0.768f, 0.957f, 0.965f },
    uT("Simple2DGame.Texture.Clock"),
    { }));

  m_ClockUpdater = [=](void)
  {
    using namespace ::std::chrono;

    const float GameTime = duration<float>{
      system_clock::now() - m_BeginGameTime }.count();

    float Second = 0.0f;
    const float MilliSecond = modff(GameTime, &Second);

    pSecondArrowRotation->SetValue(uT("z"), 
      3.1415926f * 2.0f * Second / 60.0f);
    pMillisecondArrowRotation->SetValue(uT("z"), 
      3.1415926f * 2.0f * MilliSecond);
  };
}

void Simple2DGame::AddActors(void)
{
  m_Enemies.push_back(GameUnit{ 0.25f, 0.25f, 0.15f, 0.15f, 30.0f });
  m_Enemies.push_back(GameUnit{ -0.25f, 0.25f, 0.3f, 0.15f, -115.0f });
  m_Enemies.push_back(GameUnit{ 0.25f, -0.25f, 0.15f, 0.3f, -60.0f });

  for (const auto & Enemy : m_Enemies)
  {
    m_Scene.push_back(BuildRectangle(
      { Enemy.m_pPosition },
      Enemy.X.Size, Enemy.Y.Size,
      uT("Simple2DGame.Texture")));
  }

  m_Scene.push_back(BuildRectangle(
    { m_UserUnit.m_pPosition },
    m_UserUnit.X.Size, m_UserUnit.Y.Size,
    0xFF00FF00));

  auto EndGame = [&](void)
  {
    m_Scene.pop_back();
    m_Scene.push_back(BuildRectangle(
      { m_UserUnit.m_pPosition },
      m_UserUnit.X.Size, m_UserUnit.Y.Size,
      0xFF0000FF));

    using namespace ::std::chrono;

    const float GameTime = duration<float>{
      system_clock::now() - m_BeginGameTime }.count();

    m_Events[::events::Simple2DGame.Finish](GameTime);

    m_Updaters.clear();
  };

  auto ProcessGame = [&, EndGame](void)
  {
    using namespace ::std::chrono;

    const auto CurrentTime = system_clock::now();
    const float FrameTime = duration<float>{ CurrentTime - m_LastTime }.count();
    m_LastTime = CurrentTime;

    if (m_IsTouch)
    {
      m_UserUnit.Update(m_MouseX, m_MouseY);
    }

    if (m_UserUnit.X.GetMin() <= -0.5f ||
      m_UserUnit.X.GetMax() >= 0.5f ||
      m_UserUnit.Y.GetMin() <= -0.5f ||
      m_UserUnit.Y.GetMax() >= 0.5f)
    {
      m_Updaters = { EndGame };
      return;
    }

    m_EnemiesSpeed += 0.02f * FrameTime;

    for (auto & Enemy : m_Enemies)
    {
      Enemy.Update(m_EnemiesSpeed * FrameTime);

      if (m_UserUnit.IsIntersect(Enemy))
      {
        m_Updaters = { EndGame };
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
    m_Updaters = 
    {
      ProcessGame,
      m_ClockUpdater,
    };
  };

  m_Updaters = { WaitStartGame };
}

auto Simple2DGame::BuildRectangle(const Object_t & _Transform,
  float _Width, float _Height, const String_t & _TextureId) -> Id
{
  const float TextureOneCellInPixel = 0.2f;
  const float U = _Width / TextureOneCellInPixel;
  const float V = _Height / TextureOneCellInPixel;

  return BuildRectangle(
    Rect{ -_Width / 2.0f, -_Height / 2.0f, _Width / 2.0f, _Height / 2.0f },
    0xFFFFFFFF,
    Rect{ 0.0f, 0.0f, U, V },
    _TextureId,
    _Transform);
}

auto Simple2DGame::BuildRectangle(const Object_t & _Transform,
  float _Width, float _Height, uint32_t _Color) -> Id
{
  return BuildRectangle(
    Rect{ -_Width / 2.0f, -_Height / 2.0f, _Width / 2.0f, _Height / 2.0f },
    _Color,
    Rect{ 0.0f, 0.0f, 0.0f, 0.0f },
    {
      /// [Colored object]
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Colored") },
      }),
      /// [Colored object]
    },
    _Transform);
}

auto Simple2DGame::BuildRectangle(
  const Rect & _Polygon,
  uint32_t _Color,
  const Rect & _TexCoord,
  const String_t & _TextureId,
  const Object_t & _Transform) -> Id
{
  return BuildRectangle(_Polygon, _Color, _TexCoord,
    {
      /// [Textured object]
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Textured") },
      }),
      Component_t::Make(
      {
        { uT("id"), _TextureId },
      }),
      /// [Textured object]
    }, _Transform);
}

auto Simple2DGame::BuildRectangle(
  const Rect & _Polygon, 
  uint32_t _Color,
  const Rect & _TexCoord, 
  const Object_t & _PixelData, 
  const Object_t & _Transform) -> Id
{
  const Id Id;

  /// [Vertex buffer]
  const VertexData_t VertexData =
  {
    {  _Polygon.Left,  _Polygon.Top,     _Color,  _TexCoord.Left,  _TexCoord.Top,    }, // 0
    {  _Polygon.Left,  _Polygon.Bottom,  _Color,  _TexCoord.Left,  _TexCoord.Bottom, }, // 1
    {  _Polygon.Right, _Polygon.Top,     _Color,  _TexCoord.Right, _TexCoord.Top,    }, // 2
    {  _Polygon.Right, _Polygon.Bottom,  _Color,  _TexCoord.Right, _TexCoord.Bottom, }, // 3
  };
  /// [Vertex buffer]

  using namespace ::alicorn::extension::std;

  m_Objects[Id] = m_pRenders->Obtain(
    _PixelData +
    /// [Common object renders]
    Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Buffer") },
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Vertex.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Index.Rectangle") },
      }),
    } +
    _Transform +
    Object_t
    {
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
    /// [Common object renders]

  return Id;
}
