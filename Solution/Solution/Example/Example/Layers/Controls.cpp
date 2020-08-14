
#include "stdafx.h"
#include "Controls.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

using namespace layers;

const auto LayerDescription = 
  uT("Пример использования элементов управления:<br/>") +
  uT("- Однострочное поле ввода текста.<br/>") +
  uT("- Кнопки (выбор одного значения из нескольких).<br/>") +
  uT("- Кнопки (увеличение/уменьшение значения).<br/>") +
  uT("- Выпадающий список.<br/>") +
  uT("- Checkbox (на базе кнопки с использованием символов шрифта).<br/>") +
  uT("- Ползунок.<br/>") +
  uT("");

Controls::Controls(IWindowGui_t & _Window) :
  Layer(_Window, "controls.rml", uT("Настройки"), LayerDescription)
{
  namespace events = ::covellite::events;
  using namespace ::alicorn::extension::std;

  // ************************************************************************ //
  // ************************************************************************ //

  AddPart(uT("Персонаж"), uT("Параметры персонажа"));

  // ************************************************************************ //

  AddParameter(uT("\uF2BD"), uT("Имя"), 
    uT("<input type=\"text\" value=\"<нет>\" id=\"id_name\"/>"), 
    uT("none"), uT(""));

  m_Events[events::Change.DocumentId(GetId()).ElementId("id_name")]
    .Connect([&](void)
  {
    auto Name = GetElement("id_name");
    Name.SetMeaning(Name.GetMeaning().Replace(uT("<нет>"), uT("")));
  });

  // ************************************************************************ //

  AddParameter(uT("\uF228"), uT("Пол"), uT("не выбран"), uT("id_sex"),
    BuildButton(uT("id_female"), uT("\uF221"), true) +
    BuildButton(uT("id_male"), uT("\uF222"), true));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_male")]
    .Connect([&](void)
  {
    GetElement("id_sex").SetMeaning(uT("Мужской"));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_female")]
    .Connect([&](void)
  {
    GetElement("id_sex").SetMeaning(uT("Женский"));
  });

  // ************************************************************************ //

  AddParameter(uT("\uF77C"), uT("Возраст"), uT("20"), uT("id_age"), 
    BuildButton(uT("id_plus"), uT("\uF067"), true) +
    BuildButton(uT("id_minus"), uT("\uF068"), true));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_minus")]
    .Connect([&](void)
  {
    if (m_Age > 15) m_Age--;
    GetElement("id_age").SetMeaning(uT("%AGE%").Replace(uT("%AGE%"), m_Age));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_plus")]
    .Connect([&](void)
  {
    if (m_Age < 45) m_Age++;
    GetElement("id_age").SetMeaning(uT("%AGE%").Replace(uT("%AGE%"), m_Age));
  });

  // ************************************************************************ //

  // value - это строки, которые будет возвращать функция GetMeaning(), 
  // а значения узлов option - строки, которые будут отображаться на экране.
  // Атрибут size не поддерживается.
  AddParameter(uT("\uF042"), uT("Раса"), 
    uT("<select id=\"id_race\">") + 
    uT("  <option value=\"Гном\">Гном</option>") +
    uT("  <option value=\"Дварф\">Дварф</option>") +
    uT("  <option value=\"Человек\" selected>Человек</option>") + // Изначально выбранный элемент
    uT("  <option value=\"Эльф\">Эльф</option>") +
    uT("</select>"),
    uT(""), uT(""));

  // Это не работает, для установки значения, прочитанного из настроек,
  // формировать список, у которого для заданного элемента стоит атрибут
  // selected.
  //GetElement("id_race").SetMeaning(uT("Человек"));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_race")]
    .Connect([&](void)
  {
    // Сюда не попадаем!
  });

  m_Events[events::Change.DocumentId(GetId()).ElementId("id_race")]
    .Connect([&](void)
  {
    const auto SelectedValue = GetElement("id_race").GetMeaning();
  });

  // ************************************************************************ //
  // ************************************************************************ //

  AddPart(uT("Разное"), uT("Параметры игры"));

  // ************************************************************************ //

  AddParameter(uT("\uF026"), uT("Звук"), uT("Выключен"), 
    uT("id_sound_enabled_value"),
    BuildButton(uT("id_sound_enabled"), uT("\uF204"), true));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_sound_enabled")]
    .Connect([&](void) 
  { 
    m_IsSoundEnabled = !m_IsSoundEnabled;

    GetElement("id_sound_enabled").SetMeaning(
      m_IsSoundEnabled ? uT("\uF205") : uT("\uF204"));
    GetElement("id_sound_enabled_value").SetMeaning(
      m_IsSoundEnabled ? uT("Включен") : uT("Выключен"));
  });

  // ************************************************************************ //

  AddParameter(uT("\uF027"), uT("Громкость звука"), 
    uT("<div>0 <input id=\"id_volume\" type=\"range\" min=\"0\" max=\"100\" value=\"50\" step=\"1\"/> 100</div>"),
    uT(""), uT(""));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_volume")]
    .Connect([&](void)
  {
    // Сюда не попадаем!
  });

  m_Events[events::Change.DocumentId(GetId()).ElementId("id_volume")]
    .Connect([&](void)
  {
    const auto Value = GetElement("id_volume").GetMeaning();
  });
}

void Controls::AddPart(const String_t & _Name, const String_t & _Description)
{
  m_Index = 0;

  auto Settings = GetElement("id_settings");

  using namespace ::alicorn::extension::std;

  Settings.SetMeaning(Settings.GetMeaning() +
      uT("<part>") +
      uT("<part-name>") + _Name + uT("</part-name>") +
      uT("<description>") + _Description + uT("</description>") +
      uT("</part>") +
      uT(""));
}

void Controls::AddParameter(const String_t & _Icon, const String_t & _Name,
  const String_t & _DefaultValue, const String_t & _ValueId, 
  const String_t & _Control)
{
  m_Index++;

  auto Settings = GetElement("id_settings");

  using namespace ::alicorn::extension::std;

  Settings.SetMeaning(Settings.GetMeaning() +
      uT("<param>") +
      uT("<icon class=\"ficons\">") + _Icon + uT("</icon>") +
      uT("<control-panel>") + _Control + uT("</control-panel>") +
      uT("<div>") +
      uT("<value-name>") + _Name + uT("</value-name>") +
      uT("<value id=\"") + _ValueId + uT("\">") + _DefaultValue + uT("</value>") +
      uT("</div>") +
      uT("</param>") +
      uT(""));
}

/*static*/ auto Controls::BuildButton(const String_t & _Id,
  const String_t & _Code, bool _IsUsingBackSymbol) -> String_t
{
  return     
    uT("<control class=\"fcontrols\">") +
    uT("<control-back>") + 
      (_IsUsingBackSymbol ? uT("\uF111") : uT(" ")) + 
    uT("</control-back>") +
    uT("<button class=\"control\" id=\"") + _Id + uT("\">") + 
      _Code + 
    uT("</button>") +
    uT("</control>");
}
