
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

#include <Covellite/Core.mock.hpp>
#include <Covellite/Core/Settings.mock.hpp>
#include <Covellite/Gui.inside.mock.hpp>

#include "Mock/Time.hpp"
#include "Mock/StringTranslator.hpp"
#include "Mock/RocketInterfaces.hpp"
#include "Mock/SystemToRocketKeyCode.inl"
#include "Mock/Renderer.hpp"
