
#pragma once
#include <memory>
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Api/Renders.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Интерфейсный класс для всех окон, реализующих различные графические API.
*  
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
*  1.1.0.0        \n
* \date
*  29 Сентябрь 2017    \n
*  20 Август 2018    \n
*  19 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using RendersPtr_t = ::std::shared_ptr<Component::Renders>;

public:
  /// Функция получения размеров клиентской области окна.
  virtual Rect GetClientRect(void) const = 0;
  /// Функция получения объекта создания/удаления рендеров для компонентов.
  virtual RendersPtr_t GetRenders(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace api

} // namespace covellite
