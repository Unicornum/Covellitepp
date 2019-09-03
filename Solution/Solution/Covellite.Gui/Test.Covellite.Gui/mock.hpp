
#pragma once
#include <memory>
#include <vector>
#include <map>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <boost/filesystem.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/platform/environment.mock.hpp>
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
    // ��� ��������� �������� ��� ������ ����, ��� � ���� �����������
    // ����� ����� ������ �����.
    ::std::make_shared<FileSystem>(""),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.File"),
    ::std::make_shared<FileSystem>(THIS_DIRECTORY / "Test.Renderer")
  });
}

//! @endcond

// 06 ������� 2019 13:08 (unicornum.verum@gmail.com)
TODO("�������� ���� � Externals � ������� ������ �� ����.");
#include "Mock/Rocket.mock.hpp"
//#include <Rocket/Rocket.mock.hpp>

namespace testing { namespace internal {

// ��� ����� �� ����� gmock ��� �������� boost::filesystem::path.
inline void PrintTo(const ::boost::filesystem::path & p, ::std::ostream * os)
{
  *os << p;
}

} }

#include <alicorn/application/current-module.mock.hpp>

/**
* \file
*  ���������� ���������� �������.
*/

#include "Mock/Namespaces.hpp"

#include <Covellite/App/Settings.mock.hpp>
#include <Covellite/Gui.inside.mock.hpp>

#include "Mock/Time.hpp"
#include "Mock/StringTranslator.hpp"
#include "Mock/RocketInterfaces.hpp"
#include "Mock/SystemToRocketKeyCode.inl"
#include "Mock/Renderer.hpp"
