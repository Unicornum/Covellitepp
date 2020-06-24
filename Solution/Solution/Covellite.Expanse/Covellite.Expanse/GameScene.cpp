
#include "stdafx.h"
#include "GameScene.impl.hpp"
#include <Covellite/Api/Renders.hpp>
#include "IProduce3DObject.impl.hpp"

using namespace ::covellite::expanse;

/**
* \brief
*  ����������� ������.
*  
* \param [in] _pRenders
*  ������, ���������� �� \ref CovelliteApiPage ��� �������� ������� ��������
*  3D �������� �� ������� ����������� ������� ��������.
* \param [in] _Produce3DObject
*  ������ ��� ���������� � �������� 3D �������� � 3D c����.
*/
GameScene::GameScene(
  const RendersPtr_t & _pRenders, 
  IProduce3DObject & _Produce3DObject) noexcept :
  m_pRenders(_pRenders),
  m_Produce3DObject(_Produce3DObject)
{

}

GameScene::~GameScene(void)
{
  for (size_t i = 0; i < m_GameObjects.size(); i++)
  {
    if (!m_GameObjects[i].empty())
    {
      LOGGER(Warning) << "GameScene: game object was not deleted [id=" << i << "]:";

      for (const auto & pComponent : m_GameObjects[i])
      {
        LOGGER(Warning) << "  > component id=" << pComponent->Id << " type=" << pComponent->Type;
      }
    }
  }
}

/**
* \brief
*  ������� �������� �������.
* \details
*  - ��� ���������� �������� ������� ����� ������ ����� �������� 3D ������� �
*  ��������� ���������� �������������, ������� ����� ������������ ��� ����������
*  � ������� ����������.
*  - ���� � �������� ������ ����������� ����� ��������� \b Updater, ��� ����
*  ����� ������ ��������� ������, ������� ����� ���������� ��� ���������� �����
*  ����� ����������� �����.
*  
* \param [in] _Object
*  ����� ����������� �������� �������.
*  
* \return
*  ������������� ���������� �������.
*  
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
ObjectId_t GameScene::CreateObject(const GameObject_t & _Object) /*override*/
{
  const auto Id = m_Id.GetFreeId();

  if (Id >= m_GameObjects.size())
  {
    m_GameObjects.resize(Id + 1);
  }

  C3DObject_t o3DObject;

  for (const auto & pComponent : _Object)
  {
    if (pComponent->Type == uT("Updater"))
    {
      m_Updaters.push_back({ Id, m_pRenders->Obtain(pComponent) });
    }
    else
    {
      o3DObject.push_back(m_pRenders->Obtain(pComponent));
    }
  }

  m_Produce3DObject.Add(Id, o3DObject);

  m_GameObjects[Id] = _Object;
  return Id;
}

/**
* \brief
*  ������� �������� �������.
*  
* \param [in] _Id
*  ������������� ����� ���������� �������.
*
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
void GameScene::RemoveObject(const ObjectId_t _Id) /*override*/
{
  if (_Id >= m_GameObjects.size() || m_GameObjects[_Id].empty()) return;

  m_Produce3DObject.Remove(_Id);

  for (const auto & pComponent : m_GameObjects[_Id])
  {
    m_pRenders->Remove(pComponent);
  }

  const auto itRemove = ::std::remove_if(
    ::std::begin(m_Updaters), ::std::end(m_Updaters),
    [&](const ::std::pair<ObjectId_t, Render_t> & _Object) 
    { 
      return _Object.first == _Id;
    });

  m_Updaters.erase(itRemove, ::std::end(m_Updaters));
  m_GameObjects[_Id].clear();
  m_Id.AddRemovedObjectId(_Id);
}

/**
* \brief
*  ������� ���������� �����.
* \details
*  - ������� ���������� ������� ���������� \b Updater ��� ���� ���������
*  ��������, � ������� �� ��� ������.
*  - ���������������, ��� ������� ����� ���������� � ������ ���������� ����� �
*  ��� updater'� �������� ����� ��������� �������������� ��������, �������
*  ������ ���������� � ������� ����� � ������� ����������.
*/
void GameScene::Update(void) const
{
  for (const auto & Updater : m_Updaters) Updater.second();
}
