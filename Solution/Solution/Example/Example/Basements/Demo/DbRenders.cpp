
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

DbRenders::~DbRenders(void)
{
  LOGGER(Trace) << "Destroy DbRenders.";
}

void DbRenders::AddObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  LOGGER(Trace) << "Create renders for object: id = " << _Id;

  if (_Id >= m_Renders.size())
  {
    m_Renders.resize(_Id + 1, 
      { [](void) { throw STD_EXCEPTION << "Unexpected render."; } });
  }

  m_Renders[_Id] = m_pRenders->Obtain(_Object);
}

void DbRenders::RemoveObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  LOGGER(Trace) << "Remove renders for object: id = " << _Id;

  m_pRenders->Remove(_Object);

  m_Renders[_Id] = 
  { 
    [=](void)
    {
      throw STD_EXCEPTION << "Using render for removed object: " << _Id;
    }
  };
}

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
