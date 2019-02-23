
#include "stdafx.h"
#include "Layer.hpp"
#include <alicorn/std/string.hpp>
#include <alicorn/version.hpp>
#include "Description.hpp"

using namespace layers;

Layer::Layer(IWindowGui_t & _Window, const Path_t & _FileName,
  const String_t & _Title, const String_t & _Description) :
  ::covellite::gui::Layer(_Window, "data" / _FileName),
  m_FontSize(EmployFontSize(2.5))
{
  using namespace ::alicorn::extension::std;

  GetElement("id_back").SetMeaning(uT("\uF060"));
  GetElement("id_help").SetMeaning(uT("\uF059"));
  GetElement("id_title").SetMeaning(_Title);

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