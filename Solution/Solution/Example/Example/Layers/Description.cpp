
#include "stdafx.h"
#include "Description.hpp"
#include <alicorn/std/string.hpp>

using namespace layers;

Description::Description(IWindowGui_t & _Window) :
  Layer(_Window, "description.rml", uT("???"), uT("???"))
{
  using namespace ::alicorn::extension::std;

  m_Events[Help.Title].Connect([&](const String & _Text)
  {
    GetElement("id_title").SetMeaning(_Text);
  });

  m_Events[Help.Text].Connect([&](const String & _Text)
  {
    GetElement("id_description").SetMeaning(_Text + uT("<br/>") +
        uT("- Для возврата на предыдущий экран нажмите иконку в левой части ") +
        uT("заголовка или <p>[BACK]</p>.<br/>") +
        uT("- Для выхода из программы нажмите <p>[EXIT]</p>."));
  });
}
