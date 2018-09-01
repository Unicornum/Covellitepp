
#pragma once
#include <string>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteGuiPage \n
*  ����� ������� ����� ����� �� �������� ����������.
*  
* \version
*  1.0.0.0        \n
* \date
*  27 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Click_t final
{
public:
  using TypeId_t = ::std::string;

  /**
  * \brief
  *  ��������������� ����� ��� �������� ����������� �������������� ��������.
  */
  class Document
  {
  public:
    /**
    * \brief
    *  ��������������� ����� ��� �������� ����������� �������������� ��������.
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
  *  ��������������� ������� ��� ���������� ����������� �������������� ��������.
  * \details
  *  ������������� �������� ������� ����� ����� �������� ���
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
  *  ����� ������ � ������ \ref CovelliteGuiPage \n
  *  �����, ���������� ���������� �� ��������, ��� �������� ���� �������������
  *  ������� ����� �����.
  */
  class Info
  {
  public:
    ::std::string Tag;  ///< �������� xml ���� ��������.
    ::std::string Type; ///< ��� �������� (�������� �������� \b type).
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
