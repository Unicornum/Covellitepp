
#include "stdafx.h"

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
TEST_F(Component_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue)
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
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_Pointer)
{
  const uint8_t * pData = (const uint8_t *)1811061740;

  const auto pExample = Tested_t::Make({
    {
      { uT("pData"), pData },
    } });

  EXPECT_EQ(pData, pExample->GetValue<const uint8_t *>(uT("pData"), nullptr));
}

// ************************************************************************** //
TEST_F(Component_test, /*DISABLED_*/Test_GetValue_InvalidType)
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
