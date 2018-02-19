
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IStringTranslator.
*/

// ������������ ������ IStringTranslator
#include <Covellite\Rocket\IStringTranslator.hpp>

// ����� �������� ����� ������ IStringTranslator
class IStringTranslator_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::IStringTranslator;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IStringTranslator 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IStringTranslator_test, Test_Function);

// ************************************************************************** //
TEST_F(IStringTranslator_test, /*DISABLED_*/Test_VirtualDestructor)
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
    Utf8String_t Translate(const Utf8String_t &) override { throw ::std::exception{}; }

  public:
    ~Tested(void)
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
