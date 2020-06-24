
#include "stdafx.h"
#include "DbRenders.hpp"
#include <alicorn/logger.hpp>

namespace basement
{

namespace view
{

DbRenders::DbRenders(const RendersPtr_t & _pRenders) :
  m_pRenders(_pRenders)
{
  LOGGER(Trace) << "Create DbRenders.";
}

DbRenders::~DbRenders(void) noexcept
{
  LOGGER(Trace) << "Destroy DbRenders.";
}

// cppcheck-suppress passedByValue
void DbRenders::AddObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  LOGGER_DEBUG(Trace) << "Create renders for object: id = " << _Id;

  if (_Id >= m_Renders.size())
  {
    m_Renders.resize(_Id + 1, 
      { [](void) { throw STD_EXCEPTION << "Unexpected render."; } });
  }

  m_Renders[_Id].clear();

  for (const auto & pComponent : _Object)
  {
    if (pComponent->Type == uT("Data")) continue;

    m_Renders[_Id].push_back(m_pRenders->Obtain(pComponent));
  }
}

// cppcheck-suppress passedByValue
void DbRenders::RemoveObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  LOGGER_DEBUG(Trace) << "Remove renders for object: id = " << _Id;

  for (const auto & pComponent : _Object)
  {
    if (pComponent->Type == uT("Data")) continue;

    m_pRenders->Remove(pComponent);
  }

  m_Renders[_Id] = 
  { 
    [=](void) 
    { 
      throw STD_EXCEPTION << "Using render for removed object: " << _Id;
    }
  };
}

// cppcheck-suppress passedByValue
const Renders_t & DbRenders::GetRenders(const Id_t _Id) const
{
  if (_Id >= m_Renders.size())
  {
    throw STD_EXCEPTION << "Unexpected object id: " << _Id;
  }

  return m_Renders[_Id];
}

} // namespace view

} // namespace basement
