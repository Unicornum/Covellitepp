
#pragma once
#include <memory>
#include <windows.h>
#include <alicorn\std\exception.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\std\chrono.mock.hpp>
#include <alicorn\platform\app-info.hpp>
#include <alicorn\platform\environment.mock.hpp>
#include <alicorn\logger.mock.hpp>
#include <alicorn\settings.mock.hpp>
#include <Rocket\Rocket.mock.hpp>
#include <Platform\Android.mock.hpp>

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

#include <Covellite\Core.mock.hpp>
#include <Covellite\Rocket.inside.mock.hpp>

#include "Mock\Time.hpp"
#include "Mock\StringTranslator.hpp"
#include "Mock\RocketInterfaces.hpp"
