
#pragma once

/**
* \file
*  ������������ ���� ��� ���������� �������������
*  [Rocket](https://github.com/libRocket/libRocket) ���
*  [RmlUi](https://github.com/mikke89/RmlUi) � �������� ���������� GUI.
* \details
*  - ��� ��������� ������ ���������� � ��������� �������� Covellite.Gui � 
*  Covellite.Gui.Android ���������� ���������� ������ COVELLITE_GUI_ROCKET ���
*  COVELLITE_GUI_RMLUI.
*  - �������� �������, ����������� ������ ������ ������ �� ��������� �����
*  ������ ���������� ��������������� ������.
*  - ��������� ������� �� ������ ���������� �������� ���� ����������, � ����
*  ������ ������ ������������ ���� ���������� � ��������������� �����������.
*/

#ifdef COVELLITE_GUI_RMLUI

#ifndef __USING_GTEST

#include <RmlUi/RmlUi.hpp>
namespace CovelliteGui = ::Rml;

#else // __USING_GTEST

// 01 ������ 2025 17:58 (unicornum.verum@gmail.com)
TODO("�������� ������, ����� � Alicorn SDK ������� ����������� ���� RmlUi.mock.hpp");
//#include <RmlUi\RmlUi.mock.hpp>
#include <Externals\Alicorn\Externals\include\rmlui\RmlUi.mock.hpp>

namespace CovelliteGui = ::mock::Rml;

namespace mock
{

namespace CovelliteGui = ::CovelliteGui;

namespace Rml
{

using EventListener_tested = EventListener;

} // namespace Rml 

} // namespace mock 

#endif // __USING_GTEST

namespace covellite
{

namespace gui
{

using CovelliteGuiTime_t = double;
using CovelliteGuiUnicode_t = CovelliteGui::Character;

inline ::std::string CovelliteGuiStringToUtf8(
  const CovelliteGui::String & _String)
{
  return _String;
}

inline void CovelliteGuiRemove(CovelliteGui::ElementDocument * _pDocument)
{
  _pDocument->Close();
}

inline void CovelliteGuiRemove(CovelliteGui::Context * _pContext)
{
  CovelliteGui::RemoveContext(_pContext->GetName());
}

inline void CovelliteGuiLoadFontFace(const ::std::string & _Path)
{
  CovelliteGui::LoadFontFace(_Path.c_str());
}

inline void CovelliteGuiLoadFontFace(const ::std::vector<uint8_t> & _Data)
{
  CovelliteGui::LoadFontFace(_Data.data(), static_cast<int>(_Data.size()), 
    "", CovelliteGui::Style::FontStyle::Normal, 
    CovelliteGui::Style::FontWeight::Normal);
}

inline void CovelliteGuiSetProgressBarValue(
  CovelliteGui::Element * _pProgressBar,
  const float _Value)
{
  auto & Control =
    dynamic_cast<CovelliteGui::ElementProgress &>(*_pProgressBar);

  Control.SetValue(_Value);
}

} // namespace gui

} // namespace covellite

#else // COVELLITE_GUI_*

namespace CovelliteGui
{

template<typename> class Vector2;
using Vector2i = Vector2<int>;
class ElementDocument;
class Element;
class Context;

} // namespace CovelliteGui

#endif // COVELLITE_GUI_*
