
#include "stdafx.h"
#include "DbComponents.hpp"
#include <queue>
#include <alicorn/std/exception.hpp>
#include <alicorn/logger.hpp>

namespace basement
{

namespace model
{

class DbComponents::Id final
{
public:
  Id_t GetFreeId(void)
  {
    if (!m_FreeIds.empty())
    {
      const auto Result = m_FreeIds.front();
      m_FreeIds.pop();
      return Result;
    }

    return m_NextId++;
  }

  // cppcheck-suppress passedByValue
  void AddRemovedObjectId(const Id_t _Id)
  {
    m_FreeIds.push(_Id);
  }

private:
  Id_t m_NextId = 0;
  ::std::queue<Id_t> m_FreeIds;

public:
  ~Id(void) noexcept
  {
    LOGGER(Info) << "Maximum number of objects: " << m_NextId;
  }
};

DbComponents::DbComponents(void) :
  m_pId(::std::make_shared<Id>())
{
  LOGGER(Trace) << "Create DbComponents.";
}

DbComponents::~DbComponents(void) noexcept
{
  LOGGER(Trace) << "Destroy DbComponents.";
}

// cppcheck-suppress passedByValue
const Object_t & DbComponents::GetObject(const Id_t _Id) const /*override*/
{
  if (_Id >= m_Objects.size())
  {
    throw STD_EXCEPTION << "Unexpected object id: " << _Id;
  }

  return m_Objects[_Id];
}

ComponentPtr_t DbComponents::GetComponent(const String_t & _ComponentId) const /*override*/
{
  for (const auto & Object : m_Objects)
  {
    for (const auto & pComponent : Object)
    {
      if (pComponent->Id == _ComponentId) return pComponent;
    }
  }

  throw STD_EXCEPTION << "Unexpected component id: " << _ComponentId;
}

Id_t DbComponents::AddGameObject(const Object_t & _Object)
{
  const auto Id = m_pId->GetFreeId();

  LOGGER_DEBUG(Trace) << "Create object: id = " << Id;

  if(Id >= m_Objects.size()) m_Objects.resize(Id + 1);

  m_Objects[Id] = _Object;

  DoAddObject(Id);

  return Id;
}

// cppcheck-suppress passedByValue
void DbComponents::RemoveGameObject(const Id_t _Id)
{
  DoRemoveObject(_Id);

  LOGGER_DEBUG(Trace) << "Remove object: id = " << _Id;

  m_Objects[_Id].clear();

  m_pId->AddRemovedObjectId(_Id);
}

} // namespace model

} // namespace basement
