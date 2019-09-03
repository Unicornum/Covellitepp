
#include "stdafx.h"
#include <chrono>

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

protected:
  class Test
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(CopyConstructor, void(void));
    };

  public:
    const int Value;

  public:
    explicit Test(const int _Value) : Value(_Value) {}
    Test(const Test & _Value) :
      Value(_Value.Value)
    {
      Proxy::GetInstance()->CopyConstructor();
    }
    Test & operator= (const Test &) = delete;
  };
};

// Образец макроса для подстановки в класс Component 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Component_test, Test_Function);

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Id_Unknown)
{
  const auto pExample = Tested_t::Make({
    {
      { uT("noid"), uT("1810251203") },
    } });

  EXPECT_EQ(uT("Unknown"), pExample->Type);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Id)
{
  const auto Id = uT("Id1810251204");

  const auto pExample = Tested_t::Make({
    {
      { uT("id"), Id },
    } });

  EXPECT_EQ(Id, pExample->Id);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Type_Unknown)
{
  const auto pExample = Tested_t::Make({
    {
      { uT("notype"), uT("1810251150") },
    } });

  EXPECT_EQ(uT("Unknown"), pExample->Type);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Type)
{
  const auto Type = uT("Type1810251145");

  const auto pExample = Tested_t::Make({
    {
      { uT("type"), Type },
    } });

  EXPECT_EQ(Type, pExample->Type);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Kind_Unknown)
{
  const auto pExample = Tested_t::Make({
    {
      { uT("nokind"), uT("1902131956") },
    } });

  EXPECT_EQ(uT("Unknown"), pExample->Kind);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_Kind)
{
  const auto Kind = uT("Type1902131957");

  const auto pExample = Tested_t::Make({
    {
      { uT("kind"), Kind },
    } });

  EXPECT_EQ(Kind, pExample->Kind);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_IsType)
{
  int * pPointer = (int *)1902230922;
  const int * pConstPointer = (int *)1902230923;

  const auto pExample = Tested_t::Make({
    {
      { uT("Param1"), uT("Value1") },
      { uT("Param2"), uT("true") },
      { uT("Param3"), true },
      { uT("Param4"), uT("12345") },
      { uT("Param5"), 12345 },
      { uT("Param6"), pPointer },
      { uT("Param7"), pConstPointer },
    } });

  EXPECT_EQ(false, pExample->IsType<String_t>(uT("Param0")));
  EXPECT_EQ(false, pExample->IsType<bool>(uT("Param0")));
  EXPECT_EQ(false, pExample->IsType<int>(uT("Param0")));
  EXPECT_EQ(true, pExample->IsType<String_t>(uT("Param1")));
  EXPECT_EQ(false, pExample->IsType<bool>(uT("Param1")));
  EXPECT_EQ(false, pExample->IsType<int>(uT("Param1")));
  EXPECT_EQ(true, pExample->IsType<String_t>(uT("Param2")));
  EXPECT_EQ(false, pExample->IsType<bool>(uT("Param2")));
  EXPECT_EQ(false, pExample->IsType<int>(uT("Param2")));
  EXPECT_EQ(false, pExample->IsType<String_t>(uT("Param3")));
  EXPECT_EQ(true, pExample->IsType<bool>(uT("Param3")));
  EXPECT_EQ(false, pExample->IsType<int>(uT("Param3")));
  EXPECT_EQ(true, pExample->IsType<String_t>(uT("Param4")));
  EXPECT_EQ(false, pExample->IsType<bool>(uT("Param4")));
  EXPECT_EQ(false, pExample->IsType<int>(uT("Param4")));
  EXPECT_EQ(false, pExample->IsType<String_t>(uT("Param5")));
  EXPECT_EQ(false, pExample->IsType<bool>(uT("Param5")));
  EXPECT_EQ(true, pExample->IsType<int>(uT("Param5")));
  EXPECT_EQ(true, pExample->IsType<int *>(uT("Param6")));
  EXPECT_EQ(false, pExample->IsType<const int *>(uT("Param6")));
  EXPECT_EQ(false, pExample->IsType<int *>(uT("Param7")));
  EXPECT_EQ(true, pExample->IsType<const int *>(uT("Param7")));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName)
{
  const auto pExample = Tested_t::Make({
    {
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

  EXPECT_EQ(0, pExample->GetValue(uT("Param0"), 0));
  EXPECT_EQ(uT("Unknown"), pExample->GetValue(uT("Param0"), uT("Unknown")));
  EXPECT_EQ(uT("Value1"), pExample->GetValue(uT("Param1"), uT("Unknown")));
  EXPECT_EQ(uT("Value2"), pExample->GetValue(uT("Param2"), uT("Unknown")));
  EXPECT_EQ(true, pExample->GetValue(uT("Param3"), false));
  EXPECT_EQ(false, pExample->GetValue(uT("Param4"), false));
  EXPECT_EQ(true, pExample->GetValue(uT("Param5"), true));
  EXPECT_EQ(false, pExample->GetValue(uT("Param6"), true));
  EXPECT_EQ(uT("12345"), pExample->GetValue(uT("Param7"), uT("0")));
  EXPECT_EQ(12345, pExample->GetValue(uT("Param7"), 0));
  EXPECT_EQ(12345.0f, pExample->GetValue(uT("Param7"), 0.0f));
  EXPECT_EQ(12345, pExample->GetValue(uT("Param8"), 0));
  EXPECT_EQ(123.45f, pExample->GetValue(uT("Param9"), 0.0f));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_Pointer)
{
  const uint8_t * pData = (const uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  EXPECT_EQ(pData, pExample->GetValue<const uint8_t *>(uT("pData"), nullptr));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_Reference)
{
  using Proxy_t = Test::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const auto Expected = 1907211959;

  const auto pExample = Tested_t::Make({
    {
      { uT("Data"), Test{ Expected } },
    } });

  using namespace ::testing;

  EXPECT_CALL(Proxy, CopyConstructor())
    .Times(0);

  const auto Result = 
    pExample->GetValue<const Test &>(uT("Data"), Test{ 0 }).Value;
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_InvalidType)
{
  const auto pExample = Tested_t::Make({
    {
      { uT("Param1"), uT("Value1") },
      { uT("Param2"), true },
      { uT("Param3"), 12345 },
      { uT("Param4"), 123.45f },
    } });

  EXPECT_THROW(pExample->GetValue(uT("Param1"), 0), ::std::exception);
  EXPECT_THROW(pExample->GetValue(uT("Param2"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(pExample->GetValue(uT("Param3"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(pExample->GetValue(uT("Param4"), 0), ::std::exception);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash)
{
  const auto pExample = Tested_t::Make({
    {
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

  auto GetValue = [&](const String_t & _Name, const auto & _DefaultValue) -> auto
  {
    return pExample->GetValue(Tested_t::GetHash(_Name), _DefaultValue);
  };

  EXPECT_EQ(0, GetValue(uT("Param0"), 0));
  EXPECT_EQ(uT("Unknown"), GetValue(uT("Param0"), uT("Unknown")));
  EXPECT_EQ(uT("Value1"), GetValue(uT("Param1"), uT("Unknown")));
  EXPECT_EQ(uT("Value2"), GetValue(uT("Param2"), uT("Unknown")));
  EXPECT_EQ(true, GetValue(uT("Param3"), false));
  EXPECT_EQ(false, GetValue(uT("Param4"), false));
  EXPECT_EQ(true, GetValue(uT("Param5"), true));
  EXPECT_EQ(false, GetValue(uT("Param6"), true));
  EXPECT_EQ(uT("12345"), GetValue(uT("Param7"), uT("0")));
  EXPECT_EQ(12345, GetValue(uT("Param7"), 0));
  EXPECT_EQ(12345.0f, GetValue(uT("Param7"), 0.0f));
  EXPECT_EQ(12345, GetValue(uT("Param8"), 0));
  EXPECT_EQ(123.45f, GetValue(uT("Param9"), 0.0f));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_Pointer)
{
  const uint8_t * pData = (const uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  EXPECT_EQ(pData, pExample->GetValue<const uint8_t *>(
    Tested_t::GetHash(uT("pData")), nullptr));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_Reference)
{
  using Proxy_t = Test::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const auto Expected = 1907212026;

  const auto pExample = Tested_t::Make({
    {
      { uT("Data"), Test{ Expected } },
    } });

  using namespace ::testing;

  EXPECT_CALL(Proxy, CopyConstructor())
    .Times(0);

  const auto Result = pExample->GetValue<const Test &>(
    Tested_t::GetHash(uT("Data")), Test{ 0 }).Value;
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_InvalidType)
{
  const auto pExample = Tested_t::Make({
    {
      { uT("Param1"), uT("Value1") },
      { uT("Param2"), true },
      { uT("Param3"), 12345 },
      { uT("Param4"), 123.45f },
    } });

  auto GetValue = [&](const String_t & _Name, const auto & _DefaultValue) -> auto
  {
    return pExample->GetValue(Tested_t::GetHash(_Name), _DefaultValue);
  };

  EXPECT_THROW(GetValue(uT("Param1"), 0), ::std::exception);
  EXPECT_THROW(GetValue(uT("Param2"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(GetValue(uT("Param3"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(GetValue(uT("Param4"), 0), ::std::exception);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_NotExistsParam)
{
  auto pExample = Tested_t::Make({
    {
      { uT("Unknown"), uT("0") },
    } });

  EXPECT_EQ(0, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(uT("Param"), 67890);
  EXPECT_EQ(67890, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(uT("Param"), true);
  EXPECT_EQ(true, pExample->GetValue(uT("Param"), false));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue)
{
  auto pExample = Tested_t::Make({
    {
      { uT("Param"), uT("12345") },
    } });

  EXPECT_EQ(12345, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(uT("Param"), 67890);
  EXPECT_EQ(67890, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(uT("Param"), true);
  EXPECT_EQ(true, pExample->GetValue(uT("Param"), false));

  const ::std::vector<int> Data = { 1907211952, 1907211953, 1907211954 };

  pExample->SetValue(uT("Param"), Data);
  EXPECT_EQ(Data, pExample->GetValue(uT("Param"), ::std::vector<int>{}));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_Hash)
{
  auto pExample = Tested_t::Make({
    {
      { uT("Param"), uT("12345") },
    } });

  EXPECT_EQ(12345, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(Tested_t::GetHash(uT("Param")), 67890);
  EXPECT_EQ(67890, pExample->GetValue(uT("Param"), 0));

  pExample->SetValue(Tested_t::GetHash(uT("Param")), true);
  EXPECT_EQ(true, pExample->GetValue(uT("Param"), false));

  const ::std::vector<int> Data = { 1907211955, 1907211956, 1907211957 };

  pExample->SetValue(Tested_t::GetHash(uT("Param")), Data);
  EXPECT_EQ(Data, pExample->GetValue(uT("Param"), ::std::vector<int>{}));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Speed)
{
  const auto Run = [](const Tested_t::ComponentPtr_t & _pComponent)
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
        Result += _pComponent->GetValue<const float &>(hName, 0.0f);
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto Result = Run(Tested_t::Make({
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
  } }));
  EXPECT_TRUE(Result > 35000u) << "Result: " << Result;
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_Speed)
{
  const auto Run = [](const Tested_t::ComponentPtr_t & _pComponent)
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
        _pComponent->SetValue(hName, Result);
      }

      Count += 100;
    }

    EXPECT_NE(0.0f, Result);

    return Count;
  };

  const auto Result = Run(Tested_t::Make({
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
  } }));
  EXPECT_TRUE(Result > 25000u) << "Result: " << Result;
}
