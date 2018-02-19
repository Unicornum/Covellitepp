
#pragma once
#include <alicorn\std\string.forward.hpp>

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
class IWindow
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using RenderInterfacePtr_t = ::std::shared_ptr<Rocket::Core::RenderInterface>;

public:
  virtual String_t GetUsingApi(void) const = 0;
  virtual int32_t GetWidth(void) const = 0;
  virtual int32_t GetHeight(void) const = 0;
  virtual RenderInterfacePtr_t MakeRenderInterface(void) const = 0;

public:
  virtual ~IWindow(void) noexcept {}
};

} // namespace api

} // namespace covellite
