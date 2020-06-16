
#include "stdafx.h"
#include <chrono>

#ifndef NDEBUG
# error "This project for Release configuration only!"
#endif

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Component.
*/

// Расположение класса Component
#include <Covellite/Api/Component.inl>

// Общий тестовый класс класса Component
class Component_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Component;
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

// Образец макроса для подстановки в класс Component 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Component_test, Test_Function);

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Speed)
{
  const auto Run1 = [](const Tested_t & _Component)
  {
    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    float Result = 0;
    size_t Count = 0;
    const auto hName = Tested_t::GetHash(uT("Data"));

    while ((system_clock::now() - Begin) < milliseconds(1000))
    {
      for (int i = 0; i < 100; i++)
      {
        Result += _Component.GetValue<const float &>(hName, 0.0f);
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto Run2 = [](const Tested_t & _Component)
  {
    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    float Result = 0;
    size_t Count = 0;
    const auto hName = Tested_t::GetHash(uT("Data"));

    while ((system_clock::now() - Begin) < milliseconds(1000))
    {
      for (int i = 0; i < 100; i++)
      {
        Result += (const float &)_Component[hName];
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto Run3 = [](Tested_t & _Component)
  {
    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    float Result = 0;
    size_t Count = 0;
    const auto hName = Tested_t::GetHash(uT("Data"));

    while ((system_clock::now() - Begin) < milliseconds(1000))
    {
      for (int i = 0; i < 100; i++)
      {
        Result += (const float &)_Component[hName].Default(0.0f);
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto pExample = Tested_t::Make({
  {
    { uT("Data"), 1.0f },
    { uT("Param1"), uT("Value1") },
    { uT("Param2"), uT("Value2") },
    { uT("Param3"), uT("true") },
    { uT("Param4"), uT("false") },
    { uT("Param5"), true },
    { uT("Param6"), false },
    { uT("Param7"), uT("12345") },
    { uT("Param8"), 12345 },
    { uT("Param9"), 123.45f },
  } });

  const auto Result1 = Run1(*pExample);
  const auto Result2 = Run2(*pExample);
  const auto Result3 = Run3(*pExample);
  EXPECT_EQ(Result2, Result1);
  EXPECT_EQ(Result3, Result2);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_Speed)
{
  const auto Run1 = [](Tested_t & _Component)
  {
    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    float Result = 0;
    size_t Count = 0;
    const auto hName = Tested_t::GetHash(uT("Data"));

    while ((system_clock::now() - Begin) < milliseconds(100))
    {
      for (int i = 0; i < 100; i++)
      {
        Result += 1.0f;
        _Component.SetValue(hName, Result);
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto Run2 = [](Tested_t & _Component)
  {
    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    float Result = 0;
    size_t Count = 0;
    const auto hName = Tested_t::GetHash(uT("Data"));

    while ((system_clock::now() - Begin) < milliseconds(100))
    {
      for (int i = 0; i < 100; i++)
      {
        Result += 1.0f;
        _Component[hName] = Result;
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto pExample = Tested_t::Make({
  {
    { uT("Data"), 1.0f },
    { uT("Param1"), uT("Value1") },
    { uT("Param2"), uT("Value2") },
    { uT("Param3"), uT("true") },
    { uT("Param4"), uT("false") },
    { uT("Param5"), true },
    { uT("Param6"), false },
    { uT("Param7"), uT("12345") },
    { uT("Param8"), 12345 },
    { uT("Param9"), 123.45f },
  } });

  const auto Result1 = Run1(*pExample);
  const auto Result2 = Run2(*pExample);
  EXPECT_EQ(Result2, Result1);
}
