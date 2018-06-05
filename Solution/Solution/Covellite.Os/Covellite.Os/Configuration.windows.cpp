
#include "stdafx.h"
#include <Covellite\Os\Configuration.hpp>
#include <alicorn\std\string.hpp>

using namespace covellite::os;

auto Configuration::GetRawSystemLanguage(void) const -> String_t
{
  wchar_t Buffer[LOCALE_NAME_MAX_LENGTH] = { 0 };
  const auto Count = GetUserDefaultLocaleName(Buffer, LOCALE_NAME_MAX_LENGTH);

  return (Count == 0) ? uT("Unknown") : 
    ::alicorn::extension::std::String::GetString(Buffer);
}
