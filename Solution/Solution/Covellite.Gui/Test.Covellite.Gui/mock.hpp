
#pragma once
#include <memory>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/platform/environment.mock.hpp>
#include <alicorn/platform/windows.hpp>
#include <alicorn/application/current-module.mock.hpp>
#include <windef.h>
#include <winuser.h>
#include <alicorn/logger.mock.hpp>
#include <alicorn/settings.mock.hpp>
#include <Platform/Android.mock.hpp>

#include <Covellite/App/Vfs.hpp>

using ::covellite::app::VfsCore_t;
using ::covellite::app::VfsPtr_t;
using ::alicorn::extension::std::Singleton;

//! @cond Doxygen_Suppress

inline /*static*/ VfsPtr_t Singleton<VfsCore_t>::Make(void)
{
  using ::alicorn::modules::vfs::FileSystem;
  using ImplPtr_t = ::std::shared_ptr<::alicorn::modules::vfs::IImplementation>;

  return ::std::make_unique<VfsCore_t>(::std::vector<ImplPtr_t>
  {
    // Ёто позвол€ет задавать как полный путь, так и путь относительно
    // папки этого самого файла.
    ::std::make_shared<FileSystem>(""),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.File"),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.Renderer")
  });
}

//! @endcond

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include "Mock/Namespaces.hpp"

#include <Covellite/App/Settings.mock.hpp>
#include <Covellite/Gui/External.hpp>
#include <Covellite/Gui.inside.mock.hpp>

#include "Mock/Time.hpp"
#include "Mock/StringTranslator.hpp"
#include "Mock/CovelliteGuiInterfaces.hpp"
#include "Mock/SystemToGuiKeyCode.inl"
#include "Mock/Renderer.hpp"
