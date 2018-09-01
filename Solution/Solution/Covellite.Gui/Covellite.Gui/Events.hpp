
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
class Click_t final
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
    return Document{ _Id };
  }

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  Класс входит в проект \ref CovelliteGuiPage \n
  *  Класс, содержащий информацию об элементе, для которого было сгенерировано
  *  событие клика мышью.
  */
  class Info
  {
  public:
    ::std::string Tag;  ///< Название xml тега элемента.
    ::std::string Type; ///< Тип элемента (значение атрибута \b type).
  };
};

namespace { Click_t Click; }

} // namespace events

} // namespace covellite

namespace std
{

template<>
struct hash<::covellite::events::Click_t::Document::Element>
{
  using Element_t = ::covellite::events::Click_t::Document::Element;

public:
  inline size_t operator()(const Element_t & _Id) const
  {
    return ::std::hash<::covellite::events::Click_t::TypeId_t>{}(_Id.m_Id);
  }
};

}
