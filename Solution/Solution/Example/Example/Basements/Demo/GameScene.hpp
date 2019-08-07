
#pragma once
#include <vector>
#include <Covellite/Api/robin_hood.hpp>
#include "Defines.hpp"
#include "CubeCoords.hpp"
#include "Constants.hpp"

namespace basement
{

namespace support
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации игровой сцены.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
* \date
*  10 Март 2019    \n
*  25 Март 2019    \n
*  09 Апрель 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class GameScene final
{
  using SceneObjects_t = ::robin_hood::unordered_map<uint64_t, ::std::vector<Id_t>>;
  using CellObject_t = ::robin_hood::pair<uint64_t, ::std::vector<Id_t>>;
  using Callback_t = ::std::function<void(const Id_t)>;
  using RenderObject_t = ::std::pair<float, Id_t>;

public:
  class Camera final
  {
  public:
    bool IsVisible(const model::CubeCoords &) const;
    float GetSquaredDistance(const model::CubeCoords &) const;

  private:
    float m_X;
    float m_Y;
    float m_CameraOffsetX;
    float m_CameraOffsetY;
    float m_Direction;

  public:
    explicit Camera(const float = 0.0f, const float = 0.0f, const float = 0.0f);
  };

private:
  /// Класс информации об одной сцене.
  class Scene
  {
  public:
    SceneObjects_t Objects;
  };

public:
  void SetCameraInfo(const Camera &);
  void Render(const Callback_t &);
  void ProcessAll(const Callback_t &);

public:
  void Add(const size_t, const ::std::vector<Id_t>);
  void Add(const size_t, const ::std::vector<Id_t>, const model::CubeCoords &);
  ::std::vector<Id_t> Remove(const model::CubeCoords &);
  void CompleteReplace(void);
  void CompleteUpdate(void);

private:
  void BuildRenderObjects(void);
  void Add(const size_t, const ::std::vector<Id_t>, const uint64_t);

private:
  ::std::vector<Scene>          m_Scenes;
  ::std::vector<Scene>          m_BkScenes;
  Camera                        m_Camera;
  ::std::vector<RenderObject_t> m_PrepareRenderObjects;
  ::std::vector<RenderObject_t> m_CompleteRenderObjects;
  bool                          m_IsUpdateRenderObjects = false;
};

// cppcheck-suppress passedByValue
inline void GameScene::Add(
  const size_t _SceneId,
  const ::std::vector<Id_t> _ObjectIds)
{
  Add(_SceneId, _ObjectIds, static_cast<uint64_t>(0));
}

inline void GameScene::Add(
  const size_t _SceneId,
  const ::std::vector<Id_t> _ObjectIds,
  const model::CubeCoords & _Position)
{
  Add(_SceneId, _ObjectIds, _Position.GetHash());
}

inline void GameScene::Add(
  const size_t _SceneId,
  const ::std::vector<Id_t> _ObjectIds,
  const uint64_t _CoordHash)
{
  if (_SceneId >= m_BkScenes.size()) m_BkScenes.resize(_SceneId + 1);
  m_BkScenes[_SceneId].Objects[_CoordHash] = _ObjectIds;
}

} // namespace support

} // namespace basement
