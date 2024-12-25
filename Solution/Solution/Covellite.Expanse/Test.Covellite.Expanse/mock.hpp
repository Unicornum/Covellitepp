
#pragma once
#include <alicorn/cpp/disable-warnings.hpp>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string/string-cast.hpp>
#include <alicorn/std/singleton.hpp>
#include <alicorn/settings.mock.hpp>
#include <alicorn/logger.mock.hpp>

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include "Mock/Namespaces.hpp"

#include <Covellite/App.mock.hpp>
#include <Covellite/Os.mock.hpp>
#include <Covellite/Api.mock.hpp>

#include "Mock/GameScene.hpp"
#include "Mock/3DScene.hpp"

namespace alicorn::extension::std
{

using Section_t = alicorn::modules::settings::Section;
using SectionPtr_t = ::std::unique_ptr<Section_t>;

/*static*/ SectionPtr_t Singleton<Section_t>::Make(void)
{
  return ::std::make_unique<Section_t>(uT("Dummy"));
}

} // namespace alicorn::extension::std
