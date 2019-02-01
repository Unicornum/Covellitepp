
#include "stdafx.h"
#include "Text.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

using namespace layers;

const auto LayerDescription =
  uT("Демонстрация вывода текста:.<br/>") +
  uT("- Разными шрифтами.<br/>") +
  uT("- Разного размера.<br/>") +
  uT("- Разного цвета.<br/>") +
  uT("- Подчеркнутый и курсив.<br/>") +
  uT("- Со вставленными картинками.<br/>") +
  uT("<br/>") +
  uT("При выборе одной из реплик должен меняться статус персонажа ") +
  uT("с соответствующим изменением цвета статуса.<br/>");

Text::Text(IWindowGui_t & _Window) :
  Layer(_Window, "text.rml", uT("Текст в разных форматах"), LayerDescription)
{
  using namespace ::alicorn::extension::std;

  GetElement("id_text").SetText(string_cast<::std::string, Locale::UTF8>(
    uT("<big>Д</big>а, мы слышали о проблемах на побережье и поэтому меня ") +
    uT("отправили на разведку. До сих пор я видел только <i>странное ") +
    uT("поведение</i> животных, но их <u>тревога нарастает</u>. Мы могли бы ") +
    uT("объединиться и продолжить исследование вместе, что скажешь?")));

  using namespace ::covellite;

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_evil")]
    .Connect([&](void) 
  { 
    GetElement("id_status").SetStyle(u8"color: #a50606;");
    GetElement("id_status").SetText(u8"рассержен");
    GetElement("id_text").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("<big>Э</big>й, что ты делаешь?")));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_neutral")]
    .Connect([&](void)
  {
    GetElement("id_status").SetStyle(u8"color: #808080;");
    GetElement("id_status").SetText(u8"нейтрален");
    GetElement("id_text").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("<big>Н</big>у нет, так нет.")));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_good")]
    .Connect([&](void)
  {
    GetElement("id_status").SetStyle(u8"color: #018f10;");
    GetElement("id_status").SetText(u8"добродушен");
    GetElement("id_text").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("<big>О</big>тлично, идем!")));
  });
}
