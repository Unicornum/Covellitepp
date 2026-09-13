
#pragma once

namespace covellite::api
{

//class Сomponent;

// 13 Сентябрь 2026 17:47 (unicornum.verum@gmail.com)
TODO("Добавить описание класса.");
/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  < Краткое описание класса >
* \details
*  - Подробное описание класса.
*  
* \version
*  1.0.0.0        \n
* \date
*  13 Сентябрь 2026    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2026
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
class CompileShader final
{
  using ComponentPtr_t = ::std::shared_ptr<class Component>;

public:
  static void AsHLSL(const ComponentPtr_t &);
  static void AsGLSL(const ComponentPtr_t &);
};

} // namespace covellite::api
