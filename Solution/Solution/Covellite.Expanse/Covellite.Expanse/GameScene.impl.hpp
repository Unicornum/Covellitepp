
#pragma once
#include <Covellite/Expanse/IGameScene.hpp>
#include <Covellite/Api/Component.hpp>
#include "Id.impl.hpp"

namespace covellite
{

namespace expanse
{

class IProduce3DObject;

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  Класс входит в проект \ref CovelliteExpansePage \n
*  Класс-контейнер для объектов, которые должны обновляться в текущем кадре.
* \details
*  - Подробное описание класса.
*
* \version
*  1.0.0.0        \n
* \date
*  18 Июнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
class GameScene final :
  public IGameScene
{
  using RendersPtr_t = ::std::shared_ptr<api::Component::Renders>;

public:
  // Интерфейс IGameScene:
  ObjectId_t CreateObject(const GameObject_t &) override;
  void RemoveObject(const ObjectId_t) override;

public:
  void Update(void) const;

private:
  const RendersPtr_t m_pRenders;
  IProduce3DObject & m_Produce3DObject;
  Id m_Id;
  ::std::vector<GameObject_t> m_GameObjects;
  ::std::vector<::std::pair<ObjectId_t, Render_t>> m_Updaters;

public:
  GameScene(const RendersPtr_t &, IProduce3DObject &) noexcept;
  GameScene(const GameScene &) = delete;
  GameScene(GameScene &&) = delete;
  GameScene & operator= (const GameScene &) = delete;
  GameScene & operator= (GameScene &&) = delete;
  ~GameScene(void);
};

} // namespace expanse

} // namespace covellite
