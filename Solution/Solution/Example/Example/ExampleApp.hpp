
#pragma once
#include <Covellite\Covellite.hpp>

/**
* \ingroup gExampleClasses
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс главного объекта (точка входа) программы.
*  
* \version
*  1.0.0.0        \n
* \date
*  17 Ноябрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class ExampleApp final :
  public covellite::core::Application
{
private:
  void DoInitLogger(void);
  void DoInitWindow(void);

public:
  ExampleApp(void);
  ~ExampleApp(void) noexcept;
};
