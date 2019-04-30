
#include "stdafx.h"
#include "DbUpdaters.hpp"
#include <alicorn/std/exception.hpp>
#include <alicorn/logger.hpp>

namespace basement
{

namespace model
{

DbUpdaters::DbUpdaters(void)
{
  LOGGER(Trace) << "Create DbUpdaters.";
}

DbUpdaters::~DbUpdaters(void)
{
  LOGGER(Trace) << "Destroy DbUpdaters.";
}

void DbUpdaters::AddUpdater(const Id_t & _Id, const Updater_t & _Updater)
{
  m_Objects[_Id] = _Updater;
}

// cppcheck-suppress passedByValue
void DbUpdaters::RemoveUpdater(const Id_t _Id)
{
  m_Objects.erase(_Id);
}

// cppcheck-suppress passedByValue
void DbUpdaters::CallUpdater(const Id_t _Id) const
{
  const auto itUpdater = m_Objects.find(_Id);
  if (itUpdater == m_Objects.end())
  {
    throw STD_EXCEPTION << "Unexpected updater id: " << _Id;
  }

  itUpdater->second();
}

} // namespace model

} // namespace basement
