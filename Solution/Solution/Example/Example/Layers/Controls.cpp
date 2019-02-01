
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
  using namespace ::covellite;
  using namespace ::alicorn::extension::std;

  AddPart(uT("Персонаж"), uT("Параметры персонажа"));

  AddParameter(uT("\uF2BD"), uT("Имя"), 
    uT("<input type=\"text\" value=\"нет\" />"), 
    uT("id_name"), uT(""));

  AddParameter(uT("\uF228"), uT("Пол"), uT("не выбран"), uT("id_sex"),
    BuildButton(uT("id_female"), uT("\uF221"), true) +
    BuildButton(uT("id_male"), uT("\uF222"), true));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_male")]
    .Connect([&](void)
  {
    GetElement("id_sex").SetText(u8"Мужской");
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_female")]
    .Connect([&](void)
  {
    GetElement("id_sex").SetText(u8"Женский");
  });

  AddParameter(uT("\uF77C"), uT("Возраст"), uT("20"), uT("id_age"), 
    BuildButton(uT("id_plus"), uT("\uF067"), true) +
    BuildButton(uT("id_minus"), uT("\uF068"), true));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_minus")]
    .Connect([&](void)
  {
    if (m_Age > 15) m_Age--;
    GetElement("id_age").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("%AGE%").Replace(uT("%AGE%"), m_Age)));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_plus")]
    .Connect([&](void)
  {
    if (m_Age < 45) m_Age++;
    GetElement("id_age").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("%AGE%").Replace(uT("%AGE%"), m_Age)));
  });

  AddParameter(uT("\uF042"), uT("Раса"), 
    uT("<select id=\"id_race\" size=\"1\" >") + 
    uT("<option>Гном</option>") +
    uT("<option>Дварф</option>") +
    uT("<option selected>Человек</option>") + // Изначально выбранный элемент
    uT("<option>Эльф</option>") +
    uT("</select>"),
    uT(""), uT(""));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_race")]
    .Connect([&](void)
  {
    // Сюда не попадаем.

    // Так, по идее, следует получать выбранное значение.
    //const auto SelectedValue = GetElement("id_race").GetAttribute(u8"source"));
  });

  AddPart(uT("Разное"), uT("Параметры игры"));

  AddParameter(uT("\uF026"), uT("Звук"), uT("Выключен"), 
    uT("id_sound_enabled_value"),
    BuildButton(uT("id_sound_enabled"), uT("\uF204"), false));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_sound_enabled")]
    .Connect([&](void) 
  { 
    m_IsSoundEnabled = !m_IsSoundEnabled;

    GetElement("id_sound_enabled").SetText(
      m_IsSoundEnabled ? u8"\uF205" : u8"\uF204");
    GetElement("id_sound_enabled_value").SetText(
      m_IsSoundEnabled ? u8"Включен" : u8"Выключен");
  });

  AddParameter(uT("\uF027"), uT("Громкость звука"), 
    uT("0 <input id=\"id_volume\" type=\"range\" min=\"0\" max=\"100\" value=\"50\" step=\"1\"/> 100"),
    uT(""), uT(""));

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_volume")]
    .Connect([&](void)
  {
    // 25 Январь 2019 19:40 (unicornum.verum@gmail.com)
    TODO("Требуется событие изменения значения.");
    // Сюда не попадаем.

    // Так, по идее, следует получать выбранное значение.
    //const auto Value = GetElement("id_volume").GetAttribute(u8"value"));
  });
}

void Controls::AddPart(const String_t & _Name, const String_t & _Description)
{
  m_Index = 0;

  auto Settings = GetElement("id_settings");

  using namespace ::alicorn::extension::std;

  Settings.SetText(Settings.GetText() + 
    string_cast<::std::string, Locale::UTF8>(
      uT("<part>") +
      uT("<part-name>") + _Name + uT("</part-name>") +
      uT("<description>") + _Description + uT("</description>") +
      uT("</part>") +
      uT("")));
}

void Controls::AddParameter(const String_t & _Icon, const String_t & _Name,
  const String_t & _DefaultValue, const String_t & _ValueId, 
  const String_t & _Control)
{
  m_Index++;

  auto Settings = GetElement("id_settings");

  using namespace ::alicorn::extension::std;

  Settings.SetText(Settings.GetText() +
    string_cast<::std::string, Locale::UTF8>(
      uT("<param>") +
      uT("<icon class=\"ficons\">") + _Icon + uT("</icon>") +
      uT("<control-panel>") + _Control + uT("</control-panel>") +
      uT("<div>") +
      uT("<value-name>") + _Name + uT("</value-name>") +
      uT("<value id=\"") + _ValueId + uT("\">") + _DefaultValue + uT("</value>") +
      uT("</div>") +
      uT("</param>") +
      uT("")));
}

/*static*/ auto Controls::BuildButton(const String_t & _Id,
  const String_t & _Code, bool _IsUsingBackSymbol) -> String_t
{
  return     
    uT("<control class=\"fcontrols\">") +
    uT("<control-back>") + 
      (_IsUsingBackSymbol ? uT("\uF111") : uT("1")) + 
    uT("</control-back>") +
    uT("<button class=\"control\" id=\"") + _Id + uT("\">") + 
      _Code + 
    uT("</button>") +
    uT("</control>");
}
