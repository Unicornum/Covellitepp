
#pragma once

/**
* \file
*  «аголовочный файл дл€ унификации использовани€
*  [Rocket](https://github.com/libRocket/libRocket) или
*  [RmlUi](https://github.com/mikke89/RmlUi) в качестве реализации GUI.
* \details
*  - ƒл€ включени€ нужной библиотеки в свойствах проектов Covellite.Gui и 
*  Covellite.Gui.Android необходимо определить макрос COVELLITE_GUI_ROCKET или
*  COVELLITE_GUI_RMLUI.
*  - “естовые проекты, тестирующие логику работы каждой из библиотек также
*  должны определ€ть соответствующий макрос.
*  - ќстальные проекты не должны определ€ть макросов типа библиотеки, в этом
*  случае данный заголовочный файл раскроетс€ в предварительные определени€.
*/

#ifdef COVELLITE_GUI_ROCKET

#ifndef __USING_GTEST

#include <Rocket/Rocket.hpp>
#include <Rocket/Plugin.hpp>
namespace CovelliteGui = ::Rocket;

#else // __USING_GTEST

#define FontDatabase FontDatabase_dummy
#include <Rocket/Rocket.mock.hpp>
#undef FontDatabase

namespace mock
{

namespace Rocket
{

namespace Core
{

class FontDatabase
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(LoadFontFace, bool(String));
    MOCK_METHOD2(LoadFontFace, bool(::std::vector<byte>, String));
  };

public:
  static bool LoadFontFace(const String & _Path)
  {
    return Proxy::GetInstance()->LoadFontFace(_Path);
  }

  static bool LoadFontFace(const ::std::vector<byte> & _Data,
    const String & _Dummy)
  {
    return Proxy::GetInstance()->LoadFontFace(_Data, _Dummy);
  }
};

} // namespace Core

} // namespace Rocket

} // namespace mock

namespace CovelliteGui = ::mock::Rocket;

namespace mock
{

namespace CovelliteGui = ::CovelliteGui;

namespace Rocket
{

namespace Core { using EventListener_tested = EventListener; }

namespace Plugin
{

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
public:
  MOCK_METHOD0(Initialise, bool(void));
};

inline void Initialise(void)
{
  Plugin::Proxy::GetInstance()->Initialise();
}

} // namespace Plugin 

} // namespace Rocket 

} // namespace mock 

#endif // __USING_GTEST

namespace covellite
{

namespace gui
{

using CovelliteGuiTime_t = float;
using CovelliteGuiUnicode_t = CovelliteGui::Core::word;

inline ::std::string CovelliteGuiStringToUtf8(
  const CovelliteGui::Core::String & _String)
{
  return _String.CString();
}

template<class T>
inline void CovelliteGuiRemove(T * _pValue)
{
  _pValue->RemoveReference();
}

inline void CovelliteGuiLoadFontFace(const ::std::string & _Path)
{
  CovelliteGui::Core::FontDatabase::LoadFontFace(_Path.c_str());
}

inline void CovelliteGuiLoadFontFace(const ::std::vector<uint8_t> & _Data)
{
  CovelliteGui::Core::FontDatabase::LoadFontFace(_Data, "");
}

inline void CovelliteGuiSetProgressBarValue(
  CovelliteGui::Core::Element * _pProgressBar,
  const float _Value)
{
  using namespace ::alicorn::extension::std;

  auto & Control =
    dynamic_cast<CovelliteGui::Controls::ElementFormControl &>(*_pProgressBar);

  Control.SetValue(string_cast<::std::string, Encoding::UTF8>(
    uT("%VALUE%").Replace(uT("%VALUE%"), _Value)).c_str());
}

} // namespace gui

} // namespace covellite

#elif defined COVELLITE_GUI_RMLUI

#ifndef __USING_GTEST

#include <RmlUi/RmlUi.hpp>
namespace Rml { namespace Plugin { inline void Initialise(void) {} } }
namespace CovelliteGui = ::Rml;

#else // __USING_GTEST

#include "..\Test.Covellite.Gui\Mock\RmlUi.mock.hpp"
namespace CovelliteGui = ::mock::Rml;

namespace mock
{

namespace CovelliteGui = ::CovelliteGui;

namespace Rml
{

namespace Core { using EventListener_tested = EventListener; }

namespace Plugin 
{ 

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
public:
  MOCK_METHOD0(Initialise, bool(void));
};

inline void Initialise(void) 
{
  Plugin::Proxy::GetInstance()->Initialise();
} 

} // namespace Plugin 

} // namespace Rml 

} // namespace mock 

#endif // __USING_GTEST

namespace covellite
{

namespace gui
{

using CovelliteGuiTime_t = double;
using CovelliteGuiUnicode_t = CovelliteGui::Core::Character;

inline ::std::string CovelliteGuiStringToUtf8(
  const CovelliteGui::Core::String & _String)
{
  return _String;
}

inline void CovelliteGuiRemove(CovelliteGui::Core::ElementDocument * _pDocument)
{
  _pDocument->Close();
}

inline void CovelliteGuiRemove(CovelliteGui::Core::Context * _pContext)
{
  CovelliteGui::Core::RemoveContext(_pContext->GetName());
}

inline void CovelliteGuiLoadFontFace(const ::std::string & _Path)
{
  CovelliteGui::Core::LoadFontFace(_Path.c_str());
}

inline void CovelliteGuiLoadFontFace(const ::std::vector<uint8_t> & _Data)
{
  CovelliteGui::Core::LoadFontFace(_Data.data(), static_cast<int>(_Data.size()), 
    "", CovelliteGui::Core::Style::FontStyle::Normal, 
    CovelliteGui::Core::Style::FontWeight::Normal);
}

inline void CovelliteGuiSetProgressBarValue(
  CovelliteGui::Core::Element * _pProgressBar,
  const float _Value)
{
  auto & Control =
    dynamic_cast<CovelliteGui::Controls::ElementProgressBar &>(*_pProgressBar);

  Control.SetValue(_Value);
}

} // namespace gui

} // namespace covellite

#else // COVELLITE_GUI_*

namespace CovelliteGui
{

namespace Core
{

template<typename> class Vector2;
using Vector2i = Vector2<int>;
class ElementDocument;
class Element;
class Context;

} // namespace Core

} // namespace CovelliteGui

#endif // COVELLITE_GUI_*
