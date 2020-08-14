
#pragma once
#include <set>
#include <Covellite/Expanse/I3DScene.hpp>
#include "IProduce3DObject.impl.hpp"

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  Класс входит в проект \ref CovelliteExpansePage \n
*  Класс-контейнер для объектов, которые должны рендеринтся в текущем кадре.
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
class C3DScene final :
  public IProduce3DObject,
  public I3DScene
{
  using Object_t = ::std::pair<size_t, ObjectId_t>;

public:
  // Интерфейс IProduce3DObject:
  void Add(const ObjectId_t, const C3DObject_t &) override;
  void Remove(const ObjectId_t) override;

public:
  // Интерфейс I3DScene:
  void Add(const size_t, const ObjectId_t, const size_t = 0) override;
  void Add(const ObjectId_t) override;

public:
  void Render(void);

private:
  const C3DObject_t & Get(const ObjectId_t) const;

private:
  struct CombinationLess
  {
    inline bool operator()(const Object_t & _Left, const Object_t & _Right) const noexcept
    {
      return _Left.first < _Right.first;
    }
  };

  ::std::vector<C3DObject_t> m_Renders;
  ::std::vector<::std::multiset<Object_t, CombinationLess>> m_RenderQueueDepracated;
  ::std::vector<ObjectId_t> m_RenderQueue;

public:
  C3DScene(void);
  C3DScene(const C3DScene &) = delete;
  C3DScene(C3DScene &&) = delete;
  C3DScene & operator= (const C3DScene &) = delete;
  C3DScene & operator= (C3DScene &&) = delete;
  ~C3DScene(void) = default;

private:
  FRIEND_TEST(C3DScene_test, /*DISABLED_*/Test_Produce3DObjects);
};

} // namespace expanse

} // namespace covellite
