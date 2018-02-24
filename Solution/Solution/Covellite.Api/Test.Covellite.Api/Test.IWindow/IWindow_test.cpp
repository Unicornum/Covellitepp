
#include "stdafx.h"
#include <alicorn\std\string.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IWindow.
*/

// ������������ ������ IWindow
#include <Covellite\Api\IWindow.hpp>

// ����� �������� ����� ������ IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::IWindow;

  // ���������� ����� �������� ������� �����
  // cppcheck-suppress unusedFunction
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  // cppcheck-suppress unusedFunction
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
    String_t GetUsingApi(void) const override { throw ::std::exception{}; }
    int32_t GetWidth(void) const override { return 0; }
    int32_t GetHeight(void) const override { return 0; }
    RenderInterfacePtr_t MakeRenderInterface(void) const override { throw ::std::exception{}; }

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
