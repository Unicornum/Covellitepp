
#pragma once
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Api\IWindow.hpp>

namespace mock
{

class ApiImpl :
  public ::covellite::core::IWindow,
  public ::covellite::api::IWindow
{
  using WindowOsPtr_t = ::std::shared_ptr<::covellite::os::IWindow>;
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<::mock::Rocket::Core::RenderInterface>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, ::mock::Id_t(::mock::Id_t));
    MOCK_METHOD2(Subscribe, void(::mock::Id_t, ::mock::Id_t));
    MOCK_METHOD1(GetUsingApi, String_t (::mock::Id_t));
    MOCK_METHOD1(GetWidth, int32_t(::mock::Id_t));
    MOCK_METHOD1(GetHeight, int32_t(::mock::Id_t));
    MOCK_METHOD1(MakeRenderInterface, RenderInterfacePtr_t(::mock::Id_t));
  };

public:
  ::mock::Id_t m_Id;

public:
  // Èםעונפויס core::IWindow:
  void Subscribe(const EventHandlerPtr_t & _Events) override
  {
    Proxy::GetInstance()->Subscribe(m_Id, _Events->m_Id);
  }

public:
  // Èםעונפויס api::IWindow:
  String_t GetUsingApi(void) const override
  {
    return Proxy::GetInstance()->GetUsingApi(m_Id);
  }

  int32_t GetWidth(void) const override
  {
    return Proxy::GetInstance()->GetWidth(m_Id);
  }

  int32_t GetHeight(void) const override
  {
    return Proxy::GetInstance()->GetHeight(m_Id);
  }

  RenderInterfacePtr_t MakeRenderInterface(void) const override
  {
    return Proxy::GetInstance()->MakeRenderInterface(m_Id);
  }

public:
  explicit ApiImpl(const WindowOsPtr_t & _pWindow) :
    m_Id(Proxy::GetInstance()->Constructor(
      dynamic_cast<::mock::covellite::os::Window &>(*_pWindow).m_Id))
  {

  }
};

} // namespace mock
