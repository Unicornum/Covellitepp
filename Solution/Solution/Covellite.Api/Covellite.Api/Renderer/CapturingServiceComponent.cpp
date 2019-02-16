
#include "stdafx.h"
#include "CapturingServiceComponent.hpp"

using namespace covellite::api::renderer;

/**
* \brief
*  ������� ���������� ���������� � ������� ��������������� �����������.
*  
* \param [in] _pComponent
*  ����������� ���������.
*/
void CapturingServiceComponent::Add(const ComponentPtr_t & _pComponent)
{
  m_Components.push_back(_pComponent);
}

/**
* \brief
*  ������� ��������� ������ ��������������� �����������.
* \details
*  - ������� ������������� ��� ��������� ������, �������������� �����������
*  ��������� ���������� ��������������� ���������.
*  - ��������� ���������� (����������� � �������� �����) ��������� �� �������.
*  - ����� ������ ������� ������� ����������� ����� ������.
*  
* \param [in] _Expected
*  ���������� �� ��������� ������ �����������, � ������� ������ �������
*  ������ ��������� �������� \b kind �������� ���������� � ���������, �������
*  ����� ������� � �������� �����, ���� ���������� � ��������� \b kind
*  � ������� ��������������� ����������� �� ����������.
*  
* \return
*  ����� �����������, � �������:
*  - ���������� ����������� � �������� ��������� � ����������� ���������
*  �������� ������.
*  - ���������� ����������� � ��� ������� �������� \b kind, ��� � �� �������
*  ������.
*/
auto CapturingServiceComponent::Get(const ::std::vector<Expected_t> & _Expected) -> ::std::vector<ComponentPtr_t>
{
  ::std::vector<ComponentPtr_t> Result;

  for (const auto & Info : _Expected)
  {
    auto itResult = ::std::find_if(m_Components.rbegin(), m_Components.rend(),
      [&](const ComponentPtr_t & _pComponent) 
        { return Info.first == _pComponent->Kind; });

    if (itResult != m_Components.rend())
    {
      Result.push_back(*itResult);
      m_Components.erase(::std::next(itResult).base());
    }
    else
    {
      Result.push_back(Info.second);
    }
  }

  if (!m_Components.empty())
  {
    // 14 ������� 2019 16:29 (unicornum.verum@gmail.com)
    TODO("������ � ��� warning'� � ������ �����������.");

    m_Components.clear();
  }

  return Result;
}

/**
* \brief
*  ������� ��� ���������������� ��������� ������� ��������������� �����������.
* \details
*  - ������� �������� �� ������� ��������������� ����������� � ��� ������� 
*  ���������� �������� ��������� ��� ���� (�� ��������� \b kind) ����������,
*  ��������� ��� � �������� ��������� ���� ���������.
*  - ����������� ����� ���������� � ��� �������, � ������� ��������������� 
*  ���������� ���� ��������� � �������.
*  - ����� ������ ������� ������� ����������� ����� ������.
*  
* \param [in] _PreRenders
*  �������� ����� ������������ �����������.
*/
void CapturingServiceComponent::Process(const Handlers_t & _PreRenders)
{
  for (; !m_Components.empty(); m_Components.pop_front())
  {
    const auto pComponent = m_Components.front();

    auto itComponent = _PreRenders.find(pComponent->Kind);
    if (itComponent != _PreRenders.end())
    {
      itComponent->second(pComponent);
    }
    else
    {
      // 13 ������� 2018 14:22 (unicornum.verum@gmail.com)
      TODO("������ � ��� warning � ������ ����������.");
    }
  }
}
