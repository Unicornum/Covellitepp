
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
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации \b контроллера паттерна MVC.
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
class Controller final
{
public:
  Controller(model::IDbComponents &, view::IDbRenders &);
  Controller(model::IGameWorld &, view::I3DScene &);
};

} // namespace controller

} // namespace basement
