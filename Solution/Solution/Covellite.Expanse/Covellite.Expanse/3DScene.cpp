
#include "stdafx.h"
#include "3DScene.impl.hpp"
#include <alicorn/std/exception.hpp>

using namespace ::covellite::expanse;

/**
* \brief
*  ������� ���������� 3D ������� � 3D �����.
* \details
*  - ����� ���������� ������������� ������� ����� ����� ��������� � �������
*  ����������, ����� ��� ������ ������� ���������� ����� ������� ��� �������.
*  
* \param [in] _Id
*  ������������� �������.
* \param [in] _Object
*  ����� �������� 3D �������.
*/
void C3DScene::Add(const ObjectId_t _Id, const C3DObject_t & _Object) /*override*/
{
  if (_Id >= m_Renders.size())
  {
    m_Renders.resize(_Id + 1,
      { [](void) { throw STD_EXCEPTION << "Using render for not created object."; } });
  }

  m_Renders[_Id] = _Object;
}

/**
* \brief
*  ������� �������� 3D ������� �� 3D c����.
* \details
*  - ��� ���������� �������������� ���������� ������� � �������
*  ���������� ��� ������� ����������� ����� ����� ������� ����������.
*  
* \param [in] _Id
*  ������������� ����� ���������� �������.
*/
void C3DScene::Remove(const ObjectId_t _Id) /*override*/
{
  if (_Id >= m_Renders.size()) return;

  m_Renders[_Id] =
  {
    [=](void)
    {
      throw STD_EXCEPTION << "Using render for removed object: " << _Id;
    }
  };
}

/**
* \brief
*  ������� ���������� �������������� ������� � ������� ���������� ����������
*  �������.
* \details
*  - ���������������, ��� ��������� ����� ������������� � ��������� ��������
*  (� ������� ����������� �� �������), ������ �� ������� ������������ �����
*  ������� ����������, � ������� ������� ������������� � ������� �����������
*  �� �����.
*
* \param [in] _Pass
*  ����� �������.
* \param [in] _Id
*  ������������� �������.
* \param [in] _HashForSort
*  ��� ��� ����������.
*/
void C3DScene::Add(
  const size_t _Pass,
  const ObjectId_t _Id,
  const size_t _HashForSort) /*override*/
{
  if (_Pass >= m_RenderQueue.size()) m_RenderQueue.resize(_Pass + 1);

  // ���� ������������ vector<Object_t>, �� ������� ���������� ������� � �����
  // �������� ������� �������, �� ��� ������������� ���������� ~500 �������
  // �������� � ����� - ��������� ������� ���������� ��������, � ��� 50�
  // �������� �� �������� 2013-�� ���� ������� ��� ������� �������� ~33%
  // ������� ��� ���������� ������ �������� (��� ���� Pass.clear() �������� 10%).
  // ������� 2016-�� ���� ������� ������� ����� ���� � 2 ����.
  m_RenderQueue[_Pass].emplace(Object_t{ _HashForSort, _Id });
}

/**
* \brief
*  ������� ���������� 3D �����.
* \details
*  - ������� ������������� ��� ���������� ���� ��������, ��������������
*  ����������� � ������� ����������.
*  - ����� ���������� ���������� ����� ��� ������� ����� �������, �������
*  � ������ ������� ����� �� ������� ��������� ������.
*  
* \exception std::exception
*  - ������� ���������� ��������������� ��� ���������� �������.
*/
void C3DScene::Render(void)
{
  for (auto & Pass : m_RenderQueue)
  {
    for (const auto & Object : Pass)
    {
      for (const auto & Render : Get(Object.second))
      {
        Render();
      }
    }

    Pass.clear();
  }
}

/**
* \brief
*  ������� ��������� ������ �������� 3D ������� �� ��� ��������������.
*  
* \param [in] _Id
*  ������������� �������.
*  
* \exception std::exception
*  - ������ � ��������� ��������������� �� ��� �������� � 3D �����.
*/
const C3DObject_t & C3DScene::Get(const ObjectId_t _Id) const
{
  if (_Id >= m_Renders.size())
  {
    throw STD_EXCEPTION << "Unexpected object id: " << _Id;
  }

  return m_Renders[_Id];
}
