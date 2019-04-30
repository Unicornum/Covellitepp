
#pragma once
#include <memory>
#include <windows.h>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/platform/environment.mock.hpp>
#include <alicorn/logger.mock.hpp>
#include <alicorn/settings.mock.hpp>
#include <Platform/Android.mock.hpp>

#include <Covellite/Gui/Vfs.hpp>

using ::covellite::gui::VfsCore_t;
using ::covellite::gui::VfsPtr_t;
using ::alicorn::extension::std::Singleton;

inline /*static*/ VfsPtr_t Singleton<VfsCore_t>::Make(void)
{
  using ::alicorn::modules::vfs::FileSystem;
  using ImplPtr_t = ::std::shared_ptr<::alicorn::modules::vfs::IImplementation>;

  return ::std::make_unique<VfsCore_t>(::std::vector<ImplPtr_t>
  {
    // Это позволяет задавать как полный путь, так и путь оносительно
    // папки этого самого файла.
    ::std::make_shared<FileSystem>(""),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.File"),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.Renderer")
  });
}

// 06 Февраль 2019 13:08 (unicornum.verum@gmail.com)
TODO("Заменить файл в Externals и вернуть ссылку на него.");
#include "Mock/Rocket.mock.hpp"
//#include <Rocket/Rocket.mock.hpp>

namespace testing { namespace internal {

// Это чтобы не падал gmock при возврате boost::filesystem::path.
inline void PrintTo(const ::boost::filesystem::path & p, ::std::ostream * os)
{
  *os << p;
}

} }

#include <alicorn/application/current-module.mock.hpp>

/**
* \file
*  Объявления подставных классов.
*/

#include "Mock/Namespaces.hpp"

#include <Covellite/Core.mock.hpp>
#include <Covellite/Core/Settings.mock.hpp>
#include <Covellite/Gui.inside.mock.hpp>

#include "Mock/Time.hpp"
#include "Mock/StringTranslator.hpp"
#include "Mock/RocketInterfaces.hpp"
#include "Mock/SystemToRocketKeyCode.inl"
#include "Mock/Renderer.hpp"
