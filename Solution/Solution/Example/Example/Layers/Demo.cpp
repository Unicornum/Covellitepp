
#include "stdafx.h"
#include "Demo.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

// 17 Март 2019 11:45 (unicornum.verum@gmail.com)
TODO("Недопустимая ссылка на заголовочный файл!");
#include "../Basements/Demo.hpp"

using namespace layers;

const auto LayerDescription =
  uT("Демонстрационный режим предназначен для демонстрации возможностей ") +
  uT("фреймворка отрисовки сцены из 3D объектов и тестирования его работы ") +
  uT("в течении длительного времени.<br/>") +
  uT("<br/>") +
  uT("Главное меню:<br/>") +
  uT("- <b>Ручное управление</b> - режим работы, в котором можно ") +
  uT("перемещаться по игровому миру делая шаг вреред и повороты направо и ") +
  uT("при помощи кнопок внутриигрового меню.<br/>") +
  uT("- <b>Демо</b> - режим работы, при котором камера перемещается по ") +
  uT("игровому миру в автоматическом режиме.<br/>") +
  uT("- <b>Описание</b> - отображение этого окна.<br/>") +
  uT("- <b>Выход</b> - возврат на главное окно программы-примера ") +
  uT("использования фреймворка.<br/>") +
  uT("<br/>") +
  uT("Выход в главное меню производится кнопкой в левом верхнем углу ") +
  uT("внутриигрового меню.<br/>");

Demo::Demo(IWindowGui_t & _Window) :
  Layer(_Window, "data/demo.rml")
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

  m_Events[Click.DocumentId(GetId()).ElementId("id_auto")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.Auto);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_manual")]
    .Connect([=](void)
  {
    RunLoadScene(::events::Demo.Manual);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_help")]
    .Connect([=](void) 
  { 
    m_Events[Button.Help]();
    m_Events[Help.Title](uT("Демонстрационный режим"));
    m_Events[Help.Text](LayerDescription);
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

  using namespace ::covellite::events;

  const auto pIsTouch = ::std::make_shared<bool>(false);
  const auto pIsMoveForward = ::std::make_shared<bool>(false);
  const auto pCursorCurrentPosition = ::std::make_shared<Coord>();
  const auto pCursorTouchPosition = ::std::make_shared<Coord>();
  const auto pTime = ::std::make_shared<float>(0.0f);

  m_Events[Cursor.Motion].Connect([=](const Cursor_t::Position & _Position)
  {
    *pCursorCurrentPosition = { _Position.X, _Position.Y };

    if (!*pIsTouch || *pIsMoveForward) return;

    const auto dX = pCursorCurrentPosition->X - pCursorTouchPosition->X;
    const auto dY = pCursorCurrentPosition->Y - pCursorTouchPosition->Y;

    if (abs(dY) > 25 &&
      abs(dY) > 3 * abs(dX) && // Вертикальное движение мышью
      dY < 0)
    {
      *pIsMoveForward = true;
      *pTime = 0.0f;
    }
  });

  m_Events[Drawing.Do].Connect([=](void)
  {
    using namespace ::std::chrono;

    static const auto BeginTime = system_clock::now();

    const duration<float> Time = system_clock::now() - BeginTime;

    if (*pIsMoveForward && 
      (Time.count() - *pTime) >= 0.5f)
    {
      *pTime = Time.count();
      m_Events[::events::Demo.MoveForward]();
    }
  });

  m_Events[Cursor.Touch].Connect([=](void) 
  {
    *pIsTouch = true;
    *pCursorTouchPosition = *pCursorCurrentPosition;
  });

  m_Events[Cursor.Release].Connect([=](void) 
  {
    *pIsTouch = false;
    *pIsMoveForward = false;

    const auto dX = pCursorCurrentPosition->X - pCursorTouchPosition->X;
    const auto dY = pCursorCurrentPosition->Y - pCursorTouchPosition->Y;

    if (abs(dX) < 25 && abs(dY) < 25) return;

    ::events::Demo_t::Id Action;

    if (abs(dX) > 3 * abs(dY)) // Горизонтальное движение мышью
    {
      if (dX > 0) Action = ::events::Demo.TurnRight;
      else Action = ::events::Demo.TurnLeft;
    }
    else if (abs(dY) > 3 * abs(dX)) // Вертикальное движение мышью
    {
      if (dY > 0)
      {
        Action = ::events::Demo.MoveBackward;
      }
      else
      {
        return;
      }
    }
    else // Диагональное движение мышью
    {
      if (dY < 0)
      {
        if (dX > 0) Action = ::events::Demo.MoveRightForward;
        else Action = ::events::Demo.MoveLeftForward;
      }
      else
      {
        if (dX > 0) Action = ::events::Demo.MoveRightBackward;
        else Action = ::events::Demo.MoveLeftBackward;
      }
    }

    m_Events[Action]();
  });
}

auto Demo::GetProcessLoading(void) -> Updater_t
{
  return [=](void)
  {
    const auto Value = uT("%VALUE%")
      .Replace(uT("%VALUE%"), m_PercentLoading / 100.0f);

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
