
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Settings.
*/

// Расположение класса Settings
#include <Covellite\Core\Settings.hpp>

// Общий тестовый класс класса Settings
class Settings_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::Settings_t;
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

// Образец макроса для подстановки в класс Settings 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Settings_test, Test_Function);

/// \cond DoxygenWarningSuppress

/// [SetDefaultValues]
namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::core;

/*static*/ SectionPtr_t Singleton<Section_t>::Make(void)
{
  // Создаем объект настроек...
  auto pSettings = ::std::make_unique<Section_t>(uT("Part1802231330"));

  // Устанавливаем значения параметров по умолчанию...
  pSettings->SetDefault(uT("Name"), uT("Value"), uT("Description"));
    
  return pSettings;
}

} // namespace std

} // namespace extension

} // namespace alicorn

/// [SetDefaultValues]

/// \endcond

// ************************************************************************** //
TEST_F(Settings_test, /*DISABLED_*/Test_Using)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  const ::mock::Id_t RootSectionId = 1711081102;
  const ::mock::Id_t CovelliteppSectionId = 1711081103;
  const auto Name = uT("Name1711081105");
  const auto Value = uT("Value1711081106");
  
  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(RootSectionId));

  EXPECT_CALL(SettingsProxy,
    GetChildSectionImpl(RootSectionId, uT("Part1802231330")))
    .Times(1);

  EXPECT_CALL(SettingsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(CovelliteppSectionId));

  EXPECT_CALL(SettingsProxy, SetDefaultValue(CovelliteppSectionId, 
    uT("Name"), uT("Value"), uT("Description")))
    .Times(1);

  const auto & Example = Tested_t::GetInstance();

  EXPECT_CALL(SettingsProxy, GetValue(CovelliteppSectionId, Name))
    .Times(1)
    .WillOnce(Return(Value));

  EXPECT_EQ(Value, Example.Get<String_t>(Name));
}
