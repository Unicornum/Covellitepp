
#pragma once
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/IGraphicApi.hpp>

namespace mock
{

using Data_t = ::covellite::api::renderer::SettingsData;
template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const Data_t &>;

class RendererImpl :
  public Registator_t<::covellite::api::renderer::IGraphicApi>
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, ::mock::Id_t(const Data_t &));
    MOCK_METHOD2(SetClassName, void(::mock::Id_t, const String_t &));
    MOCK_METHOD1(GetUsingApi, String_t(::mock::Id_t));
    MOCK_METHOD1(ClearFrame, void(::mock::Id_t));
    MOCK_METHOD1(PresentFrame, void(::mock::Id_t));
    MOCK_METHOD3(ResizeWindow, void(::mock::Id_t, int32_t, int32_t));
    MOCK_METHOD1(GetCreators, const Creators_t &(::mock::Id_t));
  };

public:
  const ::mock::Id_t m_Id;

public:
  String_t GetUsingApi(void) const override
  {
    return Proxy::GetInstance()->GetUsingApi(m_Id);
  }

  void PresentFrame(void) override
  {
    Proxy::GetInstance()->PresentFrame(m_Id);
  }

  void ResizeWindow(const ::covellite::Rect & _ClientRect) override
  {
    Proxy::GetInstance()->ResizeWindow(m_Id, _ClientRect.Width, _ClientRect.Height);
  }

  const Creators_t & GetCreators(void) const override
  {
    return Proxy::GetInstance()->GetCreators(m_Id);
  }

public:
  explicit RendererImpl(const Data_t & _Data) :
    m_Id(Proxy::GetInstance()->Constructor(_Data))
  {

  }
};

FACTORY_REGISTER_STRING_NAME(RendererImpl);

#define DEFINE_RENDER_IMPL(ClassName) \
  class ClassName : \
    public RendererImpl\
  {\
  public:\
    explicit ClassName(const Data_t & _Data) :\
      RendererImpl(_Data)\
    {\
      Proxy::GetInstance()->SetClassName(m_Id, uT(#ClassName));\
    }\
  };\
  \
  FACTORY_REGISTER_STRING_NAME(ClassName);

} // namespace mock
