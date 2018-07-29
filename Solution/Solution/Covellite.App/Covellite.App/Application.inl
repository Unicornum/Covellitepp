
#pragma once
#include <Covellite\App\IWindow.hpp>

namespace covellite
{

namespace app
{

/**
* \brief
*  Функция создания объекта окна программы.
* \details
*  - Функция создает объект окна программы класса, указанного в качестве
*  параметра шаблона.
*  - Функция должна быть вызвана в классе главного объекта программы
*  в функции обработки сообщения Application.Create для всего необходимого
*  приложению набора объектов окон (окно операционной системы, окно
*  графического API, окно реализации приложения, окно реализации libRocket).
*
* \tparam TWindow
*  Класс окна программы (должен быть наследником от класса 
*  covellite::app::IWindow).
*
* \param _Args
*  Параметры, передаваемые конструктору создаваемого объекта окна.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
template<class TWindow, class ... TArgs>
inline TWindow & Application::MakeWindow(TArgs && ... _Args)
{
  static_assert(::std::is_convertible<TWindow &, IWindow &>::value, 
    "TWindow must inherit covellite::app::IWindow.");

  auto pWindow = ::std::make_shared<TWindow>(_Args ...);
  m_Windows.push(pWindow);
  return *pWindow;
}

/**
* \brief
*  Функция точки входа в программу.
* \details
*  - Именно эта функция вызывается внутри макроса точки входа в программу.
*  
* \tparam TApplication
*  Класс приложения (должен наследоваться от covellite::app::Application).
*  
* \param [in] _pParams
*  Параметры запуска программы:
*  - Windows: параметры командной строки.
*  - Android: указатель на объект android_main.
*/
template<class TApplication>
inline /*static*/ void Application::Main(void * _pParams) noexcept
{
  static_assert(::std::is_convertible<TApplication &, Application &>::value,
    "TApplication must inherit covellite::app::Application.");

  using ApplicationPtr_t = ::std::unique_ptr<Application>;

  auto fnCreateApplication =
    [](void) -> ApplicationPtr_t { return ::std::make_unique<TApplication>(); };

  Main(fnCreateApplication, _pParams);
}

} // namespace app

} // namespace covellite
