
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
    int Value;

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

  EXPECT_EQ(false, (*pExample)[uT("Param0")].IsType<String_t>());
  EXPECT_EQ(false, (*pExample)[uT("Param0")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param0")].IsType<int>());
  EXPECT_EQ(true,  (*pExample)[uT("Param1")].IsType<String_t>());
  EXPECT_EQ(false, (*pExample)[uT("Param1")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param1")].IsType<int>());
  EXPECT_EQ(true,  (*pExample)[uT("Param2")].IsType<String_t>());
  EXPECT_EQ(false, (*pExample)[uT("Param2")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param2")].IsType<int>());
  EXPECT_EQ(false, (*pExample)[uT("Param3")].IsType<String_t>());
  EXPECT_EQ(true,  (*pExample)[uT("Param3")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param3")].IsType<int>());
  EXPECT_EQ(true,  (*pExample)[uT("Param4")].IsType<String_t>());
  EXPECT_EQ(false, (*pExample)[uT("Param4")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param4")].IsType<int>());
  EXPECT_EQ(false, (*pExample)[uT("Param5")].IsType<String_t>());
  EXPECT_EQ(false, (*pExample)[uT("Param5")].IsType<bool>());
  EXPECT_EQ(true,  (*pExample)[uT("Param5")].IsType<int>());
  EXPECT_EQ(true,  (*pExample)[uT("Param6")].IsType<int *>());
  EXPECT_EQ(false, (*pExample)[uT("Param6")].IsType<const int *>());
  EXPECT_EQ(false, (*pExample)[uT("Param7")].IsType<int *>());
  EXPECT_EQ(true,  (*pExample)[uT("Param7")].IsType<const int *>());
}

template<class T, class TResult>
void COMPONENT_EXPECT_EQ(const T & _Expected, const TResult & _Result)
{
  const T Result = _Result;
  EXPECT_EQ(_Expected, Result);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Const_StringName)
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
      { uT("Param8"), uT("12345") },
      { uT("Param9"), 12345 },
      { uT("Param10"), 123.45f },
    } });

  const Tested_t & Example = (*pExample);

  EXPECT_THROW(const int Result1 = Example[uT("Unknown")], ::std::exception);
  EXPECT_THROW(const int Result2 = Example[uT("Unknown")], ::std::exception);
  COMPONENT_EXPECT_EQ(uT("Value1"), Example[uT("Param1")]);
  COMPONENT_EXPECT_EQ(uT("Value2"), Example[uT("Param2")]);
  EXPECT_THROW(const bool Result3 = Example[uT("Param3")], ::std::exception);
  EXPECT_THROW(const bool Result4 = Example[uT("Param4")], ::std::exception);
  COMPONENT_EXPECT_EQ(true, Example[uT("Param5")]);
  COMPONENT_EXPECT_EQ(false, Example[uT("Param6")]);
  COMPONENT_EXPECT_EQ(uT("12345"), Example[uT("Param7")]);
  EXPECT_THROW(const int Result7 = Example[uT("Param7")], ::std::exception);
  COMPONENT_EXPECT_EQ(uT("12345"), Example[uT("Param8")]);
  EXPECT_THROW(const float Result8 = Example[uT("Param8")], ::std::exception);
  COMPONENT_EXPECT_EQ(12345, Example[uT("Param9")]);
  COMPONENT_EXPECT_EQ(123.45f, Example[uT("Param10")]);
}

template<class T, class TResult>
void COMPONENT_EXPECT_EQ(const T & _Expected, TResult & _Result)
{
  const T Result = _Result;
  EXPECT_EQ(_Expected, Result);
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
      { uT("Param8"), uT("12345") },
      { uT("Param9"), 12345 },
      { uT("Param10"), 123.45f },
    } });

  COMPONENT_EXPECT_EQ(0, (*pExample)[uT("Unknown0")].Default(0));
  COMPONENT_EXPECT_EQ(uT("Unknown"), (*pExample)[uT("Unknown1")].Default(uT("Unknown")));
  COMPONENT_EXPECT_EQ(uT("Value1"), (*pExample)[uT("Param1")].Default(uT("Unknown")));
  COMPONENT_EXPECT_EQ(uT("Value2"), (*pExample)[uT("Param2")]);
  COMPONENT_EXPECT_EQ(true, (*pExample)[uT("Param3")]);
  COMPONENT_EXPECT_EQ(false, (*pExample)[uT("Param4")]);
  COMPONENT_EXPECT_EQ(true, (*pExample)[uT("Param5")]);
  COMPONENT_EXPECT_EQ(false, (*pExample)[uT("Param6")]);
  COMPONENT_EXPECT_EQ(uT("12345"), (*pExample)[uT("Param7")]);
  COMPONENT_EXPECT_EQ(12345, (*pExample)[uT("Param7")]);
  COMPONENT_EXPECT_EQ(uT("12345"), (*pExample)[uT("Param8")]);
  COMPONENT_EXPECT_EQ(12345.0f, (*pExample)[uT("Param8")]);
  COMPONENT_EXPECT_EQ(12345, (*pExample)[uT("Param9")]);
  COMPONENT_EXPECT_EQ(123.45f, (*pExample)[uT("Param10")]);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_Pointer)
{
  const auto * pData = (uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  void * pUnknown = (*pExample)[uT("pUnknown")].Default((void *)nullptr);
  EXPECT_EQ(nullptr, pUnknown);

  const uint8_t * pResult = (*pExample)[uT("pData")];
  EXPECT_EQ(pData, pResult);
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

  Test & Result = (*pExample)[uT("Data")];
  EXPECT_EQ(Expected, Result.Value);

  const auto Expected2 = 2006081153;

  Result.Value = Expected2;
  const Test & Result2 = (*pExample)[uT("Data")];
  EXPECT_EQ(Expected2, Result2.Value);
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

  EXPECT_THROW(const int Value1 = (*pExample)[uT("Param1")], ::std::exception);
  EXPECT_THROW(const String_t Value2 = (*pExample)[uT("Param2")], ::std::exception);
  EXPECT_THROW(const String_t Value3 = (*pExample)[uT("Param3")], ::std::exception);
  EXPECT_THROW(const int Value4 = (*pExample)[uT("Param4")], ::std::exception);
}

template<class T>
T GetValue(
  const ::covellite::api::Component & _Example, 
  const ::alicorn::extension::std::String & _Name, 
  const T & /*_DefaultValue*/)
{
  return _Example[::covellite::api::Component::GetHash(_Name)];
};

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Const_Hash)
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
      { uT("Param8"), uT("12345") },
      { uT("Param9"), 12345 },
      { uT("Param10"), 123.45f },
    } });

  EXPECT_THROW(GetValue(*pExample, uT("Unknown0"), 0), ::std::exception);
  EXPECT_THROW(GetValue(*pExample, uT("Unknown1"), uT("Unknown")), ::std::exception);
  EXPECT_EQ(uT("Value1"), GetValue(*pExample, uT("Param1"), uT("Unknown")));
  EXPECT_EQ(uT("Value2"), GetValue(*pExample, uT("Param2"), uT("Unknown")));
  EXPECT_THROW(GetValue(*pExample, uT("Param3"), false), ::std::exception);
  EXPECT_THROW(GetValue(*pExample, uT("Param4"), false), ::std::exception);
  EXPECT_EQ(true, GetValue(*pExample, uT("Param5"), true));
  EXPECT_EQ(false, GetValue(*pExample, uT("Param6"), true));
  EXPECT_EQ(uT("12345"), GetValue(*pExample, uT("Param7"), uT("0")));
  EXPECT_THROW(GetValue(*pExample, uT("Param7"), 0), ::std::exception);
  EXPECT_EQ(uT("12345"), GetValue(*pExample, uT("Param8"), uT("0")));
  EXPECT_THROW(GetValue(*pExample, uT("Param8"), 0.0f), ::std::exception);
  EXPECT_EQ(12345, GetValue(*pExample, uT("Param9"), 0));
  EXPECT_EQ(123.45f, GetValue(*pExample, uT("Param10"), 0.0f));
}

template<class T>
T GetValue(
  ::covellite::api::Component & _Example,
  const ::alicorn::extension::std::String & _Name,
  const T & _DefaultValue)
{
  return _Example[::covellite::api::Component::GetHash(_Name)].Default(_DefaultValue);
};

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
      { uT("Param8"), uT("12345") },
      { uT("Param9"), 12345 },
      { uT("Param10"), 123.45f },
    } });

  EXPECT_EQ(0, GetValue(*pExample, uT("Unknown0"), 0));
  EXPECT_EQ(uT("Unknown"), GetValue(*pExample, uT("Unknown1"), uT("Unknown")));
  EXPECT_EQ(uT("Value1"), GetValue(*pExample, uT("Param1"), uT("Unknown")));
  EXPECT_EQ(uT("Value2"), GetValue(*pExample, uT("Param2"), uT("Unknown")));
  EXPECT_EQ(true, GetValue(*pExample, uT("Param3"), false));
  EXPECT_EQ(false, GetValue(*pExample, uT("Param4"), false));
  EXPECT_EQ(true, GetValue(*pExample, uT("Param5"), true));
  EXPECT_EQ(false, GetValue(*pExample, uT("Param6"), true));
  EXPECT_EQ(uT("12345"), GetValue(*pExample, uT("Param7"), uT("0")));
  EXPECT_EQ(12345, GetValue(*pExample, uT("Param7"), 0));
  EXPECT_EQ(uT("12345"), GetValue(*pExample, uT("Param8"), uT("0")));
  EXPECT_EQ(12345.0f, GetValue(*pExample, uT("Param8"), 0.0f));
  EXPECT_EQ(12345, GetValue(*pExample, uT("Param9"), 0));
  EXPECT_EQ(123.45f, GetValue(*pExample, uT("Param10"), 0.0f));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_Pointer)
{
  const uint8_t * pData = (const uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  void * pUnknown = 
    (*pExample)[Tested_t::GetHash(uT("pUnknown"))].Default((void *)nullptr);
  EXPECT_EQ(nullptr, pUnknown);

  const uint8_t *  pResult = 
    (*pExample)[Tested_t::GetHash(uT("pData"))];
  EXPECT_EQ(pData, pResult);
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

  Test & Result = (*pExample)[Tested_t::GetHash(uT("Data"))];
  EXPECT_EQ(Expected, Result.Value);

  const auto Expected2 = 2006081217;
  Result.Value = Expected2;

  const Test & Result2 = (*pExample)[Tested_t::GetHash(uT("Data"))];
  EXPECT_EQ(Expected2, Result2.Value);
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

  EXPECT_THROW(GetValue(*pExample, uT("Param1"), 0), ::std::exception);
  EXPECT_THROW(GetValue(*pExample, uT("Param2"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(GetValue(*pExample, uT("Param3"), uT("Unknown")), ::std::exception);
  EXPECT_THROW(GetValue(*pExample, uT("Param4"), 0), ::std::exception);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_NotExistsParam)
{
  auto pExample = Tested_t::Make({
    {
      { uT("Unknown"), uT("0") },
    } });

  int Result = (*pExample)[uT("Param")].Default(0);
  EXPECT_EQ(0, Result);

  (*pExample)[uT("Param")] = 67890;
  Result = (*pExample)[uT("Param")];
  EXPECT_EQ(67890, Result);

  (*pExample)[uT("Param")] = true;
  bool Result2 = (*pExample)[uT("Param")];
  EXPECT_EQ(true, Result2);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue)
{
  auto pExample = Tested_t::Make({
    {
      { uT("Param"), uT("12345") },
    } });

  const int Result1 = (*pExample)[uT("Param")];
  EXPECT_EQ(12345, Result1);

  (*pExample)[uT("Param")] = 67890;
  const int Result2 = (*pExample)[uT("Param")];
  EXPECT_EQ(67890, Result2);

  (*pExample)[uT("Param")] = true;
  const bool Result3 = (*pExample)[uT("Param")];
  EXPECT_EQ(true, Result3);

  const ::std::vector<int> Data = { 1907211952, 1907211953, 1907211954 };

  (*pExample)[uT("Param")] = Data;
  const ::std::vector<int> Result4 = (*pExample)[uT("Param")];
  EXPECT_EQ(Data, Result4);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_Hash)
{
  auto pExample = Tested_t::Make({
  {
    { uT("Param"), uT("12345") },
  } });

  const int Result1 = (*pExample)[Tested_t::GetHash(uT("Param"))];
  EXPECT_EQ(12345, Result1);

  (*pExample)[Tested_t::GetHash(uT("Param"))] = 67890;
  const int Result2 = (*pExample)[Tested_t::GetHash(uT("Param"))];
  EXPECT_EQ(67890, Result2);

  (*pExample)[Tested_t::GetHash(uT("Param"))] = true;
  const bool Result3 = (*pExample)[Tested_t::GetHash(uT("Param"))];
  EXPECT_EQ(true, Result3);

  const ::std::vector<int> Data = { 1907211952, 1907211953, 1907211954 };

  (*pExample)[Tested_t::GetHash(uT("Param"))] = Data;
  const ::std::vector<int> Result4 = (*pExample)[Tested_t::GetHash(uT("Param"))];
  EXPECT_EQ(Data, Result4);
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_ChangeType)
{
  auto pExample = Tested_t::Make({
  {
    { uT("Param"), uT("12345") },
  } });

  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<int>());
  EXPECT_EQ(true, (*pExample)[uT("Param")].IsType<String_t>());

  const int Result = (*pExample)[uT("Param")];
  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<bool>());
  EXPECT_EQ(true, (*pExample)[uT("Param")].IsType<int>());
  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<String_t>());

  (*pExample)[uT("Param")] = true;
  EXPECT_EQ(true, (*pExample)[uT("Param")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<int>());
  EXPECT_EQ(false, (*pExample)[uT("Param")].IsType<String_t>());

  const String_t Result2 = (*pExample)[uT("Unknown")].Default(uT("0"));
  EXPECT_EQ(false, (*pExample)[uT("Unknown")].IsType<bool>());
  EXPECT_EQ(false, (*pExample)[uT("Unknown")].IsType<int>());
  EXPECT_EQ(true, (*pExample)[uT("Unknown")].IsType<String_t>());

  (*pExample)[uT("Unknown")] = 2006081307;
  EXPECT_EQ(false, (*pExample)[uT("Unknown")].IsType<bool>());
  EXPECT_EQ(true, (*pExample)[uT("Unknown")].IsType<int>());
  EXPECT_EQ(false, (*pExample)[uT("Unknown")].IsType<String_t>());
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_IsType_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_Pointer_deprecated)
{
  const uint8_t * pData = (const uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  EXPECT_EQ(pData, pExample->GetValue<const uint8_t *>(uT("pData"), nullptr));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_Reference_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_StringName_InvalidType_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_Pointer_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_Reference_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Hash_InvalidType_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_NotExistsParam_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_deprecated)
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
TEST_F(Component_test, /*DISABLED_*/Test_SetValue_Hash_deprecated)
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
