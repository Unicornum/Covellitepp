
#include "stdafx.h"
#include "3DScene.impl.hpp"
#include <alicorn/std/exception.hpp>

using namespace ::covellite::expanse;

C3DScene::C3DScene(void)
{
  m_RenderQueue.reserve(1000u);
}

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
* \param [in] _Id
*  ������������� �������.
*/
void C3DScene::Add(const ObjectId_t _Id) /*override*/
{
  m_RenderQueue.push_back(_Id);
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
  for (const auto & Id : m_RenderQueue)
  {
    for (const auto & Render : Get(Id))
    {
      Render();
    }
  }

  m_RenderQueue.clear();
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
