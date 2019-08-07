
#pragma once

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/logger.mock.hpp>
#include <alicorn/platform/environment.mock.hpp>
#include <Platform/Windows.mock.hpp>
#include <Platform/Android.mock.hpp>

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
class RenderInterface
{
public:
  const ::mock::Id_t m_Id;

public:
  virtual ~RenderInterface(void) {}
  virtual void RenderGeometry(Vertex *, int, int *, int, TextureHandle,
    const Vector2f &) {}
  virtual CompiledGeometryHandle CompileGeometry(Vertex *, int, int *, int,
    TextureHandle) { throw ::std::exception{}; }
  virtual void RenderCompiledGeometry(CompiledGeometryHandle, const Vector2f &) {}
  virtual void ReleaseCompiledGeometry(CompiledGeometryHandle) {}
  virtual void EnableScissorRegion(bool) {}
  virtual void SetScissorRegion(int, int, int, int) {}
  virtual bool LoadTexture(TextureHandle &, Vector2i &, const String &) { return false; }
  virtual bool GenerateTexture(TextureHandle &, const byte *, const Vector2i &) { return false; }
  virtual void ReleaseTexture(TextureHandle) {}
  virtual float GetVerticalTexelOffset(void) { throw ::std::exception(); }

public:
  explicit RenderInterface(::mock::Id_t _Id = 0) : m_Id(_Id) {}
};

} // namespace Core

} // namespace Rocket

} // namespace mock

#include "Mock/Namespaces.hpp"

#include <Covellite/App/Settings.mock.hpp>
#include <Covellite/Os/Configuration.mock.hpp>
