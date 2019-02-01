
#include "stdafx.h"
#include <Covellite/Os/Window.hpp>
#include <alicorn/platform/environment.hpp>
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

  auto BuildMessage = [](int32_t _Type, int32_t _Value) -> int32_t
  {
    return (_Type << 8) | _Value;
  };

  const auto Motion = 
    BuildMessage(AINPUT_EVENT_TYPE_MOTION, AMOTION_EVENT_ACTION_MOVE);

  m_Events[Motion].Connect([&](const AInputEvent_t & _pEvent)
  {
    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
  });

  const auto Touch =
    BuildMessage(AINPUT_EVENT_TYPE_MOTION, AMOTION_EVENT_ACTION_DOWN);

  m_Events[Touch].Connect([&](const AInputEvent_t & _pEvent)
  {
    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    // Отдельный Motion необходим, т.к. Android не генерирует отдельное
    // сообщение при касании пальцем экрана.
    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
    m_Events[events::Cursor.Touch]();
  });

  const auto Release =
    BuildMessage(AINPUT_EVENT_TYPE_MOTION, AMOTION_EVENT_ACTION_UP);

  m_Events[Release].Connect([&](const AInputEvent_t & _pEvent)
  {
    const auto X = static_cast<int32_t>(AMotionEvent_getX(_pEvent, 0));
    const auto Y = static_cast<int32_t>(AMotionEvent_getY(_pEvent, 0));

    // Отдельный Motion необходим, т.к. Android не генерирует отдельное
    // сообщение при отпускании пальцем экрана.
    m_Events[events::Cursor.Motion](events::Cursor_t::Position{ X, Y });
    m_Events[events::Cursor.Release]();
  });

  const auto Pressed =
    BuildMessage(AINPUT_EVENT_TYPE_KEY, events::Key_t::APP_CMD_KEY_PRESSED);

  m_Events[Pressed].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    m_Events[events::Key.Pressed](Code);
  });

  const auto Down =
    BuildMessage(AINPUT_EVENT_TYPE_KEY, AKEY_EVENT_ACTION_DOWN);

  m_Events[Down].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    m_Events[events::Key.Down](Code);
  });

  const auto Up =
    BuildMessage(AINPUT_EVENT_TYPE_KEY, AKEY_EVENT_ACTION_UP);

  m_Events[Up].Connect([&](const AInputEvent_t & _pEvent)
  {
    const events::Key_t::Code Code = AKeyEvent_getKeyCode(_pEvent);

    if (Code == AKEYCODE_BACK)
    {
      m_Events[events::Key.Back]();
    }
    else if (Code == AKEYCODE_MENU)
    {
      m_Events[events::Key.Menu]();
    }
    else
    {
      m_Events[events::Key.Up](Code);
    }
  });
}

Window::~Window(void) = default;

/**
* \brief
*  Функция получения размеров клиентской области окна.
* \details
*  - Для Android'a следует иметь в виду, что если в манифесте не отключено
*  отображение системного заголовка, то \b Top будет содержать высоту этого 
*  заголовка, а \b Height будет содержать высоту окна, включающую высоту 
*  заголовка.
*  - При вычислениях доступной для рисования 2D объектов области следует 
*  учитывать, что реализации рендера для Android устанавливают viewport, который 
*  уже учитывает высоту системного заголовка (т.е. точка с координатами (0,0) 
*  находится в левом верхнем углу, непосредственно под заголовком, а высота 
*  viewport'a равна (Height - Top)).
*  
* \return \b Value
*  В случае (таком-то...).
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
Window::Rect Window::GetClientRect(void) const /*override*/
{
  return 
  {
    0,
    ::alicorn::system::platform::Environment{}.GetStatusBarHeight(),
    ANativeWindow_getWidth(m_Handle),
    ANativeWindow_getHeight(m_Handle)
  };
}
