
#pragma once
#include <memory>
#include <windows.h>
#include <alicorn\std\exception.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\std\chrono.mock.hpp>
#include <alicorn\platform\app-info.hpp>
#include <alicorn\platform\environment.mock.hpp>
#include <alicorn\logger.mock.hpp>
#include <alicorn\settings.mock.hpp>
#include <Platform\Android.mock.hpp>

#define RenderInterface RenderInterface_incomplete
#include <Rocket/Rocket.mock.hpp>
#undef RenderInterface

namespace mock
{

namespace Rocket
{

namespace Core
{

// 25 јвгуст 2018 10:33 (unicornum.verum@gmail.com)
TODO("”далить, когда будет дополнен оригинал");
class RenderInterface :
  public RenderInterface_incomplete
{
public:
  virtual CompiledGeometryHandle CompileGeometry(Vertex *, int, int *, int,
    TextureHandle) { throw ::std::exception{}; }
  virtual void RenderCompiledGeometry(CompiledGeometryHandle, const Vector2f &) {}
  virtual void ReleaseCompiledGeometry(CompiledGeometryHandle) {}

public:
  explicit RenderInterface(::mock::Id_t _Id = 0) : RenderInterface_incomplete(_Id) {}
};

} // namespace Core

} // namespace Rocket

} // namespace mock

namespace testing { namespace internal {

// Ёто чтобы не падал gmock при возврате boost::filesystem::path.
inline void PrintTo(const ::boost::filesystem::path & p, ::std::ostream * os)
{
  *os << p;
}

} }

#include <alicorn\application\current-module.mock.hpp>

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include "Mock\Namespaces.hpp"

#include <Covellite\Core.mock.hpp>
#include <Covellite\Core\Settings.mock.hpp>
#include <Covellite\Gui.inside.mock.hpp>

#include "Mock\Time.hpp"
#include "Mock\StringTranslator.hpp"
#include "Mock\RocketInterfaces.hpp"
#include "Mock\SystemToRocketKeyCode.inl"
#include "Mock\Renderer.hpp"
