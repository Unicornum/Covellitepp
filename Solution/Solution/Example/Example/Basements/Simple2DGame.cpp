
#include "stdafx.h"
#include "Simple2DGame.hpp"
#include <chrono>
#include <thread>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/App/Settings.hpp>
#include <Covellite/App/Vfs.hpp>

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
  (*m_pPosition)[uT("x")] = X.Position;
  (*m_pPosition)[uT("y")] = Y.Position;
}

void Simple2DGame::GameUnit::Update(float _X, float _Y)
{
  X.Position = _X;
  Y.Position = _Y;

  (*m_pPosition)[uT("x")] = X.Position;
  (*m_pPosition)[uT("y")] = Y.Position;
}

void Simple2DGame::GameUnit::Update(float _Moving)
{
  X.Update(_Moving);
  Y.Update(_Moving);

  (*m_pPosition)[uT("x")] = X.Position;
  (*m_pPosition)[uT("y")] = Y.Position;
}

bool Simple2DGame::GameUnit::IsIntersect(const GameUnit & _Unit) const
{
  if (X.GetMin() > _Unit.X.GetMax()) return false;
  if (X.GetMax() < _Unit.X.GetMin()) return false;
  if (Y.GetMin() > _Unit.Y.GetMax()) return false;
  if (Y.GetMax() < _Unit.Y.GetMin()) return false;

  return true;
}

Simple2DGame::Simple2DGame(WindowExpanse_t & _Window, const Rect & _Rect) :
  Common(_Window),
  m_Events(_Window),
  m_Width(_Rect.Right - _Rect.Left),
  m_Height(_Rect.Bottom - _Rect.Top),
  m_GameFieldSize(0.9f * math::Min(m_Width, m_Height)),
  m_UserUnit(0.0f, 0.0f, 0.15f, 0.15f, 0.0f)
{
  const auto Xo = (_Rect.Right + _Rect.Left) / 2.0f;
  const auto Yo = (_Rect.Bottom + _Rect.Top) / 2.0f;

  AddCommonComponents();
  m_CameraId = AddCamera(Xo, Yo);
  m_BackGroundId = AddBackground();
  m_ClockId = AddClock();
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
}

Simple2DGame::~Simple2DGame(void)
{
  LOGGER(Info) << "Basements Simple2DGame destroyed.";
}

void Simple2DGame::AddCommonComponents(void)
{
  // Создание общих рендеров, которые будут использоваться всеми объектами
  // (объекты будут использовать рендеры, запрашивая их по id).

  const auto pMainUpdater = Component_t::Make(
    {
      { uT("id"), uT("Simple2DGame.Updater.Main") },
      { uT("type"), uT("Updater") },
    });

  const Updater_t EndGame = [=](const float _Time)
  {
    AddToRenderQueue(m_CameraId);
    AddToRenderQueue(m_BackGroundId);
    AddToRenderQueue(m_ClockId);
    AddToRenderQueue(m_EnemiesId);
    AddToRenderQueue(m_RedSquareId);

    m_BeginGameTime = _Time - m_GameTime;
  };

  const Updater_t FinishGame = [=](const float _Time)
  {
    m_GameTime = _Time - m_BeginGameTime;

    m_Events[::events::Simple2DGame.Finish](m_GameTime);

    (*pMainUpdater)[uT("function")] = EndGame;
  };

  const Updater_t ProcessGame = [=](const float _Time)
  {
    AddToRenderQueue(m_CameraId);
    AddToRenderQueue(m_BackGroundId);
    AddToRenderQueue(m_ClockId);
    AddToRenderQueue(m_EnemiesId);
    AddToRenderQueue(m_GreenSquareId);

    const float FrameTime = _Time - m_LastTime;
    m_LastTime = _Time;

    if (m_IsTouch)
    {
      m_UserUnit.Update(m_MouseX, m_MouseY);
    }

    if (m_UserUnit.X.GetMin() <= -0.5f ||
      m_UserUnit.X.GetMax() >= 0.5f ||
      m_UserUnit.Y.GetMin() <= -0.5f ||
      m_UserUnit.Y.GetMax() >= 0.5f)
    {
      (*pMainUpdater)[uT("function")] = FinishGame;
      return;
    }

    m_EnemiesSpeed += 0.02f * FrameTime;

    for (auto & Enemy : m_Enemies)
    {
      Enemy.Update(m_EnemiesSpeed * FrameTime);

      if (m_UserUnit.IsIntersect(Enemy))
      {
        (*pMainUpdater)[uT("function")] = FinishGame;
        return;
      }
    }
  };

  const Updater_t WaitStartGame = [=](const float _Time)
  {
    m_BeginGameTime = _Time;

    // Этот updater будет вызываться в начале рендеринга каждого кадра, его
    // задача - добавить в список рендеринга объекты, которые должны быть
    // отрендерены.

    AddToRenderQueue(m_CameraId);
    AddToRenderQueue(m_BackGroundId);
    AddToRenderQueue(m_ClockId);
    AddToRenderQueue(m_EnemiesId);
    AddToRenderQueue(m_GreenSquareId);

    if (!m_IsTouch) return;
    if (m_UserUnit.X.GetMin() > m_MouseX) return;
    if (m_UserUnit.X.GetMax() < m_MouseX) return;
    if (m_UserUnit.Y.GetMin() > m_MouseY) return;
    if (m_UserUnit.Y.GetMax() < m_MouseY) return;

    (*pMainUpdater)[uT("function")] = ProcessGame;
    m_LastTime = _Time;
  };

  (*pMainUpdater)[uT("function")] = WaitStartGame;

  const Updater_t Synchronizer60fps = [=](const float _Now)
  {
    using namespace ::std::chrono;

    static auto BeginRenderFrameTime = _Now;

    // Синхронизация в 60 fps

    const auto Time = 0.016666667f - (_Now - BeginRenderFrameTime);

    if (Time > 0) ::std::this_thread::sleep_for(duration<float>{ Time });

    BeginRenderFrameTime = _Now;
  };

  const auto pSynchronizer60fps = Component_t::Make(
    {
      { uT("id"), uT("Simple2DGame.Updater.Synchronizer") },
      { uT("type"), uT("Updater") },
      { uT("function"), Synchronizer60fps },
    });

  using namespace ::alicorn::extension::std;

  /// [Common objects]
  const auto & Vfs = ::covellite::app::Vfs_t::GetInstance();
        
  const auto CommonObjectId = CreateObject(
    LoadTexture("simple2dgame.bricks.jpg", uT("Simple2DGame.Texture")) +
    LoadTexture("simple2dgame.clock.png", uT("Simple2DGame.Texture.Clock")) +
    GameObject_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Textured") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psTextured") },
        { uT("content"), Vfs.GetData("Data\\Shaders\\Textured.fx") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Shader.Pixel.Colored") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psColored") },
        // { uT("content"), - }, // использовать шейдер по умолчанию
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Present.Rectangle") },
        { uT("type"), uT("Present") },
        { uT("content"), ::std::vector<int>{ 0,  1,  2,   2,  1,  3, } },
      }),
      pSynchronizer60fps,
      pMainUpdater
    });
  /// [Common objects]
}

auto Simple2DGame::AddCamera(float _Xo, float _Yo) -> ObjectId_t
{
  const Id Id;

  /// [Create camera]
  const auto pCameraPosition = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), -_Xo },
      { uT("y"), -_Yo },
    });
    
  return CreateObject(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Camera.") + Id.GetStringId() },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Orthographic") },
        { uT("service"), GameObject_t{ pCameraPosition } },
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
        { uT("id"), uT("Simple2DGame.Shader.Vertex.Rectangle") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsFlat") },
      }),
    });
  /// [Create camera]
}

auto Simple2DGame::AddBackground(void) -> ObjectId_t
{
  using namespace ::alicorn::extension::std;

  return CreateObject(
    BuildRectangle({},
      m_Width / m_GameFieldSize, m_Height / m_GameFieldSize,
      uT("Simple2DGame.Texture")) +
    BuildRectangle({},
      1.0f, 1.0f,
      0.0f, 0.0f, 0.0f, 1.0f)); // 0xFF000000
}

auto Simple2DGame::AddClock(void) -> ObjectId_t
{
  using namespace ::alicorn::extension::std;

  GameObject_t Result;

  // Циферблат

  Result += BuildRectangle(
    Rect{ -0.5f, -0.5f, 0.5f, 0.5f },
    Rect{ 0.00f, 0.00f, 0.72f, 0.72f },
    1.0f, 1.0f, 1.0f, 0.5f, //0x7FFFFFFF,
    uT("Simple2DGame.Texture.Clock"),
    {});

  // Секундная стрелка

  auto pSecondArrowRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  Result += BuildRectangle(
    Rect{ -0.05f, -0.22f, 0.05f, 0.22f },
    Rect{ 0.85f, 0.14f, 0.99f, 0.70f },
    1.0f, 1.0f, 1.0f, 0.5f, //0x7FFFFFFF,
    uT("Simple2DGame.Texture.Clock"),
    {
      Component_t::Make(
        {
          { uT("type"), uT("Data") },
          { uT("kind"), uT("Position") },
          { uT("y"), -0.161f },
        }),
      pSecondArrowRotation
    });

  // Миллисекундная стрелка

  auto pMillisecondArrowRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  Result += BuildRectangle(
    Rect{ -0.04f, -0.27f, 0.04f, 0.27f },
    Rect{ 0.738f, 0.00f, 0.848f, 0.70f },
    1.0f, 0.0f, 0.0f, 0.5f, //0x7F0000FF,
    uT("Simple2DGame.Texture.Clock"),
    {
      Component_t::Make(
        {
          { uT("type"), uT("Data") },
          { uT("kind"), uT("Position") },
          { uT("y"), -0.22f },
        }),
      pMillisecondArrowRotation
    });

  // Заглушка над центром стрелок

  Result += BuildRectangle(
    Rect{ -0.03f, -0.03f, 0.03f, 0.03f },
    Rect{ 0.755f, 0.768f, 0.957f, 0.965f },
    0.0f, 0.0f, 0.0f, 0.5f, //0x7F000000,
    uT("Simple2DGame.Texture.Clock"),
    { });

  const Updater_t ClockUpdater = [=](const float _Time)
  {
    float Second = 0.0f;
    const float MilliSecond = modff(_Time - m_BeginGameTime, &Second);

    namespace math = ::alicorn::extension::cpp::math;

    (*pSecondArrowRotation)[uT("z")] = 
      math::Constant<float>::Pi * 2.0f * Second / 60.0f;
    (*pMillisecondArrowRotation)[uT("z")] = 
      math::Constant<float>::Pi * 2.0f * MilliSecond;
  };

  Result.push_back(
    Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Updater.Clock") },
        { uT("type"), uT("Updater") },
        { uT("function"), ClockUpdater },
      })
  );

  return CreateObject(Result);
}

void Simple2DGame::AddActors(void)
{
  m_Enemies.push_back(GameUnit{ 0.25f, 0.25f, 0.15f, 0.15f, 30.0f });
  m_Enemies.push_back(GameUnit{ -0.25f, 0.25f, 0.3f, 0.15f, -115.0f });
  m_Enemies.push_back(GameUnit{ 0.25f, -0.25f, 0.15f, 0.3f, -60.0f });

  using namespace ::alicorn::extension::std;

  GameObject_t Result;

  for (const auto & Enemy : m_Enemies)
  {
    Result += BuildRectangle(
      { Enemy.m_pPosition },
      Enemy.X.Size, Enemy.Y.Size,
      uT("Simple2DGame.Texture"));
  }

  m_EnemiesId = CreateObject(Result);

  m_GreenSquareId = CreateObject(BuildRectangle(
    { m_UserUnit.m_pPosition },
    m_UserUnit.X.Size, m_UserUnit.Y.Size,
    0.0f, 1.0f, 0.0f, 1.0f));//0xFF00FF00));

  m_RedSquareId = CreateObject(BuildRectangle(
    { m_UserUnit.m_pPosition },
    m_UserUnit.X.Size, m_UserUnit.Y.Size,
    1.0f, 0.0f, 0.0f, 1.0f));//0xFF0000FF));
}

auto Simple2DGame::BuildRectangle(const GameObject_t & _Transform,
  float _Width, float _Height, const String_t & _TextureId) -> GameObject_t
{
  const float TextureOneCellInPixel = 0.2f;
  const float U = _Width / TextureOneCellInPixel;
  const float V = _Height / TextureOneCellInPixel;

  return BuildRectangle(
    Rect{ -_Width / 2.0f, -_Height / 2.0f, _Width / 2.0f, _Height / 2.0f },
    Rect{ 0.0f, 0.0f, U, V },
    1.0f, 1.0f, 1.0f, 1.0f, //0xFFFFFFFF,
    _TextureId,
    _Transform);
}

auto Simple2DGame::BuildRectangle(const GameObject_t & _Transform,
  float _Width, float _Height, float _R, float _G, float _B, float _A) -> GameObject_t
{
  return BuildRectangle(
    Rect{ -_Width / 2.0f, -_Height / 2.0f, _Width / 2.0f, _Height / 2.0f },
    Rect{ 0.0f, 0.0f, 0.0f, 0.0f },
    _R, _G, _B, _A,
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
  const Rect & _TexCoord,
  float _R, float _G, float _B, float _A,
  const String_t & _TextureId,
  const GameObject_t & _Transform) -> GameObject_t
{
  return BuildRectangle(_Polygon, _TexCoord, _R, _G, _B, _A,
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
  const Rect & _TexCoord,
  float _R, float _G, float _B, float _A,
  const GameObject_t & _PixelData, 
  const GameObject_t & _Transform) -> GameObject_t
{
  const Id Id;

  /// [Vertex buffer]
  const VertexData_t VertexData =
  {
    {  
      _Polygon.Left, _Polygon.Top, 0.0f, 1.0f,
      _TexCoord.Left, _TexCoord.Top,
      _R, _G, _B, _A,
    }, // 0
    {  
      _Polygon.Left, _Polygon.Bottom, 0.0f, 1.0f,
      _TexCoord.Left, _TexCoord.Bottom,
      _R, _G, _B, _A,
    }, // 1
    {  
      _Polygon.Right, _Polygon.Top, 0.0f, 1.0f,
      _TexCoord.Right, _TexCoord.Top,
      _R, _G, _B, _A,
    }, // 2
    {  
      _Polygon.Right, _Polygon.Bottom, 0.0f, 1.0f,
      _TexCoord.Right, _TexCoord.Bottom,
      _R, _G, _B, _A,
    }, // 3
  };
  /// [Vertex buffer]

  GameObject_t Trasform = _Transform;
  Trasform.push_back(Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Scale") },
      { uT("x"), m_GameFieldSize },
      { uT("y"), m_GameFieldSize },
    }));

  using namespace ::alicorn::extension::std;

  return 
    _PixelData +
    GameObject_t
    {
      /// [Common object renders]
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Buffer.Vertex.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("content"), VertexData },
        { uT("dimension"), 2 },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Transform.Rectangle.") + Id.GetStringId() },
        { uT("type"), uT("Transform") },
        { uT("service"), Trasform },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Simple2DGame.Present.Rectangle") },
      })
      /// [Common object renders]
    };
}
