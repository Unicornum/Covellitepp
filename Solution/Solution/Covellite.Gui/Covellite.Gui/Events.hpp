
#pragma once
#include <string>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Базовый класс для событий панели GUI.
*  
* \version
*  1.0.0.0        \n
* \date
*  04 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Gui
{
public:
  using TypeId_t = ::std::string;

  /**
  * \brief
  *  Вспомогательный класс для создания уникального идентификатора элемента.
  */
  class Document
  {
  public:
    /**
    * \brief
    *  Вспомогательный класс для создания уникального идентификатора элемента.
    */
    class Element
    {
    public:
      const TypeId_t m_Id;

    public:
      explicit Element(const TypeId_t & _Id) : m_Id(_Id) { }
    };

  public:
    Element ElementId(const TypeId_t & _Id) const
    {
      return Element{ m_Id + "::" + _Id };
    }

  public:
    const TypeId_t m_Id;

  public:
    explicit Document(const TypeId_t & _Id) : m_Id(_Id) { }
  };

public:
  /**
  * \brief
  *  Вспомогательная функция для построения уникального идентификатора элемента.
  * \details
  *  Идентификатор элемента события клика мышью строится как
  *  events::Click.DocumentId(Layer::GetId()).ElementId("id_button_back").
  */
  Document DocumentId(const TypeId_t & _Id) const
  {
    return Document{ m_EventType + "::" + _Id };
  }

public:
  const TypeId_t m_EventType;

public:
  explicit Gui(const TypeId_t & _EventType) : m_EventType(_EventType) { }
};

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс событий клика мышью по элементу управления.
*  
* \version
*  1.0.0.0        \n
* \date
*  27 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Click_t final :
  public Gui
{
public:
  Click_t(void) : Gui("click") {}
};

namespace { Click_t Click; }

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс событий изменения состояния элемента управления.
*
* \version
*  1.0.0.0        \n
* \date
*  04 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Change_t final :
  public Gui
{
public:
  Change_t(void) : Gui("change") {}
};

namespace { Change_t Change; }

} // namespace events

} // namespace covellite

namespace std
{

template<>
struct hash<::covellite::events::Gui::Document::Element>
{
  using Element_t = ::covellite::events::Gui::Document::Element;

public:
  inline size_t operator()(const Element_t & _Id) const
  {
    return ::std::hash<::covellite::events::Gui::TypeId_t>{}(_Id.m_Id);
  }
};

}
