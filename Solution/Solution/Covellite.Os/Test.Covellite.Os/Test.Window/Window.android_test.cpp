
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Window_test Window_android_test
#define Window Window_android
#define Configuration Configuration_android

// Расположение класса Window
#include "..\..\Covellite.Os\Window.android.cpp"
#include "..\..\Covellite.Os\Window.cpp"
#include "..\..\Covellite.Os\Configuration.android.cpp"
#include "..\..\Covellite.Os\Configuration.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::os::Window;
  using ITested_t = ::covellite::os::IWindow;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetHandle)
{
  ANativeWindow Window;
  auto * pWindow = &Window;

  const AppInfo_t Info{ &pWindow };

  const Tested_t Example;
  const ITested_t & IExample = Example;

  const auto Result = IExample.GetHandle();
  EXPECT_EQ(&Window, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetSystemLanguage)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeWindow Window;
  auto * pWindow = &Window;

  AConfiguration Config;
  auto * pConfig = &Config;

  const AppInfo_t Info{ &pWindow, &pConfig };

  const Tested_t Example;
  const ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  const ::std::map<::std::string, String_t> Languages =
  {
    { "en", uT("English") },
    { "en", uT("English") },
    { "ru", uT("Russian") },
    { "Unknown", uT("Default") },
  };

  for (const auto & Language : Languages)
  {
    EXPECT_CALL(AEventProxy, GetLanguage(&Config))
      .Times(1)
      .WillOnce(Return(Language.first));

    const auto Result = IExample.GetConfiguration().GetSystemLanguage();
    EXPECT_EQ(Language.second, Result);
  }
}
