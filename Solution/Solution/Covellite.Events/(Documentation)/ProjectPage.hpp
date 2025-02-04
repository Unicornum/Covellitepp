
/**
* \page CovelliteEventsBriefPage
*  ������ ��� ���� ������ � ��������� ����������.
*/

/**
* \page CovelliteEventsPage Covellite.Events
*  
*  ������ ������� ������� \ref CovelliteEventsGroup
*  
*  ��������
*  --------
*  
* \copydoc CovelliteEventsBriefPage
*  
*  ���������
*  ---------
*  
*  ������ ������� ������������ ���������� ��������� � �� ������������� ��
*  ����, � ����� ������������ ������� �������� ����������.
*  
* \copydoc CovelliteUsingEventsPage
*/

/**
* \page CovelliteUsingEventsPage ������������� ������� ����������
*
* ### �������� ������ ���� �������
*
*  ��� �������� ����������������� ���� ������� ������� ������� ����� ���� 
*  ������� (� ������� ���������� �������������� ������� ����� ����) � �������� 
*  ������ ������ ���� ������� (��� �������� ������������� � ���� �� ������ 
*  ����� ���������� �����-��������� ����������, ������������ ����� �������).
*
* \code

// ����� ���� �������.
class UserType_t final
{
public:
  // �������������� �������.
  enum Id
  {
    Error = 0,
  };
    
public:
  // ����� ���������� ������� Error.
  class Error final
  {
  public:
    int Code;
    ::std::string Description;
  };
};
  
// ��������������� ������ ��� ������������� ������� UserType.Error (����� 
// �������� ����������� ������������ �������, ������� ���������� 
// �����/��������� �������� � ������ �������, ���� �� ������������ ������
// UserType_t::Error, ��� ������� ����� ��������������� ���� ������������ � 
// �� ����� �������� ����������).
namespace { UserType_t UserType; }

* \endcode
*  
* \note
*  ������ ���� ������� ����� ����� ���������� �������������� (��� �����, ���
*  � ��������), ������� � ������ ���������������� ����� ��������� ������� 
*  ���������.
*  
* ### �������� ������, ������� ������ ����������� �� �������
*  
*  ������, ������� ������ ����������� �� �������, ���������� �������� ������ 
*  ������ covellite::events::Events � ������������������� ��� � ������������,
*  ���������� �������� ����� ���� ���� �� ������������ ������� ��������
*  (covellite::app::IApplication, covellite::os::IWindow, 
*  covellite::api::IWindow, covellite::rocket::IWindow).
*  
*  � ������ ������������� '�������� ������' ������ ������� (��������, ����
*  ��������� �������� ����������� ��� ����� ������ ������������ Api � �����
*  GUI, ������� ������ ������������ ���-���� ����� ���������� GUI), ����� 
*  c������ ������������ �� covellite::events::IEvents � ����������� ��� 
*  ����������� ��������, ������ ������ �������.
*  
* \note
*  ������ ������� � ������ ������ ��� ��������� � ��� ���������� � ���������
*  �� ��� ����������� ������� ������ (� ������ ������������� ��������� ������
*  ������ ��������� ������� ������� ������������ ������ ������� � ������� �����,
*  ��������� ��� ����� ����������� �������).
*
* \code

class Window final
{
private:
  covellite::events::Events m_Events;
    
public:
  explicit Window(const covellite::api::IWindow & _Window) :
    m_Events(_Window)
  {
    m_Events[UserType.Error].Connect([](const UserType_t::Error &)
    {
      // ���� ��������� ��� ��������� ������� UserType.Error.
    });
  }
};

* \endcode
*  
* ### ��������� �������
*  
*  ��� ��������� ������� ���������� ������� �������� () �� ������� ������ 
*  ������, ����������� ������ ������ �������. � �������� ��������� (�������
*  ������������ ������ ����) ����� ���� ������� ������ ������ ������
*  (��� ������ ��������� � �����, ��������� � �����������, � ��������� ������
*  ����� ������������� ����������).
*  
* \code

covellite::api::Window ApiWindow{ ... };
Window Window{ ApiWindow };
  
const UserType_t::Error Description = { 5, "Error description" }.
  
Window[UserType.Error](Description);

* \endcode
*  
* ### ������������
*  
*  - ������� ����� ������� ������ ���������� ������������� ��� �����������
*  ������ ������ ���������� � �� ������ �������������� �� ����������� ����
*  (���������� ��� ����� ������ ������������� � ����������� �� ���,
*  �� ������ �����������, ��������, ������� ���������� ������ ��������� 
*  Application.Exit), ��������� ��. �������� ���������� 
*  [�������](\ref CovelliteEventsIdGroup).
*  - ����������� �� ����������� ������� ������������ ������ �������, ������
*  ��������� ��� ���� �������� ������������ ��������� (��������, ����� 
*  ���������� ������� ���������� �� �� ����� �������), ������������� �� 
*  ����������� ������� ������������ ������� ����� �������� ������.
*  - ��������� ������� ���������� ����������� � �������������, ��� ������
*  ��������� ����� (����, �����-��������� �� covellite::rocket::Layer) 
*  ��������� ����� �������� ��������� \b View � ������ �������� MVC �
*  ����� �������� ����������� ��� ������ ����� \b Model, ��� �� ����� 
*  (��� ��������-����, ��������) �������� �������� ���������� ����������� 
*  ���������� ��� ����� ��������� ���������, � ������� ��� ������ ����� 
*  ������������ � �������� ����������.
*/
