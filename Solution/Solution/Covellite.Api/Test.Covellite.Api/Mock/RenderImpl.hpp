
#pragma once
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>
#include <Covellite.Os/Covellite.Os/Predefined.hpp>
#include <Covellite.Api/Covellite.Api/Render/IGraphicApi.hpp>

namespace mock
{

using Data_t = ::covellite::api::render::IRender::Data;
template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const Data_t &>;

class RenderImpl :
  public Registator_t<::covellite::api::render::IGraphicApi>
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, ::mock::Id_t(const Data &));
    MOCK_METHOD2(SetClassName, void(::mock::Id_t, const String_t &));
    MOCK_METHOD1(ClearWindow, void(::mock::Id_t));
    MOCK_METHOD1(Present, void(::mock::Id_t));
    MOCK_METHOD3(ResizeWindow, void(::mock::Id_t, int32_t, int32_t));
    MOCK_METHOD1(GetUsingApi, String_t(::mock::Id_t));
    MOCK_METHOD2(CreateTexture, ITexture * (::mock::Id_t, const ITexture::Data &));
    MOCK_METHOD2(DestroyTexture, void(::mock::Id_t, ITexture *));
    MOCK_METHOD2(CreateGeometry, IGeometry * (::mock::Id_t, const IGeometry::Data &));
    MOCK_METHOD2(DestroyGeometry, void(::mock::Id_t, IGeometry *));
    MOCK_METHOD5(EnableScissorRegion, void(::mock::Id_t, int, int, int, int));
    MOCK_METHOD1(DisableScissorRegion, void(::mock::Id_t));
    MOCK_METHOD1(Render, void(::mock::Id_t));
  };

public:
  const ::mock::Id_t m_Id;

public:
  // Èםעונפויס IRender:
  void ClearWindow(void) override
  {
    Proxy::GetInstance()->ClearWindow(m_Id);
  }

  void Present(void) override
  {
    Proxy::GetInstance()->Present(m_Id);
  }

  void ResizeWindow(int32_t _X, int32_t _Y) override
  {
    Proxy::GetInstance()->ResizeWindow(m_Id, _X, _Y);
  }

public:
  String_t GetUsingApi(void) const override
  {
    return Proxy::GetInstance()->GetUsingApi(m_Id);
  }

public:
  ITexture * Create(const ITexture::Data & _Data) override 
  {
    return Proxy::GetInstance()->CreateTexture(m_Id, _Data);
  }

  void Destroy(ITexture * _pTexture) override 
  {
    Proxy::GetInstance()->DestroyTexture(m_Id, _pTexture);
  }

  IGeometry * Create(const IGeometry::Data & _Data) override
  {
    return Proxy::GetInstance()->CreateGeometry(m_Id, _Data);
  }

  void Destroy(IGeometry * _pGeometry) override 
  {
    Proxy::GetInstance()->DestroyGeometry(m_Id, _pGeometry);
  }

  void EnableScissorRegion(int _X, int _Y, int _W, int _H) override 
  {
    Proxy::GetInstance()->EnableScissorRegion(m_Id, _X, _Y, _W, _H);
  }

  void DisableScissorRegion(void) override 
  {
    Proxy::GetInstance()->DisableScissorRegion(m_Id);
  }

  void Render(void) override 
  {
    Proxy::GetInstance()->Render(m_Id);
  }

public:
  explicit RenderImpl(const Data & _Data) :
    m_Id(Proxy::GetInstance()->Constructor(_Data))
  {

  }
};

FACTORY_REGISTER_STRING_NAME(RenderImpl);

#define DEFINE_RENDER_IMPL(ClassName) \
  class ClassName : \
    public RenderImpl\
  {\
  public:\
    explicit ClassName(const Data & _Data) :\
      RenderImpl(_Data)\
    {\
      Proxy::GetInstance()->SetClassName(m_Id, uT(#ClassName));\
    }\
  };\
  \
  FACTORY_REGISTER_STRING_NAME(ClassName);

} // namespace mock
