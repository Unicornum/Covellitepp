
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IWindow.
*/

#include <Covellite\Core.mock.hpp>

// ������������ ������ IWindow
#include <Covellite\Core\IWindow.hpp>

// ����� �������� ����� ������ IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::IWindow;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IWindow 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IWindow_test, Test_Function);

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_VirtualDestructor)
{
  class Tested :
    public Tested_t
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Destructor, void(void));
    };

  public:
    void Subscribe(const EventHandlerPtr_t &) override {}

  public:
    ~Tested(void) noexcept
    {
      Proxy::GetInstance()->Destructor();
    }
  };

  // ������ �� �������� ::std::make_shared<>()???
  Tested_t * pExample = new Tested;

  using Proxy_t = Tested::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor())
    .Times(1);

  delete pExample;
}
