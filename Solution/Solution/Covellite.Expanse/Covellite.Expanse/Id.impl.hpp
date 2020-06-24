
#pragma once
#include <queue>
#include <Covellite/Expanse/Defines.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  Класс входит в проект \ref CovelliteExpansePage \n
*  Класс идентификатора объекта.
* \details
*  - Нужен для того, чтобы гарантировать, что каждому игровому/3D объекту
*  соответствует уникальный идентификатор и при этом идентификаторы всех
*  активных объектов образуют непрерывную последовательность от 0 до
*  максимального значения (тогда 3D объекты можно хранить в vector'e и
*  получать их очень быстро).
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
class Id final
{
public:
  ObjectId_t GetFreeId(void) noexcept;
  void AddRemovedObjectId(const ObjectId_t & _Id);

private:
  ObjectId_t m_NextId = 0;
  ::std::queue<ObjectId_t> m_FreeIds;

public:
  Id(void) = default;
  Id(const Id &) = delete;
  Id(Id &&) = delete;
  Id & operator= (const Id &) = delete;
  Id & operator= (Id &&) = delete;
  ~Id(void);
};

} // namespace expanse

} // namespace covellite
