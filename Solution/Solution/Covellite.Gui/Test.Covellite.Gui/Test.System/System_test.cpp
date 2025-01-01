
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса System.
*/

// Расположение класса System
#include "..\..\Covellite.Gui\System.windows.cpp"
#include "..\..\Covellite.Gui\System.cpp"

#include <Covellite\Gui\IStringTranslator.hpp>

// Общий тестовый класс класса System
class System_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::System;
  using ITested_t = ::mock::CovelliteGui::SystemInterface;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  class StringTranslator :
    public ::covellite::gui::IStringTranslator
  {
  public:
    MOCK_METHOD1(Translate, Utf8String_t (const Utf8String_t &));
  };
};

// Образец макроса для подстановки в класс System 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(System_test, Test_Function);

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_Constructor_Exception)
{
  EXPECT_STDEXCEPTION(Tested_t{nullptr}, ".+String translator null pointer.");
}

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_TranslateString)
{
  const char * Source = "Source1612212335";
  const char * Translate = "Translate1612212336";

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(*pStringTranslator, Translate(Eq(Source)))
    .Times(1)
    .WillOnce(Return(Translate));

  ::mock::CovelliteGui::String Result;
  const auto Return = IExample.TranslateString(Result, Source);
  EXPECT_EQ(0, Return);
  EXPECT_EQ(Result, Translate);
}

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_LogMessage)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using Log_t = ::mock::CovelliteGui::Log;

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Trace: Text1"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_ALWAYS, "Text1");
    EXPECT_TRUE(Result);
  }

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Trace: Text2"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_ASSERT, "Text2");
    EXPECT_TRUE(Result);
  }

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Trace_debug: Text3"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_DEBUG, "Text3");
    EXPECT_TRUE(Result);
  }

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Error: Text4"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_ERROR, "Text4");
    EXPECT_TRUE(Result);
  }

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Info: Text5"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_INFO, "Text5");
    EXPECT_TRUE(Result);
  }

  {
    EXPECT_CALL(LoggerProxy, ToLog(Eq(uT("Warning: Text6"))))
      .Times(1);

    const auto Result = IExample.LogMessage(Log_t::LT_WARNING, "Text6");
    EXPECT_TRUE(Result);
  }
}
