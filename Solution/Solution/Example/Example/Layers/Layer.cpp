
#include "stdafx.h"
#include "Layer.hpp"
#include <alicorn/std/string.hpp>
#include <alicorn/version.hpp>
#include <alicorn/application/current-module.hpp>
#include "Description.hpp"

static const auto PathToDataDirectory =
  ::alicorn::system::application::CurrentModule::GetAppRootPath() / "data";

using namespace layers;

Layer::Layer(IWindowGui_t & _Window, const ::std::string & _FileName, 
  const String_t & _Title, const String_t & _Description) :
  ::covellite::gui::Layer(_Window, PathToDataDirectory / _FileName),
  m_FontSize(::std::max(GetWidth(), GetHeight()) / 40)
{
  using namespace ::alicorn::extension::std;

  GetElement("id_body").SetStyle(string_cast<::std::string, Locale::UTF8>(
    uT("font-size: %SIZE%px;").Replace(uT("%SIZE%"), m_FontSize)));

  GetElement("id_back").SetText(u8"\uF060");
  GetElement("id_help").SetText(u8"\uF059");
  GetElement("id_title").SetText(
    string_cast<::std::string, Locale::UTF8>(_Title));

  using namespace ::covellite;

  m_Events[Button.Help].Connect([&, _Title, _Description](void)
  {
    m_Events[Help.Title](_Title);
    m_Events[Help.Text](_Description);
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_help")]
    .Connect([&](void) { m_Events[Button.Help](); });
  m_Events[events::Click.DocumentId(GetId()).ElementId("id_back")]
    .Connect([&](void) { m_Events[Button.Back](); });
}
