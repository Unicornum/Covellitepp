
#include "stdafx.h"
#include <Covellite/Os/Configuration.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string/encoding.hpp>

using namespace covellite::os;

auto Configuration::GetRawSystemLanguage(void) const -> String_t
{
  using namespace ::alicorn::extension::std;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;

  char Language[30] = { 0 };
  AConfiguration_getLanguage(AppInfo_t::Get<android_app>().config, Language);
  return string_cast<String, Encoding::UTF8>(Language);
}
