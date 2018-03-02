
#pragma once
#include <alicorn\settings.mock.hpp>
#include <Covellite\Core\Settings.hpp>

/// \cond DoxygenWarningSuppress

using ::covellite::core::Settings_t;

inline /*static*/ ::covellite::core::SectionPtr_t Settings_t::Make(void)
{
  return ::std::make_unique<::covellite::core::Section_t>(uT("MockSection"));
}

/// \endcond
