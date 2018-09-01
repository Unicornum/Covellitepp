
#pragma once
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Os/IWindow.hpp>

namespace std { template<class> class shared_ptr; }
namespace Rocket { namespace Core { class RenderInterface; } }

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
* \date
*  29 Сентябрь 2017    \n
*  20 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using RenderInterfacePtr_t = ::std::shared_ptr<Rocket::Core::RenderInterface>;
  using Rect_t = ::covellite::os::IWindow::Rect;

public:
  virtual Rect_t GetClientRect(void) const = 0;
  virtual RenderInterfacePtr_t GetRenderInterface(void) const = 0;

  /// \deprecated
  ///  Функция устарела и будет удалена в следующей стабильной версии,
  ///  используйте вместо нее функцию GetClientRect().
  virtual int32_t GetWidth(void) const = 0;
  /// \deprecated
  ///  Функция устарела и будет удалена в следующей стабильной версии,
  ///  используйте вместо нее функцию GetClientRect().
  virtual int32_t GetHeight(void) const = 0;
  /// \deprecated
  ///  Функция устарела и будет удалена в следующей стабильной версии,
  ///  используйте вместо нее функцию GetRenderInterface().
  virtual RenderInterfacePtr_t MakeRenderInterface(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace api

} // namespace covellite
