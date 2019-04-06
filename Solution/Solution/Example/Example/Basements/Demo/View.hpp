
#pragma once
#include "Defines.hpp"
#include "DbRenders.hpp"
#include "3DScene.hpp"

namespace basement
{

namespace view
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���������� \b ������������� �������� MVC.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class View final :
  public IDbRenders,
  public I3DScene
{
public:
  // ��������� IDbRenders
  void AddObject(const Id_t, const Object_t &) override;
  void RemoveObject(const Id_t, const Object_t &) override;

public:
  // ��������� I3DScene
  void SetGameScene(const GameScenePtr_t &) override;
  void Render(void) override;

private:
  DbRenders m_DbRenders;
  C3DScene  m_3DScene;

public:
  explicit View(const RendersPtr_t &);
};

} // namespace view

} // namespace basement
