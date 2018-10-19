
#include "stdafx.h"
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Core\ClickEventListener.hpp>

using AppInfo_t = ::alicorn::system::platform::AppInfo;

using namespace covellite::core;

/**
* \brief
*  Функция вызова сигнала для системного сообщения.
* \details
*  - При первом старте, а также повторном старте после прерывания главной
*  кнопкой программа (в обоих случаях заново запускается функция
*  android_main()) получает сообщения:
*   + APP_CMD_START
*   + APP_CMD_RESUME
*   + APP_CMD_INPUT_CHANGED
*   + APP_CMD_INIT_WINDOW
*   + APP_CMD_CONTENT_RECT_CHANGED
*   + APP_CMD_GAINED_FOCUS
*   + APP_CMD_WINDOW_REDRAW_NEEDED
*  - При активации и разблокировке устройства:
*   + APP_CMD_START
*   + APP_CMD_RESUME
*   + APP_CMD_GAINED_FOCUS
*  - При отключении экрана:
*   + APP_CMD_PAUSE
*   + APP_CMD_SAVE_STATE
*   + APP_CMD_STOP
*   + APP_CMD_LOST_FOCUS
*  - При нажатии HOME во время работы:
*   + APP_CMD_PAUSE
*   + APP_CMD_LOST_FOCUS
*   + APP_CMD_SAVE_STATE
*   + APP_CMD_STOP - после этого возникает ошибка рисования
*  - При перекрытии программы другой программой:
*   + APP_CMD_PAUSE - после этого возникает ошибка рисования
*   + APP_CMD_LOST_FOCUS
*   + APP_CMD_TERM_WINDOW
*  - При вызове ANativeActivity_finish():
*   + APP_CMD_PAUSE
*   + APP_CMD_LOST_FOCUS
*   + APP_CMD_TERM_WINDOW
*  - При остановке/удалении программы из диспетчера задач никаких сообщений
*  не приходит.
*  - Отслеживать изменение ориентации следует по событию
*  \b APP_CMD_WINDOW_REDRAW_NEEDED (событиями \b APP_CMD_CONFIG_CHANGED и
*  \b APP_CMD_CONTENT_RECT_CHANGED лучше не пользоваться, т.к. при получении
*  этого события функции класса Window GetWidth() и GetHeight() могут
*  возвращать старые размеры экрана).
*
* \param [in] _pApp
*  Информация о текущем приложении.
* \param [in] _Message
*  Событие операционной системы.
*/
void CommandHandlerApp(android_app * _pApp, int32_t _Message) noexcept
{
  if (_pApp == nullptr || _pApp->userData == nullptr) return;
  auto & oEvents = *(reinterpret_cast<EventHandler *>(_pApp->userData));

  static const ::std::map<int32_t, Event::System> System =
  {
    { APP_CMD_STOP, Event::Stop },
    { APP_CMD_PAUSE, Event::Pause },
    { APP_CMD_RESUME, Event::Resume },
    { APP_CMD_DESTROY, Event::Destroy },
  };

  auto itSystemCommand = System.find(_Message);
  if (itSystemCommand != System.end())
  {
    oEvents.DoCommand(itSystemCommand->second, params::Empty{});
    return;
  }

  static const ::std::map<int32_t, Event::Window> Window =
  {
    { APP_CMD_INIT_WINDOW, Event::Create },
    { APP_CMD_TERM_WINDOW, Event::Close },
    { APP_CMD_LOST_FOCUS, Event::LostFocus },
    { APP_CMD_GAINED_FOCUS, Event::GainedFocus },
    { APP_CMD_WINDOW_REDRAW_NEEDED, Event::Resize },
  };

  auto itWindowCommand = Window.find(_Message);
  if (itWindowCommand != Window.end())
  {
    oEvents.DoCommand(itWindowCommand->second, params::Empty{});
    return;
  }
}

/**
* \brief
*  Функция вызова сигнала для событий пользовательского ввода.
*
* \param [in] _pApp
*  Информация о текущем приложении.
* \param [in] _pInputEvent
*  Информация о событии пользовательского ввода.
*
* \return \b true
*  Событие было обработано.
* \return \b false
*  Событие не было обработано, требуется обработка по умолчанию.
*/
bool CommandHandlerInput(android_app * _pApp, AInputEvent * _pInputEvent) noexcept
{
  if (_pInputEvent == nullptr || 
    _pApp == nullptr || _pApp->userData == nullptr) return false;
  auto & oEvents = *(reinterpret_cast<EventHandler *>(_pApp->userData));

  const auto EventType = AInputEvent_getType(_pInputEvent);

  if (EventType == AINPUT_EVENT_TYPE_MOTION)
  {
    const auto ActionType = AMotionEvent_getAction(_pInputEvent);

    if (ActionType == AMOTION_EVENT_ACTION_MOVE)
    {
      return oEvents.DoCommand(Event::Motion, params::Motion{
        static_cast<int32_t>(AMotionEvent_getX(_pInputEvent, 0)),
        static_cast<int32_t>(AMotionEvent_getY(_pInputEvent, 0)) });
    }

    if (ActionType == AMOTION_EVENT_ACTION_DOWN)
    {
      const auto IsSuccess = oEvents.DoCommand(Event::Motion, params::Motion{
        static_cast<int32_t>(AMotionEvent_getX(_pInputEvent, 0)),
        static_cast<int32_t>(AMotionEvent_getY(_pInputEvent, 0)) });
      if (!IsSuccess) return false;

      return oEvents.DoCommand(Event::Touch, params::Empty{});
    }

    if (ActionType == AMOTION_EVENT_ACTION_UP)
    {
      return oEvents.DoCommand(Event::Release, params::Empty{});
    }
  }

  if (EventType == AINPUT_EVENT_TYPE_KEY)
  {
    auto GetKeyCode =
      [&]() -> int32_t { return AKeyEvent_getKeyCode(_pInputEvent); };

    const auto ActionType = AKeyEvent_getAction(_pInputEvent);

    if (ActionType == Event::APP_CMD_KEY_PRESSED)
    {
      // Управляющие символы (кроме перевода строки) должны передавать через 
      // сигнал KeyUp.
      if (GetKeyCode() < 32 && GetKeyCode() != 10) return false;

      return oEvents.DoCommand(Event::KeyPressed, params::KeyPressed{ GetKeyCode() });
    }

    if (ActionType == AKEY_EVENT_ACTION_DOWN)
    {
      return oEvents.DoCommand(Event::KeyDown, params::KeyCode{ GetKeyCode() });
    }

    if (ActionType == AKEY_EVENT_ACTION_UP)
    {
      if (GetKeyCode() == AKEYCODE_BACK)
      {
        const auto IsSuccess = oEvents.DoCommand(Event::Back, params::Empty{});
        if (!IsSuccess) return false;
      }

      return oEvents.DoCommand(Event::KeyUp, params::KeyCode{ GetKeyCode() });
    }
  }

  return false;
}

EventHandler::EventHandler(void) :
  m_pClickEventListener(::std::make_unique<ClickEventListener>(OnRocket, OnFramework)),
  m_fnCleaner([](void)
  {
    auto & App = AppInfo_t::Get<android_app>();

    App.onAppCmd = nullptr;
    App.onInputEvent = nullptr;
    App.userData = nullptr;
  })
{
  auto & App = AppInfo_t::Get<android_app>();

  App.userData = this;
  App.onAppCmd = ::CommandHandlerApp;

  App.onInputEvent = [](android_app * _pApp, AInputEvent * _pEvent) -> int32_t
  {
    return (::CommandHandlerInput(_pApp, _pEvent)) ? 1 : 0;
  };

  (*this)[Event::Destroy]
    .connect(::std::bind(&EventHandler::OnDestroy, this));
  (*this)[Event::Pause]
    .connect([&](const ::covellite::core::Params &) {m_IsPaused = true; });
  (*this)[Event::Resume]
    .connect([&](const ::covellite::core::Params &) {m_IsPaused = false; });
}
