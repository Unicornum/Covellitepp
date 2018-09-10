
#pragma once
#include <Covellite/Os/Predefined.hpp>
#include "../../Covellite.Api/Render/IRender.hpp"

namespace mock
{

namespace covellite
{

namespace api
{

namespace render
{

class Render :
  public ::mock::Rocket::Core::RenderInterface,
  public ::covellite::api::render::IRender
{
  using String_t = ::alicorn::extension::std::String;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, ::mock::Id_t(const String_t &, const Data &));
    MOCK_METHOD1(ClearWindow, void(::mock::Id_t));
    MOCK_METHOD1(Present, void(::mock::Id_t));
    MOCK_METHOD3(ResizeWindow, void(::mock::Id_t, int32_t, int32_t));
  };

public:
  void ClearWindow(void) override
  {
    Proxy::GetInstance()->ClearWindow(m_Id);
  }

  void Present(void) override
  {
    Proxy::GetInstance()->Present(m_Id);
  }

  void ResizeWindow(int32_t _Width, int32_t _Height) override
  {
    Proxy::GetInstance()->ResizeWindow(m_Id, _Width, _Height);
  }

public:
  Render(const String_t & _ApiName, const Data & _Data) :
    RenderInterface(Proxy::GetInstance()->Constructor(_ApiName, _Data))
  {

  }
};

} // namespace render

} // namespace api

} // namespace covellite

} // namespace mock
