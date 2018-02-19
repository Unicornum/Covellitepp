
#pragma once
#include <Covellite\Core\IWindow.hpp>

namespace covellite
{

namespace core
{

/**
* \brief
*  Функция создания объекта окна программы.
* \details
*  - Функция создает объект окна программы класса, указанного в качестве
*  параметра шаблона и подписывает созданное окно на события приложения.
*  - Функция должна быть вызвана в классе главного объекта программы
*  в функции обработки сообщения EventHandler::Create для всего необходимого
*  приложению набора объектов окон (окно операционной системы, окно
*  графического API, окно реализации приложения, окно реализации libRocket).
*
* \tparam TWindow
*  Класс окна программы (должен быть наследником от класса covellite::IWindow).
*
* \param _Args
*  Параметры, передаваемые конструктору создаваемого объекта окна.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
template<class TWindow, class ... TArgs>
::std::shared_ptr<TWindow> Application::MakeWindow(TArgs && ... _Args)
{
  auto pWindow = ::std::make_shared<TWindow>(_Args ...);
  m_Windows.push(pWindow);
  m_Windows.top()->Subscribe(m_pEvents);
  return pWindow;
}

/**
* \brief
*  Функция точки входа в программу.
* \details
*  - Именно эта функция вызывается внутри макроса точки входа в программу.
*  
* \tparam TApplication
*  Класс приложения (должен наследоваться от covellite::core::Application).
*  
* \param [in] _pParams
*  Параметры запуска программы:
*  - Windows: параметры командной строки.
*  - Android: указатель на объект android_main.
*/
template<class TApplication>
inline /*static*/ void Application::Main(void * _pParams) noexcept
{
  using ApplicationPtr_t = ::std::unique_ptr<Application>;

  auto fnCreateApplication =
    [](void) -> ApplicationPtr_t { return ::std::make_unique<TApplication>(); };

  Main(fnCreateApplication, _pParams);
}

} // namespace core

} // namespace covellite
