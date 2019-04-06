
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
      const auto Id = m_FreeIds.front();
      m_FreeIds.pop();
      return Id;
    }

    return m_NextId++;
  }

  void AddRemovedObjectId(const Id_t _Id)
  {
    m_FreeIds.push(_Id);
  }

private:
  Id_t m_NextId = 0;
  ::std::queue<Id_t> m_FreeIds;

public:
  ~Id(void)
  {
    LOGGER(Info) << "Maximum number of objects: " << m_NextId;
  }
};

DbComponents::DbComponents(void) :
  m_pId(::std::make_shared<Id>())
{
  LOGGER(Trace) << "Create DbComponents.";
}

DbComponents::~DbComponents(void)
{
  LOGGER(Trace) << "Destroy DbComponents.";
}

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

  LOGGER(Trace) << "Create object: id = " << Id;

  if(Id >= m_Objects.size()) m_Objects.resize(Id + 1);

  m_Objects[Id] = _Object;

  DoAddObject(Id);

  return Id;
}

void DbComponents::RemoveGameObject(const Id_t _Id)
{
  DoRemoveObject(_Id);

  LOGGER(Trace) << "Remove object: id = " << _Id;

  m_Objects[_Id].clear();

  m_pId->AddRemovedObjectId(_Id);
}

ComponentPtr_t DbComponents::GetComponent(const Id_t _ObjectId, 
  const String_t & _ComponentType, const String_t & _ComponentKind) const
{
  throw STD_EXCEPTION << "Not released.";
}

} // namespace model

} // namespace basement
