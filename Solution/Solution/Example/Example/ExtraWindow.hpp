
#pragma once
#include <Covellite\Covellite.hpp>

/**
* \ingroup gExampleClasses
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
  public covellite::core::IWindow
{
public:
  void Subscribe(const EventHandlerPtr_t &) override;
};
