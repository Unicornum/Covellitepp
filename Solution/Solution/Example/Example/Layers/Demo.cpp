
#include "stdafx.h"
#include "Demo.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

// 17 Март 2019 11:45 (unicornum.verum@gmail.com)
TODO("Недопустимая ссылка на заголовочный файл!");
#include "../Basements/Demo.hpp"

namespace layers
{

const auto DemoLayerDescription =
uT("Демонстрационный режим предназначен для демонстрации различных ") +
uT("возможностей, предоставляемых базовым функционалом фреймворка ") +
uT("(а также тестирования его работы в течении длительного времени).<br/> ") +
uT("<br/>") +
uT("Главное меню:<br/>") +
uT("- <b>Lanscape</b> - бесконечный ландшафт, генерируемый случайным образом ") +
uT("(режимы дня/ночи и автоматического/ручного движения указывается ") +
uT("в настройках; ручное управление осуществляется при помощи кнопок ") +
uT("клавиатуры QWEASDZXC или жестов мышью с нажатой левой кнопкой).<br/>") +
uT("Ночной режим демонстрирует рендеринг с использованием большого ") +
uT("количества (100+) точечных источников с сцене.<br/>") +
uT("- <b>Animation</b> - рендеринг анимированной модели, поверхность ") +
uT("которой отображается при помощи нескольких текстур.<br/>") +
uT("- <b>Shadows</b> - демонстрация использования рендеринга во внеэкранную ") +
uT("поверхность (динамичесткие тени и глубина резкости), а также использование ") +
uT("массивов текстур (смешивание текстур поверхности).<br / >") +
uT("- <b>Help</b> - отображение окна с этим описанием.<br/>") +
uT("- <b>Exit</b> - возврат на главное окно программы-примера ") +
uT("использования фреймворка.<br/>") +
uT("<br/>") +
uT("Выход в главное меню производится кнопкой в левом верхнем углу ") +
uT("внутриигрового меню.<br/>");

Demo::Demo(IWindowGui_t & _Window) :
  Layer(_Window, "Data/demo.rml")
{
  GetElement("id_back_to_main_menu").SetMeaning(uT("\uF060"));

  EmployFontSize();
  ActivateProcessMainMenuEvents();
  ActivateProcessGameMenuEvents();
  ActivateProcessUserActionEvents();
}

void Demo::EmployFontSize(void)
{
  const auto DoEmployFontSize = [=](void)
  {
    Layer::EmployFontSize(2.5);
  };

  m_Events[::covellite::events::Window.Resize]
    .Connect(DoEmployFontSize);

  DoEmployFontSize();
}

void Demo::ActivateProcessMainMenuEvents(void)
{
  const auto RunLoadScene = [=](const ::events::Demo_t::Id _Mode)
  {
    GetElement("id_main_menu").SetClassStyle(uT("hidden"));
    GetElement("id_loading").SetClassStyle(uT(""));

    m_Updater = GetProcessLoading();

    m_PercentLoading = 0;
    m_Updater();

    m_Events[_Mode](&m_PercentLoading);
  };

  using namespace ::covellite::events;

  m_Events[Click.DocumentId(GetId()).ElementId("id_landscape")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.Landscape);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_animation")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.Animation);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_shadows")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.ShadowsWindow);
    GetElement("id_shadow_selector").SetClassStyle(uT(""));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_shadow_window")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.ShadowsWindow);
    GetElement("id_shadow_selector").SetClassStyle(uT(""));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_shadow_scale")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.ShadowsScale);
    GetElement("id_shadow_selector").SetClassStyle(uT(""));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_shadow_fix_size")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.ShadowsFixSize);
    GetElement("id_shadow_selector").SetClassStyle(uT(""));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_help")]
    .Connect([=](void)
  {
    m_Events[Button.Help]();
    m_Events[Help.Title](uT("Демонстрационный режим"));
    m_Events[Help.Text](DemoLayerDescription);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_back")]
    .Connect([=](void)
  {
    m_Events[Button.Back](true);
  });

  m_Events[Drawing.Do].Connect([=](void)
  {
    m_Updater();
  });
}

void Demo::ActivateProcessGameMenuEvents(void)
{
  using namespace ::covellite::events;

  m_Events[Click.DocumentId(GetId()).ElementId("id_back_to_main_menu")]
    .Connect([=](void)
  {
    GetElement("id_main_menu").SetClassStyle(uT(""));
    GetElement("id_loading").SetClassStyle(uT("hidden"));
    GetElement("id_game_menu").SetClassStyle(uT("hidden"));
    GetElement("id_shadow_selector").SetClassStyle(uT("hidden"));

    m_Events[::events::Demo.Exit]();
  });
}

void Demo::ActivateProcessUserActionEvents(void)
{
  class Coord
  {
  public:
    int32_t X, Y;
  };

  namespace events = ::covellite::events;

  const auto pIsTouch = ::std::make_shared<bool>(false);
  const auto pCursorCurrentPosition = ::std::make_shared<Coord>();
  const auto pCursorTouchPosition = ::std::make_shared<Coord>();
  const auto pAction =
    ::std::make_shared<::events::Demo_t::Id>(::events::Demo.None);

  m_Events[events::Cursor.Motion].Connect([=](const events::Cursor_t::Position & _Position)
  {
    *pCursorCurrentPosition = { _Position.X, _Position.Y };

    if (!*pIsTouch) return;

    const auto HalfScreenX = GetWidth() / 2.0f;
    const auto dX = pCursorCurrentPosition->X - pCursorTouchPosition->X;
    const auto dY = pCursorCurrentPosition->Y - pCursorTouchPosition->Y;

    if (abs(dX) < 25 && abs(dY) < 25) return;

    if (abs(dX) > 3 * abs(dY)) // Горизонтальное движение мышью
    {
      *pAction = (dX > 0) ?
        ::events::Demo.TurnRight : ::events::Demo.TurnLeft;
    }
    else if (abs(dY) > 3 * abs(dX)) // Вертикальное движение мышью
    {
      if (pCursorTouchPosition->X > HalfScreenX)
      {
        *pAction = (dY > 0) ?
          ::events::Demo.MoveBackward : ::events::Demo.MoveForward;
      }
      else
      {
        *pAction = (dY > 0) ?
          ::events::Demo.TurnDown : ::events::Demo.TurnUp;
      }
    }
    else // Диагональное движение мышью
    {
      if (dY < 0)
      {
        *pAction = (dX > 0) ?
          ::events::Demo.MoveRightForward : ::events::Demo.MoveLeftForward;
      }
      else
      {
        *pAction = (dX > 0) ?
          ::events::Demo.MoveRightBackward : ::events::Demo.MoveLeftBackward;
      }
    }
  });

  m_Events[events::Cursor.Touch].Connect([=](void)
  {
    *pIsTouch = true;
    *pCursorTouchPosition = *pCursorCurrentPosition;
  });

  m_Events[events::Cursor.Release].Connect([=](void)
  {
    *pIsTouch = false;
    *pAction = ::events::Demo.None;
  });

  m_Events[events::Key.Down].Connect(
    [=](const events::Key_t::Code & _KeyCode)
  {
    static const ::std::map<events::Key_t::Code, ::events::Demo_t::Id> MapCodes =
    {
      { 0x57, ::events::Demo.MoveForward },
      { 0x53, ::events::Demo.MoveForward },
      { 0x58, ::events::Demo.MoveBackward },
      { 0x51, ::events::Demo.MoveLeftForward },
      { 0x5A, ::events::Demo.MoveLeftBackward },
      { 0x45, ::events::Demo.MoveRightForward },
      { 0x43, ::events::Demo.MoveRightBackward },
      { 0x41, ::events::Demo.TurnLeft },
      { 0x44, ::events::Demo.TurnRight },
    };

    const auto itAction = MapCodes.find(_KeyCode);
    if (itAction != MapCodes.end())
    {
      *pAction = itAction->second;
    }
  });

  m_Events[events::Key.Up].Connect([=](void)
  {
    *pAction = ::events::Demo.None;
  });

  m_Events[events::Application.Update].Connect([=](void)
  {
    if (*pAction != ::events::Demo.None) m_Events[*pAction]();
  });
}

auto Demo::GetProcessLoading(void) -> Updater_t
{
  return [=](void)
  {
    const auto Value = (m_PercentLoading * 0.85f + 15.0f) / 100.0f;

    GetElement("id_progress").SetMeaning(Value);
    //GetElement("id_circular").SetMeaning(Value);

    if (m_PercentLoading == 101) // Чтобы отрисовывалось значение 100
    {
      GetElement("id_loading").SetClassStyle(uT("hidden"));
      GetElement("id_game_menu").SetClassStyle(uT(""));

      m_Updater = GetProcessCalculateFps();
    }
  };
}

auto Demo::GetProcessCalculateFps(void) -> Updater_t
{
  using namespace ::std::chrono;

  const auto pFps =
    ::std::make_shared<int>(0);
  const auto pBegin =
    ::std::make_shared<time_point<system_clock>>(system_clock::now());

  return [=](void)
  {
    (*pFps)++;

    if (duration_cast<seconds>(system_clock::now() - *pBegin) >= seconds{ 1 })
    {
      GetElement("id_fps").SetMeaning(uT("FPS: {FPS}")
        .Replace(uT("{FPS}"), *pFps));

      *pFps = 0;
      *pBegin = system_clock::now();
    }
  };
}

} // namespace layers
