
#include "stdafx.h"
#include "Text.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

namespace layers
{

const auto TextGameLayerDescription =
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
  Layer(_Window, "text.rml", uT("Текст в разных форматах"), TextGameLayerDescription)
{
  using namespace ::alicorn::extension::std;

  GetElement("id_main_text").SetMeaning(
    uT("<big>Д</big>а, мы слышали о проблемах на побережье и поэтому меня ") +
    uT("отправили на разведку. До сих пор я видел только <i>странное ") +
    uT("поведение</i> животных, но их <u>тревога нарастает</u>. Мы могли бы ") +
    uT("объединиться и продолжить исследование вместе, что скажешь?"));

  namespace events = ::covellite::events;

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_evil")]
    .Connect([&](void)
  {
    GetElement("id_status").SetClassStyle(uT("evil"));
    GetElement("id_status").SetMeaning(uT("рассержен"));
    GetElement("id_main_text").SetMeaning(uT("<big>Э</big>й, что ты делаешь?"));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_neutral")]
    .Connect([&](void)
  {
    GetElement("id_status").SetClassStyle(uT("neutral"));
    GetElement("id_status").SetMeaning(uT("нейтрален"));
    GetElement("id_main_text").SetMeaning(uT("<big>Н</big>у нет, так нет."));
  });

  m_Events[events::Click.DocumentId(GetId()).ElementId("id_good")]
    .Connect([&](void)
  {
    GetElement("id_status").SetClassStyle(uT("good"));
    GetElement("id_status").SetMeaning(uT("добродушен"));
    GetElement("id_main_text").SetMeaning(uT("<big>О</big>тлично, идем!"));
  });
}

} // namespace layers
