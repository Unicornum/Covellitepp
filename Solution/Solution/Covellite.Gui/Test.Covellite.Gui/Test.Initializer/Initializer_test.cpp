
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Initializer.
*/

// ������������ ������ Initializer
#include <Covellite\Gui\Initializer.hpp>

// ����� �������� ����� ������ Initializer
class Initializer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Initializer;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Initializer 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Initializer_test, Test_Function);

// ************************************************************************** //
TEST_F(Initializer_test, /*DISABLED_*/Test_Constructor_Destructor)
{
  using Proxy_t = ::mock::Rocket::Core::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using InterfacesProxy_t = ::mock::covellite::InterfacesProxy;
  InterfacesProxy_t InterfacesProxy;
  InterfacesProxy_t::GetInstance() = &InterfacesProxy;

  using StringTranslatorProxy_t = 
    ::mock::covellite::gui::StringTranslator::Proxy;
  StringTranslatorProxy_t StringTranslatorProxy;
  StringTranslatorProxy_t::GetInstance() = &StringTranslatorProxy;

  const ::mock::Id_t IFileId = 1612192232;
  const ::mock::Id_t ISystemId = 1612192233;
  const ::mock::Id_t IRenderId = 1612192234;
  const ::mock::Id_t StringTranslatorId = 1710081323;

  using namespace ::testing;

  EXPECT_CALL(InterfacesProxy, RenderConstructor(_))
    .Times(1)
    .WillOnce(Return(IRenderId));

  auto RenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(1)
    .WillOnce(Return(StringTranslatorId));

  auto StringTranslator =
    ::std::make_shared<::mock::covellite::gui::StringTranslator>();

  const Tested_t::Data Data =
  {
    RenderInterface,
    StringTranslator
  };

  for (size_t i = 0; i < 5; i++)
  {
    InSequence Dummy;

    EXPECT_CALL(InterfacesProxy, FileConstructor())
      .Times(1)
      .WillOnce(Return(IFileId));

    EXPECT_CALL(InterfacesProxy, SystemConstructor(StringTranslatorId))
      .Times(1)
      .WillOnce(Return(ISystemId));

    EXPECT_CALL(Proxy, SetFileInterface(IFileId))
      .Times(1);

    EXPECT_CALL(Proxy, SetSystemInterface(ISystemId))
      .Times(1);

    EXPECT_CALL(Proxy, SetRenderInterface(IRenderId))
      .Times(1);

    EXPECT_CALL(Proxy, Initialise())
      .Times((i == 0) ? 1 : 0);

    {
      Tested_t Example(Data);

      EXPECT_CALL(Proxy, SetFileInterface(0))
        .Times(1);

      EXPECT_CALL(Proxy, SetSystemInterface(0))
        .Times(1);

      EXPECT_CALL(Proxy, SetRenderInterface(0))
        .Times(1);
    }
  }
}

// ************************************************************************** //
TEST_F(Initializer_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}