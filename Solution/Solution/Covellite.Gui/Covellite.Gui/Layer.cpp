
#include "stdafx.h"
#include <Covellite/Gui/Layer.hpp>
#include <boost/format.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string-cast.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include <Covellite/Core/EventHandler.hpp>
#include <Rocket/ProgressBar/ElementProgressBar.h>

#ifdef max
# undef max
#endif

using namespace covellite::gui;

Layer::Element::Element(Element_t * _pElement) :
  m_pElement(_pElement)
{
  if (m_pElement == nullptr) throw STD_EXCEPTION << "Element null pointer.";
}

/**
* \brief
*  Функция установки фокуса ввода текущему элементу.
*/
void Layer::Element::SetFocus(void)
{
  m_pElement->Focus();
}

/**
* \brief
*  Функция изменения значения элемента слоя.
* \details
*  - Для текстовых полей ввода (\b textarea и \b input.text) функция заменяет
*  текст, который эти элементы содержат.
*  - Для \b input.range устанавливается новое (числовое) значение ползунка.
*  - Для всех остальных элементов - дочерний раздел .rml файла (таким образом 
*  можно динамически менять содержимое окна).
*  - Нужно учиывать, что изменение текста внутри тегов (даже \<p\>...\</p\>)
*  приведет к пересозданию всего или части документа, что обычно сопровождается
*  заметным пользователю подвисанием работы программы на доли секунды, поэтому
*  для вывода динамически меняющегося текста следует использовать текстовое
*  поле ввода с установленным ему атрибутом \b disabled.
*
* \param [in] _Meaning
*  Новое значение элемента.
*/
void Layer::Element::SetMeaning(const String_t & _Meaning)
{
  const auto Tag =
    m_pElement->GetTagName();
  const auto Type =
    m_pElement->GetAttribute("type", Rocket::Core::String{ "unknown" });

  using namespace ::alicorn::extension::std;

  // 06 Февраль 2019 13:27 (unicornum.verum@gmail.com)
  TODO("Проверить работу функции для textarea");

  // 05 Март 2019 12:42 (unicornum.verum@gmail.com)
  TODO("Тест для progressbar и circularbar.");

  if (Tag == "textarea" ||
    Tag == "progressbar" ||
    Tag == "circularbar" ||
    (Tag == "input" && (Type == "range" || Type == "text")))
  {
    auto & Control =
      dynamic_cast<Rocket::Controls::ElementFormControl &>(*m_pElement);

    // Для select это не работает (как и ElementFormControlSelect::SetSelection(),
    // выглядит так, как будто список строк просто удаляется), как устанавливать 
    // значение по умолчанию, см. в примере работы с элементами управления.

    Control.SetValue(string_cast<::std::string, Locale::UTF8>(_Meaning).c_str());
    return;
  }

  m_pElement->SetInnerRML(
    string_cast<::std::string, Locale::UTF8>(_Meaning).c_str());
}

/**
* \brief
*  Функция получения значения элемента слоя.
* \details
*  - Для текстовых полей ввода (\b textarea и \b input.text) функция возвращает
*  текст, который эти элементы содержат.
*  - Для \b input.range возвращается (числовое) значение ползунка.
*  - Для \b select возвращается текст выбранной строки.
*  - Для всех остальных элементов - дочерний раздел .rml файла.
*
* \return
*  Текущее значение элемента.
*/
auto Layer::Element::GetMeaning(void) const -> String_t
{
  const Rocket::Core::String Unknown = "unknown";

  const auto Tag = m_pElement->GetTagName();
  const auto Type = m_pElement->GetAttribute("type", Unknown);

  using namespace ::alicorn::extension::std;

  // 06 Февраль 2019 13:27 (unicornum.verum@gmail.com)
  TODO("Проверить работу функции для textarea");

  if (Tag == "textarea" || Tag == "select" ||
    (Tag == "input" && (Type == "text" || Type == "range")))
  {
    const auto & Control =
      dynamic_cast<Rocket::Controls::ElementFormControl &>(*m_pElement);

    return string_cast<String, Locale::UTF8>(Control.GetValue().CString());
  }

  return string_cast<String, Locale::UTF8>(
    m_pElement->GetInnerRML().CString());
}

/**
* \brief
*  Функция установки класса стиля элемента.
* \details
*  - Устанавливается класс стиля, который должен быть предварительно описан
*  в .rcss файле (с точкой).
*  - Для скрытия элемента установить ему класс, у которого задан параметр
*  'visibility: hidden;'.
*
* \param [in] _Class
*  Имя класса (без точки).
*/
void Layer::Element::SetClassStyle(const String_t & _Class)
{
  using namespace ::alicorn::extension::std;

  m_pElement->SetAttribute("class", 
    string_cast<::std::string, Locale::UTF8>(_Class).c_str());
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо нее
*  следует использовать функцию SetMeaning().
* \brief
*  Функция изменения текста дочернего элемента управления слоя.
* \details
*  - Для текстовых полей ввода функция заменяет текст, который эти элементы
*  содержат, для всех остальных элементов - дочерний раздел .rml файла
*  (таким образом можно динамически менять содержимое окна).
*  - Нужно учиывать, что изменение текста внутри тегов (даже \<p\>...\</p\>)
*  приведет к пересозданию всего или части документа, что обычно сопровождается
*  заметным пользователю подвисанием работы программы на доли секунды, поэтому
*  для вывода динамически меняющегося текста следует использовать текстовое
*  поле ввода с установленным ему атрибутом \b disabled.
*  
* \param [in] _Text
*  Новое значение текста.
*/
void Layer::Element::SetText(const Utf8String_t & _Text)
{
  const auto Tag =
    m_pElement->GetTagName();
  const auto Type =
    m_pElement->GetAttribute("type", Rocket::Core::String{ "unknown" });

  if (Tag == "textarea" ||
    (Tag == "input" && Type == "text"))
  {
    m_pElement->SetAttribute("value", _Text.c_str());
    return;
  }

  m_pElement->SetInnerRML(_Text.c_str());
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо нее
*  следует использовать функцию GetMeaning().
* \brief
*  Функция получения текста дочернего элемента управления слоя.
* \details
*  - Для текстовых полей ввода функция возвращает текст, который эти элементы
*  содержат, для всех остальных элементов - дочерний раздел .rml файла.
*  
* \return
*  Текст элемента, заданный в .rml файле как атрибут 'value'.
*/
Layer::Utf8String_t Layer::Element::GetText(void) const
{
  const Rocket::Core::String Unknown = "unknown";

  const auto Tag = m_pElement->GetTagName();
  const auto Type = m_pElement->GetAttribute("type", Unknown);

  if (Tag == "textarea" ||
    (Tag == "input" && Type == "text"))
  {
    return m_pElement->GetAttribute("value", Unknown).CString();
  }

  return m_pElement->GetInnerRML().CString();
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо нее
*  следует использовать функцию SetClassStyle().
* \brief
*  Функция установки стиля элемента.
* \details
*  - Устанавливается атрибут style, заменяя существующее значение из файла .rml.
*  - Установленные таким образом параметры стиля заменят собой параметры,
*  заданные в файле для этого элемента в файле .rcss.
*  
* \param [in] _Text
*  Строка стиля.
*/
void Layer::Element::SetStyle(const Utf8String_t & _Text)
{
  m_pElement->SetAttribute("style", _Text.c_str());
}

// ************************************************************************** //

/**
* \brief
*  Конструктор создания простого слоя.
*
* \param [in] _Window
*  Объект родительского окна слоя (передается в функции создания слоя, поэтому
*  класс-наследник должен в конструкторе получать этот объект и передавать
*  его родителю).
* \param [in] _PathToFile
*  Путь к rml файлу, из которого должен загружаться слой (рекомендуется
*  из класса-наследника конкретного слоя передавать конкретный путь, содержащий
*  описание этого слоя).
* \note
*  В силу специфики работы libRocket в пути к корневой папке программы
*  допустимы символы системного языка операционной системы, в путях же внутри
*  этой папки следует использовать исключительно латиницу.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
Layer::Layer(::covellite::gui::IWindow & _Window, const Path_t & _PathToFile) :
  m_Events(_Window),
  m_pDocument(_Window.LoadDocument(Convert(_PathToFile).c_str()))
{
  if (m_pDocument == nullptr)
  {
    throw STD_EXCEPTION << "Document null pointer: " << _PathToFile;
  }
}

/**
* \brief
*  Конструктор создания слоя с заголовком.
* \details
*  - Документ libRocket может содержать заголовок окна, для примера объявления
*  заголовка ниже в качестве _TitleId следует передать "title", тогда
*  в качестве текста заголовка будет выведено "Layer example".
*
* \code

<rml>
<head>
<title>Layer example</title>
...

* \endcode
*
* \param [in] _pContext
*  Объект контекста окна libRocket.
* \param [in] _PathToFile
*  Путь к rml файлу, из которого должен загружаться слой.
* \param [in] _TitleId
*  Идентификатор заголовка, указанного в .rml файле.
*
* \exception std::exception
*  - Заголовок с указанным идентификатором не найден.
*  - Действие невозможно (подробнее см. описание исключения).
*/
Layer::Layer(::covellite::gui::IWindow & _Window, const Path_t & _PathToFile,
  const ::std::string & _TitleId) :
  Layer(_Window, _PathToFile)
{
  auto * pTitle = m_pDocument->GetElementById(_TitleId.c_str());
  if (pTitle == nullptr)
  {
    throw STD_EXCEPTION << "Unexpected title id: " << _TitleId;
  }

  pTitle->SetInnerRML(m_pDocument->GetTitle());
}

/**
* \deprecated
*  Конструктор устарел и будет удален в следующей стабильной версии, следует
*  использовать функцию PushLayer() для создания слоев и конструкторы, 
*  получающие интерфейс окна Rocket.
* \brief
*  Конструктор создания простого слоя.
*  
* \param [in] _pContext
*  Объект контекста окна libRocket.
* \param [in] _PathToFile
*  Путь к rml файлу, из которого должен загружаться слой (рекомендуется
*  из класса-наследника конкретного слоя передавать конкретный путь, содержащий
*  описание этого слоя).
* \note
*  В силу специфики работы libRocket в пути к корневой папке программы 
*  допустимы символы системного языка операционной системы, в путях же внутри 
*  этой папки следует использовать исключительно латиницу.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
Layer::Layer(Context_t * _pContext, const Path_t & _PathToFile) :
  m_pDocument(_pContext->LoadDocument(Convert(_PathToFile).c_str()))
{
  if (m_pDocument == nullptr)
  {
    throw STD_EXCEPTION << "Document null pointer: " << _PathToFile;
  }
}

/**
* \deprecated
*  Конструктор устарел и будет удален в следующей стабильной версии, следует
*  использовать функцию PushLayer() для создания слоев и конструкторы,
*  получающие интерфейс окна Rocket.
* \brief
*  Конструктор создания слоя с заголовком.
* \details
*  - Документ libRocket может содержать заголовок окна, для примера объявления
*  заголовка ниже в качестве _TitleId следует передать "title", тогда
*  в качестве текста заголовка будет выведено "Layer example".
*
* \code

<rml>
<head>
<title>Layer example</title>
...

* \endcode
*
* \param [in] _pContext
*  Объект контекста окна libRocket.
* \param [in] _PathToFile
*  Путь к rml файлу, из которого должен загружаться слой.
* \param [in] _TitleId
*  Идентификатор заголовка, указанного в .rml файле.
*
* \exception std::exception
*  - Заголовок с указанным идентификатором не найден.
*  - Действие невозможно (подробнее см. описание исключения).
*/
Layer::Layer(Context_t * _pContext, const Path_t & _PathToFile, 
  const ::std::string & _TitleId) :
  Layer(_pContext, _PathToFile)
{
  auto * pTitle = m_pDocument->GetElementById(_TitleId.c_str());
  if (pTitle == nullptr)
  {
    throw STD_EXCEPTION << "Unexpected title id: " << _TitleId;
  }

  pTitle->SetInnerRML(m_pDocument->GetTitle());
}

Layer::~Layer(void) noexcept
{
  m_pDocument->RemoveReference();
}

/**
* \brief
*  Функция активации слоя.
*/
void Layer::Show(void) /*override*/
{
  m_pDocument->Show();
}

/**
* \brief
*  Функция скрытия слоя.
*/
void Layer::Hide(void) /*override*/
{
  m_pDocument->Hide();
}

/**
* \brief
*  Функция получения идентификатора документа.
* \details
*  - Идентификатором документа является значение атрибута \b id тега \n body
*  указанного в конструкторе .rml файла.
*
* \return
*  Идентификатор документа.
*/
auto Layer::GetId(void) const -> DocumentId_t
{
  return m_pDocument->GetId().CString();
}

/**
* \brief
*  Функция получения объекта дочернего элемента.
*  
* \param [in] _Id
*  Идентификатор искомого объекта.
*  
* \exception std::exception
*  - Дочернего элемента с указанным идентификатором не существует.
*/
Layer::Element Layer::GetElement(const ElementId_t & _Id) const
{
  return m_pDocument->GetElementById(_Id.c_str());
}

int Layer::GetWidth(void) const
{
  auto * pContext = m_pDocument->GetContext();
  if (pContext == nullptr) throw STD_EXCEPTION << "Null pointer.";

  return pContext->GetDimensions().x;
}

int Layer::GetHeight(void) const
{
  auto * pContext = m_pDocument->GetContext();
  if (pContext == nullptr) throw STD_EXCEPTION << "Null pointer.";

  return pContext->GetDimensions().y;
}

/**
* \brief
*  Функция установки базового размера шрифта всего документа.
* \details
*  - Функция устанавливает элементу 'body' документа значение размера шрифта
*  в процентах от длинной строны экрана (это можно использовать для создания
*  квадратных элементов, размеры которых будут меняться при изменении размеров
*  экрана устройства (размер задавать как 10em, например) пропорционально
*  рамерам экрана).
*  - Это значение перекроет собой значение, заданное в файле .rcss для элемента
*  'body'.
*
* \param [in] _PercentFromMaxScreenSize
*  Размер шрифта в процентах от длинной стороны экрана.
*
* \return
*  Размер установленного шрифта в пикселях.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
float Layer::EmployFontSize(float _PercentFromMaxScreenSize)
{
  if (m_pDocument->GetTagName() != u8"body")
  {
    throw STD_EXCEPTION << "Unexpected document tag name: "
      << m_pDocument->GetTagName().CString();
  }

  const auto FontSize = ::std::max(GetWidth(), GetHeight()) * 
    _PercentFromMaxScreenSize / 100.0f;

  m_pDocument->SetAttribute("style", 
    (::boost::format("font-size: %1%px;") % FontSize).str().c_str());
  return FontSize;
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо нее
*  следует использовать функцию EmployFontSize().
* \brief
*  Функция установки базового размера шрифта всего документа.
* \details
*  - Функция устанавливает элементу 'body' документа значение размера шрифта
*  в процентах от длинной строны экрана (это можно использовать для создания
*  квадратных элементов, размеры которых будут меняться при изменении размеров
*  экрана устройства (размер задавать как 10em, например) пропорционально
*  рамерам экрана).
*  - Это значение перекроет собой значение, заданное в файле .rcss для элемента
*  'body'.
*
* \param [in] _PercentFromMaxScreenSize
*  Размер шрифта в процентах от длинной стороны экрана.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
void Layer::SetFontSize(double _PercentFromMaxScreenSize)
{
  EmployFontSize(static_cast<float>(_PercentFromMaxScreenSize));
}
