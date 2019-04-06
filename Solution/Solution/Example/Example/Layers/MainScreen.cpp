
#include "stdafx.h"
#include "MainScreen.hpp"
#include <alicorn/std/string.hpp>
#include <alicorn/version.hpp>
#include "Layer.hpp"

using namespace layers;

/// [Layer example]
MainScreen::MainScreen(IWindowGui_t & _Window) :
  Layer(_Window, "data/main.rml")
{
  /// [Layer example]

  const ::alicorn::system::version::Info VersionInfo;

  using namespace ::alicorn::extension::std;

  EmployFontSize(2.5);

  GetElement("id_version").SetMeaning(uT("<h2>Версия: %VERSION%</h2>")
    .Replace(uT("%VERSION%"), VersionInfo.GetValue(uT("FileVersionShort"))));

  GetElement("id_description").SetMeaning(
    uT("Это тестовое демо фреймворка Covellite++, которое реализовано ") +
    uT("в виде набора отдельных окон, каждое из которых демонстрирует ") +
    uT("использование различных возможностей фреймворка (в скобках указано ") +
    uT("кодовое обозначение окна применительно к игре):<br/>") +
    uT("- Использование элементов управления (настройки).<br/>") +
    uT("- Вывод текста в различных форматах (диалоги).<br/>") +
    uT("- Использование 3D объектов в приложении.<br/>") +
    uT("- Пример реализации простой 2D игры.<br/>") +
    uT("- Демонстрационный режим (3D игра).<br/>") +
    uT("<br/>") +
    uT("В заголовке каждого окна есть кнопки перехода к подробному описанию ") +
    uT("текущего окна (справа) и возврату к предыдущему окну (слева).") +
    uT("<br/>") +
    uT("<br/>") +
    uT("Для продолжения нажмите одну из иконок внизу.<br/>") +
    uT("<br/>") +
    uT("<br/>"));

  GetElement("id_copyright").SetMeaning(
    VersionInfo.GetValue(uT("LegalCopyright")));

  GetElement("id_controls").SetMeaning(uT("\uF013"));
  GetElement("id_text").SetMeaning(uT("\uF2BD"));
  GetElement("id_draw3dobject").SetMeaning(uT("\uF6D1"));
  GetElement("id_simple2dgame").SetMeaning(uT("\uF443"));
  GetElement("id_demo").SetMeaning(uT("\uF1BB"));

  using namespace ::covellite;

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_controls")]
    .Connect([&](void) { m_Events[Button.Controls](); });
  m_Events[events::Click.DocumentId(GetId()).ElementId("id_text")]
    .Connect([&](void) { m_Events[Button.Text](); });
  m_Events[events::Click.DocumentId(GetId()).ElementId("id_draw3dobject")]
    .Connect([&](void) { m_Events[Button.Draw3DObject](); });
  m_Events[events::Click.DocumentId(GetId()).ElementId("id_simple2dgame")]
    .Connect([&](void) { m_Events[Button.Simple2DGame](); });
  m_Events[events::Click.DocumentId(GetId()).ElementId("id_demo")]
    .Connect([&](void) { m_Events[Button.Demo](); });
}
