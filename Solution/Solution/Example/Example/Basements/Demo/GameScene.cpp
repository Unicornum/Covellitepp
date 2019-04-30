
#include "stdafx.h"
#include "GameScene.hpp"
#include "IGameObject.hpp"

namespace basement
{

namespace support
{

namespace math = ::alicorn::extension::cpp::math;


GameScene::Camera::Camera(const float _X, const float _Y, const float _Direction) :
  m_X(_X),
  m_Y(_Y),
  m_CameraOffsetX(_X + Constant::Camera::BackOffset * math::radian::Cos(_Direction)),
  m_CameraOffsetY(_Y + Constant::Camera::BackOffset * math::radian::Sin(_Direction)),
  m_Direction(_Direction)
{

}

bool GameScene::Camera::IsVisible(const model::CubeCoords & _CellPosition) const
{
  namespace math = ::alicorn::extension::cpp::math;

  constexpr auto DoublePi = 2.0f * math::Constant<float>::Pi;
  constexpr auto HalfPi = math::Constant<float>::Pi / 2.0f;

  const auto ObjectPosition = _CellPosition.ToPlane();

  const auto DeltaX = ObjectPosition.first - m_CameraOffsetX;
  const auto DeltaY = ObjectPosition.second - m_CameraOffsetY;
  auto ObjectAngle = math::radian::ArcTan(-DeltaX, -DeltaY);

  if (math::Abs(m_Direction) > HalfPi &&
    math::Sign(m_Direction) != math::Sign(ObjectAngle))
  {
    ObjectAngle += math::Sign(m_Direction) * DoublePi;
  }

  return math::Abs(ObjectAngle - m_Direction) < (Constant::Camera::Fov / 2.0f);
}

float GameScene::Camera::GetSquaredDistance(const model::CubeCoords & _CellPosition) const
{
  const auto ObjectPosition = _CellPosition.ToPlane();

  const auto DeltaX = ObjectPosition.first - m_CameraOffsetX;
  const auto DeltaY = ObjectPosition.second - m_CameraOffsetY;

  return DeltaX * DeltaX + DeltaY * DeltaY;
}

// ************************************************************************** //

template<class TContainer, class TCallback>
inline void ForEach(const TContainer & _Container, const TCallback & _Callback)
{
  ::std::for_each(_Container.cbegin(), _Container.cend(), _Callback);
}

void GameScene::SetCameraInfo(const Camera & _Camera)
{
  m_Camera = _Camera;
  BuildRenderObjects();
}

::std::vector<Id_t> GameScene::Remove(const model::CubeCoords & _CellPosition)
{
  const auto CellHash = _CellPosition.GetHash();

  for (auto & Scene : m_Scenes)
  {
    if (Scene.Objects.empty()) continue;

    const auto itObject = Scene.Objects.find(CellHash);
    if (itObject != Scene.Objects.end())
    {
      const auto Result = itObject->second;
      Scene.Objects.erase(itObject);
      return Result;
    }
  }

  throw STD_EXCEPTION << "Object for cell [" <<
    _CellPosition.GetX() << ", " << _CellPosition.GetY() << "] not found.";
}

void GameScene::CompleteReplace(void)
{
  m_Scenes = ::std::move(m_BkScenes);
  m_RenderObjects.clear();
}

void GameScene::CompleteUpdate(void)
{
  if (m_Scenes.size() < m_BkScenes.size())
  {
    m_Scenes.resize(m_BkScenes.size());
  }

  for (::std::size_t i = 0; i < m_BkScenes.size(); i++)
  {
    m_Scenes[i].Objects.insert(
      m_BkScenes[i].Objects.cbegin(), m_BkScenes[i].Objects.cend());
  }

  m_BkScenes.clear();
  BuildRenderObjects();
}

void GameScene::Update(const Callback_t & _CbUpdater)
{
  ForEach(m_Scenes, [&_CbUpdater](const Scene & _Scene)
  {
    ForEach(_Scene.Updaters, [&_CbUpdater](const Id_t & _ObjectId)
    {
      _CbUpdater(_ObjectId);
    });
  });
}

void GameScene::Render(const Callback_t & _CbObject)
{
  ForEach(m_RenderObjects, [&_CbObject](const RenderObject_t & _Object)
  {
    _CbObject(_Object.second);
  });
}

void GameScene::ProcessAll(const Callback_t & _CbObject)
{
  ForEach(m_Scenes, [&](const Scene & _Scene)
  {
    using CellObject_t = ::std::pair<uint64_t, ::std::vector<Id_t>>;

    ForEach(_Scene.Objects, [&](const CellObject_t & _Object)
    {
      ForEach(_Object.second, [&](const Id_t & _ObjectId)
      {
        _CbObject(_ObjectId);
      });
    });
  });
}

void GameScene::BuildRenderObjects(void)
{
  using CellObject_t = ::std::pair<uint64_t, ::std::vector<Id_t>>;

  ::std::vector<RenderObject_t> SceneRenderObjects;

  const auto BildSortedFromCameraDistance = 
    [this, &SceneRenderObjects](const CellObject_t & _Object)
  {
    const auto InsertInSorted =
      [](auto & _Container, const RenderObject_t & _Item)
    {
      _Container.insert(
        ::std::upper_bound(_Container.begin(), _Container.end(), _Item,
          [](const RenderObject_t & _Item1, const RenderObject_t & _Item2)
      {
        return _Item1.first > _Item2.first;
      }), _Item);
    };

    if (_Object.first == 0) // Этот объект не привязан к ячейке.
    {
      InsertInSorted(SceneRenderObjects, RenderObject_t
        {
          math::Infinity<float>(), // Этот объект содержит общий набор 
                                   // рендеров для всех объектов и поэтому 
                                   // должен гарантированно быть самым 
                                   // дальним, чтобы рендерится первым.
          _Object.second[0]
        });
      return;
    }

    const auto Position = model::CubeCoords{ _Object.first };

    const auto SquaredDistance = m_Camera.GetSquaredDistance(Position);

    if (SquaredDistance >= Constant::Camera::NearClippingPlane &&
      SquaredDistance <= Constant::Camera::FarClippingPlane &&
      m_Camera.IsVisible(Position))
    {
      auto UsingObject = _Object.second[0];

      if (_Object.second.size() == 3)
      {
        const auto Distance = Constant::Camera::FarClippingPlane - 
          Constant::Camera::NearClippingPlane;

        if (SquaredDistance >
          Constant::Camera::NearClippingPlane + Distance / 4.0f)
        {
          UsingObject = _Object.second[1];
        }

        if (SquaredDistance >
          Constant::Camera::NearClippingPlane + Distance / 1.77f)
        {
          UsingObject = _Object.second[2];
        }
      }

      InsertInSorted(SceneRenderObjects,
        RenderObject_t{ SquaredDistance, UsingObject });
    }
  };

  const auto BildSimple = 
    [&SceneRenderObjects](const CellObject_t & _Object)
  {
    SceneRenderObjects.push_back(RenderObject_t{ 0.0f, _Object.second[0] });
  };

  m_RenderObjects.clear();

  using namespace ::alicorn::extension::std;

  for (::std::size_t i = 0; i < m_Scenes.size(); i++)
  {
    // В Debug версии без сортировки fps в 2 раза ниже!??
    // В Release версии в окне на весь экран fps не зависит от
    // наличия/отсутствия сортировки, в маленьком окне fps также меньше 
    // в 2 раза.
    ForEach(m_Scenes[i].Objects, BildSortedFromCameraDistance);
    //ForEach(m_Scenes[i].Objects, BildSimple);

    //if ((i & model::IGameObject::Type::LandscapeMask) == 0)
    {
      m_RenderObjects += SceneRenderObjects;
      SceneRenderObjects.clear();
    }
  }
}

} // namespace support

} // namespace basement
