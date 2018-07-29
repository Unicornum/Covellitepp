
#include "stdafx.h"
#include <Covellite/Os/Window.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/IApplication.hpp>
#include <Covellite/Os/Events.hpp>

using AppInfo_t = ::alicorn::system::platform::AppInfo;

using namespace covellite::os;

/**
* \deprecated
*  Устаревший конструктор, используемый совместно с классами из проекта
*  Covellite.Core, вместо них использовать классы из Covellite.App.
*/
Window::Window(void) :
  m_Handle(AppInfo_t::Get<ANativeWindow *>())
{
}

Window::Window(const ::covellite::app::IApplication & _Application) :
  m_Events(_Application),
  m_Handle(AppInfo_t::Get<ANativeWindow *>())
{
  m_Events[(int32_t)APP_CMD_WINDOW_REDRAW_NEEDED].Connect([&]()
  {
    m_Events[events::Window.Resize]();
  });

  using AInputEvent_t = AInputEvent *;

  const int32_t Motion = 
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_MOVE;

  m_Events[Motion].Connect([&](const AInputEvent_t & _pEvent)
  {
    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
  });

  const int32_t Touch =
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_DOWN;

  m_Events[Touch].Connect([&](const AInputEvent_t & _pEvent)
  {
    // 08 Июль 2018 10:53 (unicornum.verum@gmail.com)
    TODO("Отдельный Motion не нужен?");

    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
    m_Events[events::Cursor.Touch]();
  });

  const int32_t Release =
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_UP;

  m_Events[Release].Connect([&](const AInputEvent_t & _pEvent)
  {
    // 08 Июль 2018 10:53 (unicornum.verum@gmail.com)
    TODO("Отдельный Motion не нужен?");

    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
    m_Events[events::Cursor.Release]();
  });

  const int32_t Pressed =
    (AINPUT_EVENT_TYPE_KEY << 8) | events::Key_t::APP_CMD_KEY_PRESSED;

  m_Events[Pressed].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    m_Events[events::Key.Pressed](Code);
  });

  const int32_t Down =
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_DOWN;

  m_Events[Down].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    m_Events[events::Key.Down](Code);
  });

  const int32_t Up =
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_UP;

  m_Events[Up].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    if (Code == AKEYCODE_BACK)
    {
      m_Events[events::Key.Back]();
    }
    else if (Code == AKEYCODE_UNKNOWN)
    {
      // 08 Июль 2018 11:21 (unicornum.verum@gmail.com)
      TODO("Код кнопки Settings?");
      m_Events[events::Key.Settings]();
    }
    else
    {
      m_Events[events::Key.Up](Code);
    }
  });
}

Window::~Window(void) = default;
