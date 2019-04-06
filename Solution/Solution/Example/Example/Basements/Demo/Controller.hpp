
#pragma once
#include "IDbComponents.hpp"
#include "IGameWorld.hpp"
#include "IDbRenders.hpp"
#include "I3DScene.hpp"

namespace basement
{

namespace controller
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���������� \b ����������� �������� MVC.
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
class Controller final
{
public:
  Controller(model::IDbComponents &, view::IDbRenders &);
  Controller(model::IGameWorld &, view::I3DScene &);
};

} // namespace controller

} // namespace basement
