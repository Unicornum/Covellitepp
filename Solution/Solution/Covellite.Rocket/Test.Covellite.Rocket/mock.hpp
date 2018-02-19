
#pragma once
#include <memory>
#include <windows.h>
#include <alicorn\std\exception.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\platform\app-info.hpp>
#include <alicorn\logger.mock.hpp>
#include <alicorn\settings.mock.hpp>
#include <Rocket\Rocket.mock.hpp>
#include <Android\Android.mock.hpp>

namespace testing { namespace internal {

// Ёто чтобы не падал gmock при возврате boost::filesystem::path.
inline void PrintTo(const ::boost::filesystem::path & p, ::std::ostream * os)
{
  *os << p;
}

} }

#include <alicorn\application\current-module.mock.hpp>

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include "Mock\Namespaces.hpp"

#include "Mock\Time.hpp"
#include "Mock\chrono.mock.hpp"
#include "Mock\IStringTranslator.hpp"
#include "Mock\StringTranslator.hpp"
#include "Mock\RocketInterfaces.hpp"

#include <Covellite\Covellite.mock.hpp>
#include <Covellite.Jni\Jni.mock.hpp>
