
#include "stdafx.h"
#include "Controller.hpp"

using namespace basement::controller;

//! @cond Doxygen_Suppress

Controller::Controller(
  ::basement::model::IDbComponents & _DbComponents,
  ::basement::view::IDbRenders & _DbRenders)
{
  _DbComponents.DoAddObject.connect([&](const Id_t _Id)
  {
     _DbRenders.AddObject(_Id, _DbComponents.GetObject(_Id));
  });

  _DbComponents.DoRemoveObject.connect([&](const Id_t _Id)
  {
    _DbRenders.RemoveObject(_Id, _DbComponents.GetObject(_Id));
  });
}

Controller::Controller(
  ::basement::model::IGameWorld & _GameWorld, 
  ::basement::view::I3DScene & _3DScene)
{
  _3DScene.SetGameScene(_GameWorld.CreateGameScene());
}

//! @endcond