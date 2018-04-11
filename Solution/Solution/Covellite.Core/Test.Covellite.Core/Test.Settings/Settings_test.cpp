
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Settings.
*/

// ������������ ������ Settings
#include <Covellite\Core\Settings.hpp>

// ����� �������� ����� ������ Settings
class Settings_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::Settings_t;
  using String_t = ::alicorn::extension::std::String;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Settings 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
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
  // ������� ������ ��������...
  auto pSettings = ::std::make_unique<Section_t>(uT("Part1802231330"));

  // ������������� �������� ���������� �� ���������...
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
