
#pragma once
#include <alicorn/std/vector.hpp>
#include "Defines.hpp"

namespace basement
{

namespace support
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс информации об одной сцене.
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
class Scene
{
public:
  Objects_t Objects;
  Objects_t Updaters;
};

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации игровой сцены.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  10 Март 2019    \n
*  25 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class GameScene final
{
  using Callback_t = ::std::function<void(const Id_t)>;

public:
  inline void Add(const size_t _SceneId, const Id_t _ObjectId, 
    const bool _IsUpdater = false)
  {
    if (_SceneId >= m_BkScenes.size())
    {
      m_BkScenes.resize(_SceneId + 1);
    }

    m_BkScenes[_SceneId].Objects.push_back(_ObjectId);
    if (_IsUpdater) m_BkScenes[_SceneId].Updaters.push_back(_ObjectId);
  }

  inline void Complete(void)
  {
    m_Scenes = m_BkScenes;
    m_BkScenes.clear();
  }

  void CallForEach(const Callback_t & _CbUpdater, const Callback_t & _CbObject)
  {
    ForEach(m_Scenes, [&](const Scene & _Scene)
    {
      if (_CbUpdater != nullptr)
      {
        ForEach(_Scene.Updaters, 
          [&](const Id_t & _ObjectId) { _CbUpdater(_ObjectId); });
      }

      if (_CbObject != nullptr)
      {
        ForEach(_Scene.Objects, 
          [&](const Id_t & _ObjectId) { _CbObject(_ObjectId); });
      }
    });
  }

private:
  template<class TContainer, class TCallback>
  inline void ForEach(const TContainer & _Container, const TCallback & _Callback)
  {
    ::std::for_each(_Container.cbegin(), _Container.cend(), _Callback);
  }

private:
  ::std::vector<Scene> m_Scenes;
  ::std::vector<Scene> m_BkScenes;
};

} // namespace support

} // namespace basement
