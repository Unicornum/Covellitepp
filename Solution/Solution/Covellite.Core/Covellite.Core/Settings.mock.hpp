
#pragma once
#include <alicorn\settings.mock.hpp>
#include <Covellite\Core\Settings.hpp>

using ::covellite::core::Settings_t;

inline /*static*/ ::covellite::core::SectionPtr_t Settings_t::Make(void)
{
  return ::std::make_unique<::covellite::core::Section_t>(uT("MockSection"));
}
