
#pragma once
#include "Layer.hpp"

namespace layers
{

class Help_t
{
public:
  // Идентификаторы событий уровня проекта примера.
  enum Id
  {
    Title = 0,
    Text,
  };
};

namespace { Help_t Help; }

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс окна описания слоя.
* \details
* \todo
*  Добавить описание событий, по которому окно получает заголовок и текст 
*  описания слоя.
*  
* \version
*  1.0.0.0        \n
* \date
*  14 Январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Description final :
  public Layer
{
public:
  explicit Description(IWindowGui_t &);
};

} // namespace layers
