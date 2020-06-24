
#pragma once
#include <Covellite/Api/Component.hpp>
#include <Covellite/Expanse/Defines.hpp>
#include "3DScene.hpp"

namespace mock
{

namespace covellite
{

namespace expanse
{

class GameScene
{
  using RendersPtr_t = ::std::shared_ptr<::covellite::api::Component::Renders>;
  using ObjectId_t = ::covellite::expanse::ObjectId_t;
  using GameObject_t = ::covellite::expanse::GameObject_t;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, Id_t(RendersPtr_t, Id_t));
    MOCK_METHOD2(CreateObject, ObjectId_t(Id_t, const GameObject_t &));
    MOCK_METHOD2(RemoveObject, void(Id_t, ObjectId_t));
    MOCK_METHOD1(Update, void(Id_t));
  };

public:
  ObjectId_t CreateObject(const GameObject_t & _Object)
  {
    return Proxy::GetInstance()->CreateObject(m_Id, _Object);
  }

  void RemoveObject(const ObjectId_t _Id)
  {
    Proxy::GetInstance()->RemoveObject(m_Id, _Id);
  }

  void Update(void)
  {
    Proxy::GetInstance()->Update(m_Id);
  }

public:
  const Id_t m_Id;

public:
  GameScene(const RendersPtr_t & _pRenders, C3DScene & _C3DScene) :
    m_Id(Proxy::GetInstance()->Constructor(_pRenders, _C3DScene.m_Id))
  {

  }
};

} // namespace expanse

} // namespace covellite

} // namespace mock
