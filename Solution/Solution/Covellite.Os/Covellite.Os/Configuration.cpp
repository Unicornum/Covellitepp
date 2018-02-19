
#include "stdafx.h"
#include <Covellite\Os\Configuration.hpp>
#include <map>
#include <alicorn\std\string.hpp>

using namespace covellite::os;

auto Configuration::GetSystemLanguage(void) const -> String_t
{
  static const ::std::map<String_t, String_t> Languages =
  {
    { uT("en"), uT("English") },
    { uT("ru"), uT("Russian") },
  };

  const auto RawSystemLanguage = GetRawSystemLanguage();

  for (const auto Language : Languages)
  {
    if (RawSystemLanguage.Find(Language.first) == 0) return Language.second;
  }

  return uT("Default");
}
