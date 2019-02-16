
#pragma once
#include <memory>
#include <Covellite/Covellite.hpp>

namespace basement { class IBasement; }

// 10 Февраль 2019 19:06 (unicornum.verum@gmail.com)
TODO("Требуется более подходящее название класса.");
/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Простой пример класса промежуточного кода.
*  
* \version
*  1.0.0.0        \n
* \date
*  04 Ноябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class ExtraWindow final :
  public ::covellite::app::IWindow
{
  using Events_t = ::covellite::events::Events;
  using WindowApi_t = ::covellite::api::IWindow;

public:
  operator const WindowApi_t & (void) const;

private:
  const WindowApi_t & m_WindowApi;
  Events_t            m_Events;
  ::std::shared_ptr<::basement::IBasement> m_pBasement;

public:
  explicit ExtraWindow(const WindowApi_t &);
};
