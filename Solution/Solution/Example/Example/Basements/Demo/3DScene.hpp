
#pragma once
#include <chrono>
#include "I3DScene.hpp"

namespace basement
{

namespace view
{

class DbRenders;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации рендеринга 3D сцены.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class C3DScene final :
  public I3DScene
{
  using Time_t = ::std::chrono::time_point<::std::chrono::system_clock>;

public:
  // Интерфейс I3DScene:
  void SetGameScene(const GameScenePtr_t &) override;
  void Render(void) override;

private:
  void RenderToLog(const ::std::chrono::seconds &);

private:
  DbRenders &     m_DbRenders;
  GameScenePtr_t  m_pGameScene;
  Time_t          m_Time;

public:
  explicit C3DScene(DbRenders &);
};

} // namespace view

} // namespace basement
