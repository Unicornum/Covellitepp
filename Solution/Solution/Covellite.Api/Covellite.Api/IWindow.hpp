
#pragma once
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Events/IEvents.hpp>

namespace std { template<class> class shared_ptr; }
namespace Rocket { namespace Core { class RenderInterface; } }

namespace covellite
{

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Интерфейсный класс для всех окон, реализующих различные графические API.
*  
* \version
*  1.0.0.0        \n
* \date
*  29 Сентябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using RenderInterfacePtr_t = ::std::shared_ptr<Rocket::Core::RenderInterface>;

public:
  class Rect
  {
  public:
    int32_t Left;
    int32_t Top;
    int32_t Width;
    int32_t Height;
  };

public:
  virtual String_t GetUsingApi(void) const = 0;
  /// \deprecated
  ///  Функция устарела и будет удалена в следующей стабильной версии,
  ///  используйте вместо нее функцию GetClientRect().
  virtual int32_t GetWidth(void) const = 0;
  /// \deprecated
  ///  Функция устарела и будет удалена в следующей стабильной версии,
  ///  используйте вместо нее функцию GetClientRect().
  virtual int32_t GetHeight(void) const = 0;
  virtual Rect GetClientRect(void) const = 0;
  virtual RenderInterfacePtr_t MakeRenderInterface(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace api

} // namespace covellite
