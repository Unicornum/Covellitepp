
#pragma once
#include <alicorn\settings.mock.hpp>
#include <Covellite\App\Settings.hpp>

/// \cond DoxygenWarningSuppress

namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::app;

template<>
inline /*static*/ SectionPtr_t Singleton<Section_t>::Make(void)
{
  return ::std::make_unique<Section_t>(uT("MockSection"));
}

} // namespace std

} // namespace extension

} // namespace alicorn

/// \endcond
