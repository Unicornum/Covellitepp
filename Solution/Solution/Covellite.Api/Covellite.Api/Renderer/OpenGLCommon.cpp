
#include "stdafx.h"
#include "OpenGLCommon.hpp"
#include <glm/glm.force.hpp>
#include <alicorn/std/string/encoding.hpp>
#include "Component.hpp"
#include "Shaders/Shaders.hpp"
#include "GraphicApi.Constants.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLCommon::OpenGLCommon(const Data_t & _Data, const String_t & _PreVersion) :
  m_Top{ _Data.Top },
  m_Width{ _Data.Width },
  m_Height{ _Data.Height },
  m_PreVersion{ _PreVersion }
{
}

OpenGLCommon::String_t OpenGLCommon::GetUsingApi(void) const /*override*/
{
  using namespace ::alicorn::extension::std;

  const ::std::string Version = 
    reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return m_PreVersion + string_cast<String, Encoding::Ascii128>(Version);
}

void OpenGLCommon::ResizeWindow(int32_t _Width, int32_t _Height) noexcept /*final*/
{
  m_Width = _Width;
  m_Height = _Height;

  m_IsResizeWindow = true;
}

auto OpenGLCommon::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CreateBlendState = [](void) noexcept
  {
    return [](void) noexcept
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };
  };

  const auto CreateSamplerState = [&](void)
  {
    return [=](void)
    {
      m_TexParameters.MinFilter = GL_LINEAR;
      m_TexParameters.MagFilter = GL_LINEAR;
      m_TexParameters.WrapS = GL_REPEAT;
      m_TexParameters.WrapT = GL_REPEAT;
    };
  };

  const auto CreateScissorState = [&](void)
  {
    const auto pScissorRect = CapturingServiceComponent::Get(_pComponent, 
      { { uT("Rect"), api::Component::Make({}) } })[0];

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor Rect{ *pScissorRect };

      glEnable(GL_SCISSOR_TEST);

      GLint ViewPort[4] = { 0 };
      glGetIntegerv(GL_VIEWPORT, ViewPort);

      // (x, y) - левый нижний угол!
      glScissor(Rect.Left, ViewPort[3] - Rect.Bottom,
        Rect.Right - Rect.Left, Rect.Bottom - Rect.Top);
    };

    Render_t ScissorDisabled = [](void) noexcept
    {
      glDisable(GL_SCISSOR_TEST);
    };

    const Component::Scissor Scissor{ *_pComponent };
    return (Scissor.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  const auto CreateDepthState = [&](void)
  {
    return GetDepthRender(
      (*_pComponent)[uT("enabled")].Default(false),
      (*_pComponent)[uT("clear")].Default(false),
      (*_pComponent)[uT("overwrite")].Default(true));
  };

  const auto CreateClearState = [&](void)
  {
    // ?????????????????????????????????????????????????????????????????????? //
    // Использование здесь ::glm::vec4 приводит к падению программы в Release
    // версии при копировании созданного рендера в 
    // Component::Renders::m_AllExistingRenders.
    class Color { public: float r, g, b, a; };

    const auto BackgroundColor = ARGBtoFloat4<Color>(
      (*_pComponent)[uT("color")].Default(0xFF000000));
    // ?????????????????????????????????????????????????????????????????????? //

    return [=](void) noexcept
    {
      glClearColor(
        BackgroundColor.r,
        BackgroundColor.g,
        BackgroundColor.b,
        BackgroundColor.a);
      glClear(GL_COLOR_BUFFER_BIT);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"),     CreateBlendState     },
    { uT("Sampler"),   CreateSamplerState   },
    { uT("Scissor"),   CreateScissorState   },
    { uT("Depth"),     CreateDepthState     },
    { uT("Clear"),     CreateClearState     },
  };

  return Creators[_pComponent->Kind]();
}

auto OpenGLCommon::GetDepthRender(
  const bool _IsEnabled, 
  const bool _IsClear,
  const bool _IsOverwrite) -> Render_t
{
  const Render_t DepthDisable = [](void) noexcept
  {
    glDisable(GL_DEPTH_TEST);
  };

  const auto IsOverwrite = 
    static_cast<GLboolean>(_IsOverwrite ? GL_TRUE : GL_FALSE);

  const Render_t DepthEnable = [=](void) noexcept
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(IsOverwrite);
    glDepthFunc(GL_GREATER);
  };

  const Render_t DepthClear = [=](void) noexcept
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(IsOverwrite);
    glDepthFunc(GL_GREATER);
    glClearDepth(0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
  };

  return _IsEnabled ? (_IsClear ? DepthClear : DepthEnable) : DepthDisable;
}

// cppcheck-suppress unusedFunction
auto OpenGLCommon::GetPreRenderGeometry(const ComponentPtr_t & _pComponent) -> MatrixBuilder_t
{
  ::std::deque<MatrixBuilder_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pPosition](::glm::mat4 & _Matrix)
    {
      const Component::Position Pos{ *_pPosition };
      _Matrix = ::glm::translate(_Matrix, ::glm::vec3{ Pos.X, Pos.Y, Pos.Z });
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pRotation](::glm::mat4 & _Matrix)
    {
      const Component::Rotation Rot{ *_pRotation };
      _Matrix = ::glm::rotate(_Matrix, Rot.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      _Matrix = ::glm::rotate(_Matrix, Rot.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      _Matrix = ::glm::rotate(_Matrix, Rot.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pScale](::glm::mat4 & _Matrix)
    {
      const Component::Scale Scale{ *_pScale };
      _Matrix = ::glm::scale(_Matrix, ::glm::vec3{ Scale.X, Scale.Y, Scale.Z });
    });
  };

  CapturingServiceComponent::Process(_pComponent,
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  return [PreRenders](::glm::mat4 & _Matrix)
  {
    for (auto & Render : PreRenders) Render(_Matrix);
  };
}

// cppcheck-suppress unusedFunction
auto OpenGLCommon::GetPreRenderBillboardGeometry(const ComponentPtr_t & _pComponent) -> MatrixBuilder_t
{
  ::std::deque<MatrixBuilder_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pPosition](::glm::mat4 & _Matrix)
    {
      const Component::Position Pos{ *_pPosition };
      _Matrix = ::glm::translate(_Matrix, ::glm::vec3{ Pos.X, Pos.Y, Pos.Z });
    });
  };

  CapturingServiceComponent::Process(_pComponent,
    { { uT("Position"), CreatePosition } });

  PreRenders.push_front([=](::glm::mat4 & _Matrix)
  {
    _Matrix[0][3] = 0.0f;
    _Matrix[1][3] = 0.0f;
    _Matrix[2][3] = 0.0f;
    _Matrix[3][0] = 0.0f;
    _Matrix[3][1] = 0.0f;
    _Matrix[3][2] = 0.0f;
    _Matrix[3][3] = 1.0f;
  });

  return [PreRenders](::glm::mat4 & _Matrix)
  {
    for (auto & Render : PreRenders) Render(_Matrix);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
