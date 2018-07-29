
#pragma once
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/Core/IWindow.hpp>
#include <Covellite/Api/IWindow.hpp>

namespace mock
{

using WindowOs_t = ::covellite::os::IWindow;
template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const WindowOs_t &>;

class ApiImpl :
  public Registator_t<::covellite::api::IWindow>,
  public ::covellite::core::IWindow
{
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<::mock::Rocket::Core::RenderInterface>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, ::mock::Id_t(::mock::Id_t));
    MOCK_METHOD2(Subscribe, void(::mock::Id_t, ::mock::Id_t));
    MOCK_METHOD1(GetEvents, Events_t(::mock::Id_t));
    MOCK_METHOD1(GetUsingApi, String_t (::mock::Id_t));
    MOCK_METHOD1(GetWidth, int32_t(::mock::Id_t));
    MOCK_METHOD1(GetHeight, int32_t(::mock::Id_t));
    MOCK_METHOD1(GetClientRect, Rect(::mock::Id_t));
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
  operator Events_t (void) const override
  {
    return Proxy::GetInstance()->GetEvents(m_Id);
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

  Rect GetClientRect(void) const override
  {
    return Proxy::GetInstance()->GetClientRect(m_Id);
  }

  RenderInterfacePtr_t MakeRenderInterface(void) const override
  {
    return Proxy::GetInstance()->MakeRenderInterface(m_Id);
  }

public:
  explicit ApiImpl(const WindowOs_t & _Window) :
    m_Id(Proxy::GetInstance()->Constructor(
      dynamic_cast<const ::mock::covellite::os::Window &>(_Window).m_Id))
  {

  }
};

FACTORY_REGISTER_STRING_NAME(ApiImpl);
namespace { FACTORY_REGISTER(ApiImpl, uT("DefaultImpl")); }

} // namespace mock
