
#pragma once
#include <Covellite/Os/Predefined.hpp>
#include <Covellite/Api/RenderInterface.hpp>

namespace mock
{

namespace covellite
{

namespace api
{

namespace renderer
{

class Renderer :
  public ::covellite::api::RenderInterface
{
  using String_t = ::alicorn::extension::std::String;

public:
  class Data final
  {
  public:
    class Color final
    {
    public:
      float R, G, B, A;
    };

  public:
    Handle_t  Handle;      
    int       Top;         
    Color     BkColor;     
    bool      IsFullScreen;
                           
  };

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, ::mock::Id_t(const String_t &, const Data &));
    MOCK_METHOD1(GetRenders, RendersPtr_t(::mock::Id_t));
    MOCK_METHOD1(StartDrawingFrame, void(::mock::Id_t));
    MOCK_METHOD1(PresentFrame, void(::mock::Id_t));
    MOCK_METHOD3(ResizeWindow, void(::mock::Id_t, int32_t, int32_t));
  };

public:
  const ::mock::Id_t m_RendererId;

public:
  RendersPtr_t GetRenders(void) const override
  {
    return Proxy::GetInstance()->GetRenders(m_RendererId);
  }

public:
  void StartDrawingFrame(void)
  {
    Proxy::GetInstance()->StartDrawingFrame(m_RendererId);
  }

  void PresentFrame(void)
  {
    Proxy::GetInstance()->PresentFrame(m_RendererId);
  }

  void ResizeWindow(int32_t _Width, int32_t _Height)
  {
    Proxy::GetInstance()->ResizeWindow(m_RendererId, _Width, _Height);
  }

public:
  Renderer(const String_t & _ApiName, const Data & _Data) :
    m_RendererId(Proxy::GetInstance()->Constructor(_ApiName, _Data))
  {

  }
};

inline bool operator!= (
  const ::mock::covellite::api::renderer::Renderer::Data::Color & _Left,
  const ::mock::covellite::api::renderer::Renderer::Data::Color & _Right)
{
  static_assert(sizeof(_Left) ==
    (sizeof(_Left.R) +
      sizeof(_Left.G) +
      sizeof(_Left.B) +
      sizeof(_Left.A)),
    "Unexpected Data::Color size.");

  if (_Left.R != _Right.R) return true;
  if (_Left.G != _Right.G) return true;
  if (_Left.B != _Right.B) return true;
  if (_Left.A != _Right.A) return true;
  return false;
}

inline bool operator== (
  const ::mock::covellite::api::renderer::Renderer::Data & _Left,
  const ::mock::covellite::api::renderer::Renderer::Data & _Right)
{
  static_assert(sizeof(_Left) ==
    (sizeof(_Left.Handle) +
      sizeof(_Left.Top) +
      sizeof(_Left.BkColor) +
      sizeof(_Left.IsFullScreen) + 3 // Выравнивание
      ),
    "Unexpected Data size.");

  if (_Left.Handle != _Right.Handle) return false;
  if (_Left.Top != _Right.Top) return false;
  if (_Left.BkColor != _Right.BkColor) return false;
  if (_Left.IsFullScreen != _Right.IsFullScreen) return false;
  return true;
}

} // namespace renderer

} // namespace api

} // namespace covellite

} // namespace mock
