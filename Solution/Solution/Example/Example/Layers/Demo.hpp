
#pragma once
#include "Layer.hpp"

namespace layers
{

// 15 Январь 2019 18:30 (unicornum.verum@gmail.com)
TODO("Добавить описание класса.");
/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  < Краткое описание класса >
* \details
*  - Подробное описание класса.
*  
* \version
*  1.0.0.0        \n
* \date
*  15 Январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*  
*  Пример использования:
*  ---------------------
*  
* ### Первый пример
*  
* \code
// Здесь должен быть пример использования...
// ...
* \endcode
*/
class Demo final :
  public Layer
{
public:
  explicit Demo(IWindowGui_t &);
};

} // namespace layers
