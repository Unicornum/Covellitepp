
#include "stdafx.h"
#include <Covellite/Gui/Layer.hpp>
#include <boost/format.hpp>
#include <Covellite/Core/EventHandler.hpp>

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
  using String_t = Rocket::Core::String;

  const String_t Unknown = "unknown";

  const auto Tag = m_pElement->GetTagName();
  const auto Type = m_pElement->GetAttribute("type", Unknown);

  if (Tag == "textarea" ||
    (Tag == "input" && Type == "text"))
  {
    m_pElement->SetAttribute("value", Rocket::Core::String(_Text.c_str()));
  }
  else
  {
    m_pElement->SetInnerRML(Rocket::Core::String(_Text.c_str()));
  }
}

/**
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
  using String_t = Rocket::Core::String;

  const String_t Unknown = "unknown";

  const auto Tag = m_pElement->GetTagName();
  const auto Type = m_pElement->GetAttribute("type", Unknown);

  if (Tag == "textarea" ||
    (Tag == "input" && Type == "text"))
  {
    return m_pElement->GetAttribute("value", Unknown).CString();
  }

  String_t Result;
  m_pElement->GetInnerRML(Result);
  return Result.CString();
}

/**
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

/**
* \brief
*  Функция установки фокуса ввода текущему элементу.
*/
void Layer::Element::SetFocus(void)
{
  m_pElement->Focus();
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
  m_pDocument(_Window.LoadDocument(Convert(_PathToFile)))
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
  m_pDocument(_pContext->LoadDocument(Convert(_PathToFile)))
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
* \param [in] _Percent
*  Размер шрифта в процентах от длинной стороны экрана.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
void Layer::SetFontSize(double _Percent)
{
  if (m_pDocument->GetTagName() != u8"body")
  {
    throw STD_EXCEPTION << "Unexpected document tag name: "
      << m_pDocument->GetTagName().CString();
  }

  const auto Height = ::std::max(GetWidth(), GetHeight());

  Element(m_pDocument).SetStyle(
    (::boost::format("font-size: %1%px;") % (_Percent * Height / 100.0)).str());
}
