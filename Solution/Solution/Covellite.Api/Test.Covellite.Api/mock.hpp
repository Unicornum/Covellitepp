
#pragma once

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include <algorithm>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/string/string.hpp>
#include <alicorn/std/string/string-cast.hpp>
#include <alicorn/logger.mock.hpp>
#include <Platform/Windows.mock.hpp>
#include <Platform/Android.mock.hpp>

#include "Mock/Namespaces.hpp"

#include <Covellite/App/Settings.mock.hpp>
//#include <Covellite/Os/Configuration.mock.hpp>

class Tested
{
public:
  static int & GetValue(void)
  {
    static int Value = 0;
    return Value;
  }
};

#define TEST_CALL_IF(n) \
  if (Tested::GetValue() == n)
